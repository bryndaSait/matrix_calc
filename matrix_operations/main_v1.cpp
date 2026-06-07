#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <sstream>

using namespace std;

// Класс для работы с рациональными числами (дробями)
class Rational {
private:
    long long numerator;
    long long denominator;
    
    // Нахождение НОД для сокращения дроби
    long long gcd(long long a, long long b) const {
        a = abs(a);
        b = abs(b);
        while (b != 0) {
            long long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }
    
    // Сокращение дроби
    void reduce() {
        if (denominator == 0) {
            throw runtime_error("Знаменатель не может быть нулем!");
        }
        
        if (numerator == 0) {
            denominator = 1;
            return;
        }
        
        long long g = gcd(numerator, denominator);
        numerator /= g;
        denominator /= g;
        
        // Знак храним в числителе
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }
    
public:
    // Конструкторы
    Rational() : numerator(0), denominator(1) {}
    
    Rational(long long num) : numerator(num), denominator(1) {}
    
    Rational(long long num, long long den) : numerator(num), denominator(den) {
        if (den == 0) throw runtime_error("Знаменатель не может быть нулем!");
        reduce();
    }
    
    Rational(double value) {
        // Преобразование double в дробь с точностью 1e-9
        const long long precision = 1000000000;
        numerator = static_cast<long long>(value * precision + 0.5);
        denominator = precision;
        reduce();
    }
    
    // Арифметические операции
    Rational operator+(const Rational& other) const {
        return Rational(
            numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator
        );
    }
    
    Rational operator-(const Rational& other) const {
        return Rational(
            numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator
        );
    }
    
    Rational operator*(const Rational& other) const {
        return Rational(
            numerator * other.numerator,
            denominator * other.denominator
        );
    }
    
    Rational operator/(const Rational& other) const {
        return Rational(
            numerator * other.denominator,
            denominator * other.numerator
        );
    }
    
    Rational& operator+=(const Rational& other) {
        *this = *this + other;
        return *this;
    }
    
    Rational& operator-=(const Rational& other) {
        *this = *this - other;
        return *this;
    }
    
    Rational& operator*=(const Rational& other) {
        *this = *this * other;
        return *this;
    }
    
    Rational& operator/=(const Rational& other) {
        *this = *this / other;
        return *this;
    }
    
    // Унарный минус
    Rational operator-() const {
        return Rational(-numerator, denominator);
    }
    
    // Сравнение
    bool operator==(const Rational& other) const {
        return numerator == other.numerator && denominator == other.denominator;
    }
    
    bool operator!=(const Rational& other) const {
        return !(*this == other);
    }
    
    // Преобразование в double
    double toDouble() const {
        return static_cast<double>(numerator) / denominator;
    }
    
    // Получение числителя и знаменателя
    long long getNumerator() const { return numerator; }
    long long getDenominator() const { return denominator; }
    
    // Вывод в виде дроби (исправленная версия без to_string)
    string toString() const {
        ostringstream oss;
        if (denominator == 1) {
            oss << numerator;
        } else {
            oss << numerator << "/" << denominator;
        }
        return oss.str();
    }
    
    // Дружественная функция для вывода
    friend ostream& operator<<(ostream& os, const Rational& r) {
        if (r.denominator == 1) {
            os << r.numerator;
        } else {
            os << r.numerator << "/" << r.denominator;
        }
        return os;
    }
};

// Класс для работы с матрицами рациональных чисел
class RationalMatrix {
private:
    vector<vector<Rational>> data;
    int rows, cols;

public:
    // Конструкторы
    RationalMatrix(int r, int c) : rows(r), cols(c) {
        data.resize(rows, vector<Rational>(cols, Rational(0, 1)));
    }
    
    RationalMatrix(const RationalMatrix& other) : rows(other.rows), cols(other.cols), data(other.data) {}
    
