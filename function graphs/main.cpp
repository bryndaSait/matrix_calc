#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <cstring>
#include <functional>
#include <cctype>
#include <algorithm>

// Helper function for expression evaluation (outside the class)
class ExpressionEvaluator {
public:
    static double evaluate(const std::string& expr, 
                          const std::vector<std::pair<std::string, std::function<double(double)>>>& funcs) {
        try {
            std::string e = expr;
            
            // Remove spaces
            e.erase(std::remove_if(e.begin(), e.end(), ::isspace), e.end());
            
            // Handle function calls first
            for (const auto& funcPair : funcs) {
                const std::string& funcName = funcPair.first;
                size_t pos = e.find(funcName + "(");
                if (pos != std::string::npos) {
                    size_t openParen = pos + funcName.length();
                    int parenCount = 1;
                    size_t closeParen = openParen + 1;
                    while (closeParen < e.length() && parenCount > 0) {
                        if (e[closeParen] == '(') parenCount++;
                        else if (e[closeParen] == ')') parenCount--;
                        closeParen++;
                    }
                    
                    if (parenCount == 0) {
                        closeParen--;
                        std::string arg = e.substr(openParen + 1, closeParen - openParen - 1);
                        double argVal = evaluate(arg, funcs);
                        double funcResult = funcPair.second(argVal);
                        
                        std::string resultStr = std::to_string(funcResult);
                        e.replace(pos, closeParen - pos + 1, resultStr);
                        // Restart function evaluation
                        return evaluate(e, funcs);
                    }
                }
            }
            
            // Handle multiplication (implied)
            std::string processed = "";
            for (size_t i = 0; i < e.length(); i++) {
                if (i > 0 && i < e.length() - 1) {
                    if (isdigit(e[i-1]) && e[i] == 'x') {
                        processed += '*';
                    }
                }
                processed += e[i];
            }
            e = processed;
            
            // Now evaluate using recursive descent
            return parseExpression(e, 0, e.length() - 1);
        } catch (...) {
            return NAN;
        }
    }
    
private:
    static double parseExpression(const std::string& e, size_t start, size_t end) {
        if (start >= e.length() || end >= e.length() || start > end) {
            return NAN;
        }
        
        // Remove outer parentheses
        if (e[start] == '(' && e[end] == ')') {
            int parenCount = 0;
            bool hasInnerParen = false;
            for (size_t i = start + 1; i < end; i++) {
                if (e[i] == '(') parenCount++;
                else if (e[i] == ')') parenCount--;
                if (parenCount == 0 && i < end - 1) {
                    hasInnerParen = true;
                    break;
                }
            }
            if (!hasInnerParen) {
                return parseExpression(e, start + 1, end - 1);
            }
        }
        
        // Find operators
        int parenCount = 0;
        int lastPlus = -1, lastMinus = -1;
        int lastMul = -1, lastDiv = -1;
        
        for (size_t i = start; i <= end && i < e.length(); i++) {
            if (e[i] == '(') parenCount++;
            else if (e[i] == ')') parenCount--;
            else if (parenCount == 0) {
                if (e[i] == '+' && i > start) lastPlus = i;
                else if (e[i] == '-' && i > start) lastMinus = i;
                else if (e[i] == '*') lastMul = i;
                else if (e[i] == '/') lastDiv = i;
            }
        }
        
        if (lastPlus != -1) {
            double left = parseExpression(e, start, lastPlus - 1);
            double right = parseExpression(e, lastPlus + 1, end);
            if (std::isnan(left) || std::isnan(right)) return NAN;
            return left + right;
        }
        if (lastMinus != -1) {
            double left = parseExpression(e, start, lastMinus - 1);
            double right = parseExpression(e, lastMinus + 1, end);
            if (std::isnan(left) || std::isnan(right)) return NAN;
            return left - right;
        }
        if (lastMul != -1) {
            double left = parseExpression(e, start, lastMul - 1);
            double right = parseExpression(e, lastMul + 1, end);
            if (std::isnan(left) || std::isnan(right)) return NAN;
            return left * right;
        }
        if (lastDiv != -1) {
            double left = parseExpression(e, start, lastDiv - 1);
            double right = parseExpression(e, lastDiv + 1, end);
            if (std::isnan(left) || std::isnan(right) || right == 0) return NAN;
            return left / right;
        }
        
        // Parse number
        std::string numStr = e.substr(start, end - start + 1);
        char* endptr;
        double val = strtod(numStr.c_str(), &endptr);
        if (endptr == numStr.c_str() + numStr.length()) {
            return val;
        }
        
        return NAN;
    }
};

