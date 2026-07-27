# USAGE
## WRITTEN BY AI BE CAREFULL!

## Overview
This is a **C++ GUI application** that plots mathematical functions in real-time. It runs on Linux using the X11 windowing system and uses only standard libraries (no external GUI frameworks).

## What the Program Does

### 1. **Interactive Function Plotting**
- Plots any mathematical function entered by the user
- Displays the graph in a resizable window with axes and grid lines
- Supports a wide range of mathematical functions

### 2. **Supported Functions**
The program can evaluate and plot expressions containing:
- **Basic operations**: `+`, `-`, `*`, `/`
- **Parentheses**: `( )` for grouping
- **Trigonometric**: `sin(x)`, `cos(x)`, `tan(x)`, `asin(x)`, `acos(x)`, `atan(x)`
- **Hyperbolic**: `sinh(x)`, `cosh(x)`, `tanh(x)`
- **Exponential/Log**: `exp(x)`, `log(x)`, `log10(x)`
- **Other**: `sqrt(x)`, `abs(x)`, `fabs(x)`, `floor(x)`, `ceil(x)`
- **Constants**: `pi` (π), `e` (Euler's number)

### 3. **User Interface Elements**
- **Input box**: Type your function at the bottom
- **"Function: f(x)="** label shows the input area
- **Cursor support**: Click in the input box to edit, use arrow keys to navigate
- **Real-time updating**: Press Enter to plot your function
- **Display area**: Shows the graph with labeled axes
- **Range information**: Shows current x and y axis ranges

### 4. **Graph Features**
- **Grid lines**: Light gray grid for better readability
- **Axes**: Black lines with arrow tips at the ends
- **Axis labels**: Numbers on both axes for reference
- **Origin label**: "0" at the center
- **Function curve**: Plotted in red with 2-pixel width
- **Auto-scaling**: Y-axis range adjusts based on function values

### 5. **Example Functions You Can Plot**
```
sin(x)           - Sine wave
cos(x)           - Cosine wave  
tan(x)           - Tangent function
x*x              - Parabola
2*x+3            - Linear function
sin(x)*cos(x)    - Product of sine and cosine
x*x - 4*x + 3    - Quadratic with roots at 1 and 3
exp(x)           - Exponential growth
log(x)           - Natural logarithm (x > 0)
sqrt(x)          - Square root (x ≥ 0)
sin(x)/x         - Sinc function
abs(sin(x))      - Absolute sine
pow(x,2)         - Power function (x²)
```

### 6. **Technical Implementation**
- **GUI Library**: X11 (standard on Linux systems)
- **Language**: C++ with Standard Library only
- **Graphics**: Direct X11 drawing (lines, rectangles, text)
- **Parsing**: Custom recursive descent expression evaluator
- **Rendering**: 1000-point sampling for smooth curves
- **Event-driven**: Handles keyboard, mouse, and window events

### 7. **User Interaction Workflow**
1. **Launch the program** - Window appears with default `sin(x)` function
2. **Click the input box** at the bottom to activate it (cursor appears)
3. **Type your function** (e.g., "x*x + 2*x + 1")
4. **Press Enter** to evaluate and plot
5. **View the graph** - Function appears in red on the grid
6. **Resize the window** - Graph automatically redraws
7. **Click outside** the input box to deactivate it

### 8. **Key Features**
- **No external dependencies** - Uses only X11 and standard C++ libraries
- **Real-time evaluation** - Immediate graph updates
- **Interactive text input** - Full cursor support (Home, End, Delete, arrow keys)
- **Window resizing** - Responsive to window changes
- **Error handling** - Invalid functions show blank graph area

### 9. **What It Doesn't Do (Limitations)**
- No zoom/pan functionality (though range is shown)
- No saving/exporting graphs
- No multiple function plotting
- No color customization
- No advanced formatting or styling
- No mouse interaction with the graph itself

### 10. **Use Cases**
- **Educational**: Visualize mathematical concepts
- **Engineering**: Quick function visualization
- **Debugging**: Check function behavior
- **Exploration**: Experiment with different mathematical expressions

## How to Compile and Run

```bash
# Compile
g++ -o function_plotter main.cpp -lX11 -lm

# Run
./function_plotter
```

## Screenshot Description (Text)
The window has:
- **Title bar**: "Function Plotter"
- **Main area**: White background with gray grid, black axes, red function curve
- **Bottom area**: Input box with "Function: f(x)=" label and text entry
- **Right of input**: Shows current x and y range (e.g., "x: [-10.0, 10.0] y: [-10.0, 10.0]")

## Example Session
1. User runs program → sees sin(x) plotted
2. User clicks input box → cursor appears
3. User types `x*x` → presses Enter
4. Graph updates to show parabola (U-shape)
5. User types `sin(x)*cos(x)` → presses Enter  
6. Graph updates to show combined wave
7. User resizes window → graph adjusts to new size

## Conclusion
This program provides a simple, self-contained tool for visualizing mathematical functions on Linux systems. It's ideal for quick experimentation and educational purposes, with a clean interface and support for common mathematical operations.