    // Ввод матрицы
    void inputMatrix() {
        cout << "\nВведите элементы матрицы " << rows << "x" << cols << ":\n";
        cout << "(Вводите числа в виде целых чисел или десятичных дробей)\n";
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                double value;
                cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
                cin >> value;
                data[i][j] = Rational(value);
            }
        }
    }
    
    // Вывод матрицы в виде дробей
    void printMatrix(const string& title) const {
        cout << "\n" << title << ":\n";
        
        // Находим максимальную ширину для выравнивания
        int maxWidth = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int width = data[i][j].toString().length();
                if (width > maxWidth) maxWidth = width;
            }
        }
        maxWidth += 2;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << setw(maxWidth) << data[i][j];
            }
            cout << endl;
        }
    }
    
    // Сложение двух строк
    vector<Rational> addRows(int row1, int row2) const {
        checkRowIndex(row1);
        checkRowIndex(row2);
        
        vector<Rational> result(cols);
        for (int j = 0; j < cols; j++) {
            result[j] = data[row1][j] + data[row2][j];
        }
        return result;
    }
    
    // Прибавить одну строку к другой
    void addRowToRow(int targetRow, int sourceRow) {
        checkRowIndex(targetRow);
        checkRowIndex(sourceRow);
        
        for (int j = 0; j < cols; j++) {
            data[targetRow][j] += data[sourceRow][j];
        }
    }
    
    // Прибавить строку, умноженную на число, к другой строке
    void addScaledRowToRow(int targetRow, int sourceRow, const Rational& scalar) {
        checkRowIndex(targetRow);
        checkRowIndex(sourceRow);
        
        for (int j = 0; j < cols; j++) {
            data[targetRow][j] += scalar * data[sourceRow][j];
        }
    }
    
    // Сумма всех строк
    vector<Rational> sumOfAllRows() const {
        vector<Rational> result(cols, Rational(0, 1));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[j] += data[i][j];
            }
        }
        return result;
    }
    
    // Сумма выбранных строк
    vector<Rational> sumOfSelectedRows(const vector<int>& rowIndices) const {
        vector<Rational> result(cols, Rational(0, 1));
        for (int idx : rowIndices) {
            checkRowIndex(idx);
            for (int j = 0; j < cols; j++) {
                result[j] += data[idx][j];
            }
        }
        return result;
    }
    
    // Вывод строки
    void printRow(int rowIndex) const {
        checkRowIndex(rowIndex);
        cout << "Строка " << rowIndex + 1 << ": ";
        for (int j = 0; j < cols; j++) {
            cout << data[rowIndex][j] << " ";
        }
        cout << endl;
    }
    
    // Вывод вектора (результата сложения строк)
    void printVector(const vector<Rational>& vec, const string& name) const {
        if (!name.empty()) {
            cout << "\n" << name << ": ";
        }
        for (const Rational& val : vec) {
            cout << val << " ";
        }
        cout << endl;
    }
    
    // Умножение строки на число
    void multiplyRow(int rowIndex, const Rational& scalar) {
        checkRowIndex(rowIndex);
        for (int j = 0; j < cols; j++) {
            data[rowIndex][j] *= scalar;
        }
    }
    
    // Вычитание строк
    void subtractRowFromRow(int targetRow, int sourceRow) {
        checkRowIndex(targetRow);
        checkRowIndex(sourceRow);
        for (int j = 0; j < cols; j++) {
            data[targetRow][j] -= data[sourceRow][j];
        }
    }
    
    // Замена строки суммой двух других
    void setRowAsSum(int targetRow, int row1, int row2) {
        vector<Rational> sum = addRows(row1, row2);
        for (int j = 0; j < cols; j++) {
            data[targetRow][j] = sum[j];
        }
    }
    
    // Дополнительная операция: сложение строки с умноженной на дробь
    void addFractionRowToRow(int targetRow, int sourceRow, long long num, long long den) {
        Rational scalar(num, den);
        addScaledRowToRow(targetRow, sourceRow, scalar);
    }
    
    // Операция сложения строки с целым числом
    void addIntegerToRow(int targetRow, int sourceRow, long long integer) {
        Rational scalar(integer);
        addScaledRowToRow(targetRow, sourceRow, scalar);
    }