class FunctionPlotter {
private:
    Display* display;
    Window window;
    GC gc;
    int screen;
    unsigned long white, black;
    int width, height;
    std::string functionStr;
    std::function<double(double)> func;
    
    // GUI elements
    static const int TEXT_HEIGHT = 20;
    static const int INPUT_Y = 10;
    static const int PLOT_MARGIN = 40;
    static const int INPUT_WIDTH = 300;
    static const int INPUT_HEIGHT = 25;
    
    // Input buffer
    char inputBuffer[256];
    int cursorPos;
    bool inputActive;
    
    // Plot ranges
    double xMin, xMax, yMin, yMax;
    
public:
    FunctionPlotter(int w = 800, int h = 600) 
        : width(w), height(h), xMin(-10), xMax(10), yMin(-10), yMax(10),
          functionStr("sin(x)"), inputActive(false), cursorPos(0) {
        
        strcpy(inputBuffer, "sin(x)");
        cursorPos = strlen(inputBuffer);
        
        // Initialize X11
        display = XOpenDisplay(NULL);
        if (!display) {
            std::cerr << "Cannot open display" << std::endl;
            exit(1);
        }
        
        screen = DefaultScreen(display);
        white = WhitePixel(display, screen);
        black = BlackPixel(display, screen);
        
        window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                     0, 0, width, height, 1, black, white);
        
        XSelectInput(display, window, 
                     ExposureMask | KeyPressMask | ButtonPressMask | 
                     StructureNotifyMask | FocusChangeMask);
        
        gc = XCreateGC(display, window, 0, NULL);
        XSetForeground(display, gc, black);
        
        XMapWindow(display, window);
        XStoreName(display, window, "Function Plotter");
        
