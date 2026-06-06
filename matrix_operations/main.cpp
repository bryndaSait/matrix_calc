#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;

class Matrix {
private:
    vector<vector<double>> data;
    int rows, cols;

public:
    // Конструктор
    Matrix(int r, int c) : rows(r), cols(c) {
        data.resize(rows, vector<double>(cols, 0.0));
    }

    // Конструктор копирования
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols), data(other.data) {}

    // Ввод матрицы
    void inputMatrix() {
        cout << "\nВведите элементы матрицы " << rows << "x" << cols << ":\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
                cin >> data[i][j];
            }
        }
    }

    // Вывод матрицы
    void printMatrix(const string& title) const {
        cout << "\n" << title << ":\n";
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << setw(10) << fixed << setprecision(4) << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Сложение двух строк: строка result = строка row1 + строка row2
    vector<double> addRows(int row1, int row2) const {
        checkRowIndex(row1);
        checkRowIndex(row2);

        vector<double> result(cols);
        for (int j = 0; j < cols; j++) {
            result[j] = data[row1][j] + data[row2][j];
        }
        return result;
    }

    // Сложение строки с другой строкой и сохранение результата в первую строку
    void addRowToRow(int targetRow, int sourceRow) {
        checkRowIndex(targetRow);
        checkRowIndex(sourceRow);

        for (int j = 0; j < cols; j++) {
            data[targetRow][j] += data[sourceRow][j];
        }
    }

    // Сложение строки с умноженной на коэффициент другой строкой
    void addScaledRowToRow(int targetRow, int sourceRow, double scalar) {
        checkRowIndex(targetRow);
        checkRowIndex(sourceRow);

        for (int j = 0; j < cols; j++) {
            data[targetRow][j] += scalar * data[sourceRow][j];
        }
    }

    // Сложение всех строк матрицы
    vector<double> sumOfAllRows() const {
        vector<double> result(cols, 0.0);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[j] += data[i][j];
            }
        }
        return result;
    }

    // Сумма нескольких строк по их индексам
    vector<double> sumOfSelectedRows(const vector<int>& rowIndices) const {
        vector<double> result(cols, 0.0);
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
            cout << setw(8) << fixed << setprecision(4) << data[rowIndex][j] << " ";
        }
        cout << endl;
    }

    // Вывод вектора (результата сложения строк)
    void printVector(const vector<double>& vec, const string& name) const {
        cout << "\n" << name << ": ";
        for (double val : vec) {
            cout << setw(8) << fixed << setprecision(4) << val << " ";
        }
        cout << endl;
    }

    // Умножение строки на число
    void multiplyRow(int rowIndex, double scalar) {
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

    // Замена строки результатом сложения двух других строк
    void setRowAsSum(int targetRow, int row1, int row2) {
        vector<double> sum = addRows(row1, row2);
        for (int j = 0; j < cols; j++) {
            data[targetRow][j] = sum[j];
        }
    }

private:
    void checkRowIndex(int row) const {
        if (row < 0 || row >= rows) {
            throw out_of_range("Ошибка: индекс строки вне диапазона!");
        }
    }
};

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

    Matrix matrix(rows, cols);
    matrix.inputMatrix();
    matrix.printMatrix("Исходная матрица");

    int choice;
    do {
        cout << "\n========== МЕНЮ ОПЕРАЦИЙ СО СТРОКАМИ ==========\n";
        cout << "1. Сложить две строки и показать результат\n";
        cout << "2. Прибавить одну строку к другой\n";
        cout << "3. Прибавить строку, умноженную на число, к другой строке\n";
        cout << "4. Сложить все строки матрицы\n";
        cout << "5. Сложить выбранные строки\n";
        cout << "6. Заменить строку суммой двух других строк\n";
        cout << "7. Вывести конкретную строку\n";
        cout << "8. Умножить строку на число\n";
        cout << "9. Вычесть одну строку из другой\n";
        cout << "10. Показать текущую матрицу\n";
        cout << "0. Выход\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        try {
            switch (choice) {
            case 1: {
                int r1, r2;
                cout << "Введите индексы строк (1-" << rows << "): ";
                cin >> r1 >> r2;
                vector<double> result = matrix.addRows(r1 - 1, r2 - 1);
                matrix.printVector(result, string("Результат сложения строк " + r1  + r2));
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
                cout << "Введите множитель: ";
                cin >> scalar;
                matrix.addScaledRowToRow(target - 1, source - 1, scalar);
                matrix.printMatrix("Матрица после операции");
                break;
            }
            case 4: {
                vector<double> sum = matrix.sumOfAllRows();
                matrix.printVector(sum, "Сумма всех строк");
                break;
            }
            case 5: {
                int k;
                cout << "Сколько строк сложить: ";
                cin >> k;
                vector<int> indices(k);
                cout << "Введите индексы строк (1-" << rows << "): ";
                for (int i = 0; i < k; i++) {
                    cin >> indices[i];
                    indices[i]--; // переводим в 0-индексацию
                }
                vector<double> sum = matrix.sumOfSelectedRows(indices);
                matrix.printVector(sum, "Сумма выбранных строк");
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
                cout << "Введите множитель: ";
                cin >> scalar;
                matrix.multiplyRow(r - 1, scalar);
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
            case 0:
                cout << "Выход из программы.\n";
                break;
            default:
                cout << "Неверный выбор! Попробуйте снова.\n";
            }
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

    } while (choice != 0);

    return 0;
}