private:
    void checkRowIndex(int row) const {
        if (row < 0 || row >= rows) {
            throw out_of_range("Ошибка: индекс строки вне диапазона!");
        }
    }
};

// Функция для вывода разделителя
void printSeparator() {
    cout << "\n========================================\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    int rows, cols;
    cout << "Введите количество строк матрицы: ";
    cin >> rows;
    cout << "Введите количество столбцов матрицы: ";
    cin >> cols;
    
    if (rows <= 0 || cols <= 0) {
        cout << "Ошибка: размеры матрицы должны быть положительными!\n";
        return 1;
    }
    
    RationalMatrix matrix(rows, cols);
    matrix.inputMatrix();
    matrix.printMatrix("Исходная матрица");
    
    int choice;
    do {
        printSeparator();
        cout << "========== МЕНЮ ОПЕРАЦИЙ СО СТРОКАМИ ==========\n";
        cout << "1. Сложить две строки и показать результат (в виде дробей)\n";
        cout << "2. Прибавить одну строку к другой\n";
        cout << "3. Прибавить строку, умноженную на число, к другой строке\n";
        cout << "4. Сложить все строки матрицы\n";
        cout << "5. Сложить выбранные строки\n";
        cout << "6. Заменить строку суммой двух других строк\n";
        cout << "7. Вывести конкретную строку\n";
        cout << "8. Умножить строку на число\n";
        cout << "9. Вычесть одну строку из другой\n";
        cout << "10. Показать текущую матрицу\n";
        cout << "11. Прибавить строку, умноженную на дробь (числитель/знаменатель)\n";
        cout << "12. Прибавить строку, умноженную на целое число\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        
        try {
            switch(choice) {
                case 1: {
                    int r1, r2;
                    cout << "Введите индексы строк (1-" << rows << "): ";
                    cin >> r1 >> r2;
                    if (r1 < 1 || r1 > rows || r2 < 1 || r2 > rows) {
                        cout << "Ошибка: неверные индексы строк!\n";
                        break;
                    }
                    vector<Rational> result = matrix.addRows(r1 - 1, r2 - 1);
                    cout << "\nРезультат сложения строк " << r1 << " и " << r2 << " (в виде дробей):\n";
                    cout << "[ ";
                    for (size_t i = 0; i < result.size(); i++) {
                        cout << result[i];
                        if (i < result.size() - 1) cout << ", ";
                    }
                    cout << " ]" << endl;
                    break;
                }
                case 2: {
                    int target, source;
                    cout << "К какой строке прибавить (1-" << rows << "): ";
                    cin >> target;
                    cout << "Какую строку прибавить (1-" << rows << "): ";
                    cin >> source;
                    matrix.addRowToRow(target - 1, source - 1);
                    matrix.printMatrix("Матрица после сложения строк");
                    break;
                }
                case 3: {
                    int target, source;
                    double scalar;
                    cout << "К какой строке прибавить (1-" << rows << "): ";
                    cin >> target;
                    cout << "Какую строку прибавить (1-" << rows << "): ";
                    cin >> source;
                    cout << "Введите множитель (десятичная дробь): ";
                    cin >> scalar;
                    matrix.addScaledRowToRow(target - 1, source - 1, Rational(scalar));
                    matrix.printMatrix("Матрица после операции");
                    break;
                }
                case 4: {
                    vector<Rational> sum = matrix.sumOfAllRows();
                    cout << "\nСумма всех строк (в виде дробей):\n";
                    cout << "[ ";
                    for (size_t i = 0; i < sum.size(); i++) {
                        cout << sum[i];
                        if (i < sum.size() - 1) cout << ", ";
                    }
                    cout << " ]" << endl;
                    break;
                }
                case 5: {
                    int k;
                    cout << "Сколько строк сложить: ";
                    cin >> k;
                    if (k <= 0 || k > rows) {
                        cout << "Ошибка: неверное количество строк!\n";
                        break;
                    }
                    vector<int> indices(k);
                    cout << "Введите индексы строк (1-" << rows << "): ";
                    bool valid = true;
                    for (int i = 0; i < k; i++) {
                        cin >> indices[i];
                        if (indices[i] < 1 || indices[i] > rows) {
                            valid = false;
                        }
                        indices[i]--; // переводим в 0-индексацию
                    }
                    if (!valid) {
                        cout << "Ошибка: неверные индексы строк!\n";
                        break;
                    }
                    vector<Rational> sum = matrix.sumOfSelectedRows(indices);
                    cout << "\nСумма выбранных строк (в виде дробей):\n";
                    cout << "[ ";
                    for (size_t i = 0; i < sum.size(); i++) {
                        cout << sum[i];
                        if (i < sum.size() - 1) cout << ", ";
                    }
                    cout << " ]" << endl;
                    break;
                }
                case 6: {
                    int target, r1, r2;
                    cout << "Какую строку заменить (1-" << rows << "): ";
                    cin >> target;
                    cout << "Введите первую строку для сложения (1-" << rows << "): ";
                    cin >> r1;
                    cout << "Введите вторую строку для сложения (1-" << rows << "): ";
                    cin >> r2;
                    matrix.setRowAsSum(target - 1, r1 - 1, r2 - 1);
                    matrix.printMatrix("Матрица после замены строки");
                    break;
                }
                case 7: {
                    int r;
                    cout << "Введите номер строки (1-" << rows << "): ";
                    cin >> r;
                    matrix.printRow(r - 1);
                    break;
                }
                case 8: {
                    int r;
                    double scalar;
                    cout << "Введите номер строки (1-" << rows << "): ";
                    cin >> r;
                    cout << "Введите множитель (десятичная дробь): ";
                    cin >> scalar;
                    matrix.multiplyRow(r - 1, Rational(scalar));
                    matrix.printMatrix("Матрица после умножения строки");
                    break;
                }
                case 9: {
                    int target, source;
                    cout << "Из какой строки вычесть (1-" << rows << "): ";
                    cin >> target;
                    cout << "Какую строку вычесть (1-" << rows << "): ";
                    cin >> source;
                    matrix.subtractRowFromRow(target - 1, source - 1);
                    matrix.printMatrix("Матрица после вычитания");
                    break;
                }
                case 10:
                    matrix.printMatrix("Текущая матрица");
                    break;
                case 11: {
                    int target, source;
                    long long num, den;
                    cout << "К какой строке прибавить (1-" << rows << "): ";
                    cin >> target;
                    cout << "Какую строку прибавить (1-" << rows << "): ";
                    cin >> source;
                    cout << "Введите числитель дроби: ";
                    cin >> num;
                    cout << "Введите знаменатель дроби: ";
                    cin >> den;
                    if (den == 0) {
                        cout << "Ошибка: знаменатель не может быть нулем!\n";
                        break;
                    }
                    matrix.addFractionRowToRow(target - 1, source - 1, num, den);
                    matrix.printMatrix("Матрица после операции");
                    break;
                }
                case 12: {
                    int target, source;
                    long long integer;
                    cout << "К какой строке прибавить (1-" << rows << "): ";
                    cin >> target;
                    cout << "Какую строку прибавить (1-" << rows << "): ";
                    cin >> source;
                    cout << "Введите целое число: ";
                    cin >> integer;
                    matrix.addIntegerToRow(target - 1, source - 1, integer);
                    matrix.printMatrix("Матрица после операции");
                    break;
                }
                case 0:
                    cout << "Выход из программы.\n";
                    break;
                default:
                    cout << "Неверный выбор! Попробуйте снова.\n";
            }
        } catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }
        
    } while (choice != 0);
    
    cout << "\nПрограмма завершена. Спасибо за использование!\n";
    return 0;
}