        // Set default function
        setFunction("sin(x)");
    }
    
    ~FunctionPlotter() {
        XFreeGC(display, gc);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }
    
    void setFunction(const std::string& expr) {
        functionStr = expr;
        func = createFunction(expr);
    }
    
    std::function<double(double)> createFunction(const std::string& expr) {
        // Define available functions
        std::vector<std::pair<std::string, std::function<double(double)>>> funcs = {
            {"sin", [](double a){ return std::sin(a); }},
            {"cos", [](double a){ return std::cos(a); }},
            {"tan", [](double a){ return std::tan(a); }},
            {"asin", [](double a){ return std::asin(a); }},
            {"acos", [](double a){ return std::acos(a); }},
            {"atan", [](double a){ return std::atan(a); }},
            {"sinh", [](double a){ return std::sinh(a); }},
            {"cosh", [](double a){ return std::cosh(a); }},
            {"tanh", [](double a){ return std::tanh(a); }},
            {"exp", [](double a){ return std::exp(a); }},
            {"log", [](double a){ return std::log(a); }},
            {"log10", [](double a){ return std::log10(a); }},
            {"sqrt", [](double a){ return std::sqrt(a); }},
            {"abs", [](double a){ return std::abs(a); }},
            {"fabs", [](double a){ return std::fabs(a); }},
            {"floor", [](double a){ return std::floor(a); }},
            {"ceil", [](double a){ return std::ceil(a); }}
        };
        
        // Return a lambda that evaluates the expression
        return [expr, funcs](double x) -> double {
            try {
                std::string e = expr;
                
                // Replace constants
                auto replaceAll = [](std::string& str, const std::string& from, const std::string& to) {
                    size_t start_pos = 0;
                    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
                        str.replace(start_pos, from.length(), to);
                        start_pos += to.length();
                    }
                };
                
                // Replace x with value (only standalone x)
                std::string xStr = std::to_string(x);
                size_t pos = 0;
                while ((pos = e.find("x", pos)) != std::string::npos) {
                    bool isStandalone = true;
                    if (pos > 0 && (isalnum(e[pos-1]) || e[pos-1] == '_')) {
                        isStandalone = false;
                    }
                    if (pos < e.length()-1 && (isalnum(e[pos+1]) || e[pos+1] == '_')) {
                        isStandalone = false;
                    }
                    
                    if (isStandalone) {
                        e.replace(pos, 1, xStr);
                        pos += xStr.length();
                    } else {
                        pos++;
                    }
                }
                
                replaceAll(e, "pi", std::to_string(M_PI));
                replaceAll(e, "e", std::to_string(M_E));
                
                // Evaluate the expression using the static evaluator
                return ExpressionEvaluator::evaluate(e, funcs);
            } catch (...) {
                return NAN;
            }
        };
    }
    
    double evaluateFunction(double x) {
        try {
            if (func) {
                return func(x);
            }
            return NAN;
        } catch (...) {
            return NAN;
        }
    }
    
    void drawAxes() {
        // Draw grid lines
        XSetForeground(display, gc, 0xCCCCCC);
        
        // Vertical grid lines
        for (double x = std::ceil(xMin); x <= std::floor(xMax); x += 1.0) {
            if (x == 0) continue;
            int xPix = mapX(x);
            XDrawLine(display, window, gc, xPix, PLOT_MARGIN, xPix, height - PLOT_MARGIN);
        }
        
        // Horizontal grid lines
        for (double y = std::ceil(yMin); y <= std::floor(yMax); y += 1.0) {
            if (y == 0) continue;
            int yPix = mapY(y);
            XDrawLine(display, window, gc, PLOT_MARGIN, yPix, width - PLOT_MARGIN, yPix);
        }
        
        // Draw axes (black)
        XSetForeground(display, gc, black);
        XSetLineAttributes(display, gc, 2, LineSolid, CapButt, JoinMiter);
        
        // X axis
        if (yMin <= 0 && yMax >= 0) {
            int yZero = mapY(0);
            XDrawLine(display, window, gc, PLOT_MARGIN, yZero, width - PLOT_MARGIN, yZero);
            
            // Arrow tip
            XDrawLine(display, window, gc, width - PLOT_MARGIN - 10, yZero - 5, 
                      width - PLOT_MARGIN, yZero);
            XDrawLine(display, window, gc, width - PLOT_MARGIN - 10, yZero + 5, 
                      width - PLOT_MARGIN, yZero);
        }
        
        // Y axis
        if (xMin <= 0 && xMax >= 0) {
            int xZero = mapX(0);
            XDrawLine(display, window, gc, xZero, PLOT_MARGIN, xZero, height - PLOT_MARGIN);
            
            // Arrow tip
            XDrawLine(display, window, gc, xZero - 5, PLOT_MARGIN + 10, 
                      xZero, PLOT_MARGIN);
            XDrawLine(display, window, gc, xZero + 5, PLOT_MARGIN + 10, 
                      xZero, PLOT_MARGIN);
        }
        
        // Draw axis labels
        XSetLineAttributes(display, gc, 1, LineSolid, CapButt, JoinMiter);
        char label[32];
        
        // X axis labels
        for (double x = std::ceil(xMin); x <= std::floor(xMax); x += 1.0) {
            if (x == 0) continue;
            int xPix = mapX(x);
            int yPix = mapY(0);
            if (yPix < PLOT_MARGIN || yPix > height - PLOT_MARGIN) {
                yPix = height - PLOT_MARGIN + 20;
            }
            snprintf(label, sizeof(label), "%.0f", x);
            XDrawString(display, window, gc, xPix - 5, yPix + 20, label, strlen(label));
        }
        
        // Y axis labels
        for (double y = std::ceil(yMin); y <= std::floor(yMax); y += 1.0) {
            if (y == 0) continue;
            int yPix = mapY(y);
            int xPix = mapX(0);
            if (xPix < PLOT_MARGIN || xPix > width - PLOT_MARGIN) {
                xPix = PLOT_MARGIN - 30;
            }
            snprintf(label, sizeof(label), "%.0f", y);
            XDrawString(display, window, gc, xPix - 20, yPix + 5, label, strlen(label));
        }
        
        // Origin label
        XDrawString(display, window, gc, mapX(0) - 10, mapY(0) + 20, "0", 1);
    }
    
    int mapX(double x) {
        return PLOT_MARGIN + (int)((x - xMin) / (xMax - xMin) * (width - 2 * PLOT_MARGIN));
    }
    
    int mapY(double y) {
        return height - PLOT_MARGIN - (int)((y - yMin) / (yMax - yMin) * (height - 2 * PLOT_MARGIN));
    }
    
    double unmapX(int xPix) {
        return xMin + (double)(xPix - PLOT_MARGIN) / (width - 2 * PLOT_MARGIN) * (xMax - xMin);
    }
    
    double unmapY(int yPix) {
        return yMin + (double)(height - PLOT_MARGIN - yPix) / (height - 2 * PLOT_MARGIN) * (yMax - yMin);
    }
    
    void drawFunction() {
        XSetForeground(display, gc, 0xFF0000);
        XSetLineAttributes(display, gc, 2, LineSolid, CapButt, JoinMiter);
        
        std::vector<int> points;
        int numPoints = 1000;
        
        for (int i = 0; i <= numPoints; i++) {
            double x = xMin + (double)i / numPoints * (xMax - xMin);
            double y = evaluateFunction(x);
            
            if (!std::isnan(y) && std::isfinite(y) && 
                y >= yMin && y <= yMax) {
                int xPix = mapX(x);
                int yPix = mapY(y);
                points.push_back(xPix);
                points.push_back(yPix);
            } else {
                // If we have points, draw them
                if (points.size() >= 4) {
                    for (size_t j = 0; j < points.size() - 2; j += 2) {
                        XDrawLine(display, window, gc, 
                                 points[j], points[j+1],
                                 points[j+2], points[j+3]);
                    }
                }
                points.clear();
            }
        }
        
        // Draw remaining points
        if (points.size() >= 4) {
            for (size_t j = 0; j < points.size() - 2; j += 2) {
                XDrawLine(display, window, gc, 
                         points[j], points[j+1],
                         points[j+2], points[j+3]);
            }
        }
        
        XSetLineAttributes(display, gc, 1, LineSolid, CapButt, JoinMiter);
    }
    
    void drawInputBox() {
        int inputX = 20;
        int inputY = height - 50;
        
        // Draw input box
        XSetForeground(display, gc, 0xDDDDDD);
        XFillRectangle(display, window, gc, inputX, inputY, INPUT_WIDTH, INPUT_HEIGHT);
        XSetForeground(display, gc, black);
        XDrawRectangle(display, window, gc, inputX, inputY, INPUT_WIDTH, INPUT_HEIGHT);
        
        // Draw label
        XDrawString(display, window, gc, inputX, inputY - 5, "Function: f(x)=", 13);
        
        // Draw input text
        char displayText[256];
        strcpy(displayText, inputBuffer);
        int textLength = strlen(displayText);
        
        // Draw cursor
        if (inputActive) {
            std::string withCursor = std::string(displayText);
            withCursor.insert(cursorPos, "|");
            XDrawString(display, window, gc, inputX + 5, inputY + 17, 
                       withCursor.c_str(), withCursor.length());
        } else {
            XDrawString(display, window, gc, inputX + 5, inputY + 17, 
                       displayText, textLength);
        }
        
        // Draw range information
        char rangeInfo[64];
        snprintf(rangeInfo, sizeof(rangeInfo), "x: [%.1f, %.1f]  y: [%.1f, %.1f]", 
                xMin, xMax, yMin, yMax);
        XDrawString(display, window, gc, inputX + INPUT_WIDTH + 20, inputY + 17, 
                   rangeInfo, strlen(rangeInfo));
    }
    
    void redraw() {
        // Clear window
        XSetForeground(display, gc, white);
        XFillRectangle(display, window, gc, 0, 0, width, height);
        
        // Draw axes and grid
        drawAxes();
        
        // Draw function
        drawFunction();
        
        // Draw input box
        drawInputBox();
        
        XFlush(display);
    }
    
    void handleKeyPress(XKeyEvent& event) {
        char buffer[32];
        KeySym keysym;
        XLookupString(&event, buffer, sizeof(buffer), &keysym, NULL);
        
        if (inputActive) {
            if (keysym == XK_Return) {
                // Evaluate function
                inputActive = false;
                setFunction(std::string(inputBuffer));
                redraw();
                return;
            } else if (keysym == XK_BackSpace) {
                if (cursorPos > 0) {
                    // Remove character before cursor
                    for (int i = cursorPos - 1; i < (int)strlen(inputBuffer) - 1; i++) {
                        inputBuffer[i] = inputBuffer[i + 1];
                    }
                    inputBuffer[strlen(inputBuffer) - 1] = '\0';
                    cursorPos--;
                }
            } else if (keysym == XK_Left) {
                if (cursorPos > 0) cursorPos--;
            } else if (keysym == XK_Right) {
                if (cursorPos < (int)strlen(inputBuffer)) cursorPos++;
            } else if (keysym == XK_Home) {
                cursorPos = 0;
            } else if (keysym == XK_End) {
                cursorPos = strlen(inputBuffer);
            } else if (keysym == XK_Delete) {
                if (cursorPos < (int)strlen(inputBuffer)) {
                    for (int i = cursorPos; i < (int)strlen(inputBuffer); i++) {
                        inputBuffer[i] = inputBuffer[i + 1];
                    }
                }
            } else if (isprint(buffer[0])) {
                // Insert character at cursor position
                if (strlen(inputBuffer) < 255) {
                    for (int i = strlen(inputBuffer) + 1; i > cursorPos; i--) {
                        inputBuffer[i] = inputBuffer[i - 1];
                    }
                    inputBuffer[cursorPos] = buffer[0];
                    cursorPos++;
                }
            }
            
            redraw();
        }
    }
    
    void handleButtonPress(XButtonEvent& event) {
        int inputX = 20;
        int inputY = height - 50;
        
        if (event.x >= inputX && event.x <= inputX + INPUT_WIDTH &&
            event.y >= inputY && event.y <= inputY + INPUT_HEIGHT) {
            inputActive = true;
            redraw();
        } else {
            inputActive = false;
            redraw();
        }
    }
    
    void run() {
        XEvent event;
        bool running = true;
        
        while (running) {
            XNextEvent(display, &event);
            
            switch (event.type) {
                case Expose:
                    redraw();
                    break;
                    
                case KeyPress:
                    handleKeyPress(event.xkey);
                    break;
                    
                case ButtonPress:
                    handleButtonPress(event.xbutton);
                    break;
                    
                case ConfigureNotify:
                    width = event.xconfigure.width;
                    height = event.xconfigure.height;
                    redraw();
                    break;
                    
                case FocusIn:
                    break;
                    
                case FocusOut:
                    break;
                    
                default:
                    break;
            }
        }
    }
};

int main() {
    FunctionPlotter plotter(800, 600);
    plotter.run();
    return 0;
}
