#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Класс для работы с матрицами
class Matrix {
private:
    vector<vector<double>> data;
    int n; // Размер матрицы (n x n)

public:
    // Конструктор
    Matrix(int size) : n(size) {
        data.resize(n, vector<double>(n, 0.0));
    }

    // Ввод данных матрицы
    void inputMatrix() {
        cout << "\nВведите элементы матрицы " << n << "x" << n << ":\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
                cin >> data[i][j];
            }
        }
    }

    // Вывод матрицы
    void printMatrix(const string& title) const {
        cout << "\n" << title << ":\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << setw(10) << fixed << setprecision(4) << data[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Вычисление детерминанта (рекурсивный метод)
    double determinant() const {
        return determinantRecursive(data, n);
    }

    // Вывод обратной матрицы
    Matrix inverse() const {
        double det = determinant();
        if (fabs(det) < 1e-10) {
            throw runtime_error("Матрица вырождена! Обратной матрицы не существует.");
        }

        Matrix result(n);
        
        // Создаем расширенную матрицу [A|I]
        vector<vector<double>> augmented(n, vector<double>(2 * n, 0.0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                augmented[i][j] = data[i][j];
            }
            augmented[i][n + i] = 1.0; // Единичная матрица
        }

        // Метод Гаусса-Жордана
        for (int i = 0; i < n; i++) {
            // Поиск главного элемента
            int maxRow = i;
            for (int k = i + 1; k < n; k++) {
                if (fabs(augmented[k][i]) > fabs(augmented[maxRow][i])) {
                    maxRow = k;
                }
            }
            
            // Перестановка строк
            swap(augmented[i], augmented[maxRow]);
            
            // Нормализация строки
            double pivot = augmented[i][i];
            if (fabs(pivot) < 1e-10) continue;
            
            for (int j = 0; j < 2 * n; j++) {
                augmented[i][j] /= pivot;
            }
            
            // Исключение в других строках
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    double factor = augmented[k][i];
                    for (int j = 0; j < 2 * n; j++) {
                        augmented[k][j] -= factor * augmented[i][j];
                    }
                }
            }
        }
        
        // Извлечение обратной матрицы
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                result.data[i][j] = augmented[i][n + j];
            }
        }
        
        return result;
    }

    // Приведение к ступенчатому виду
    Matrix rowEchelonForm() const {
        Matrix result = *this;
        vector<vector<double>>& mat = result.data;
        
        int row = 0;
        for (int col = 0; col < n && row < n; col++) {
            // Поиск строки с ненулевым элементом в текущем столбце
            int maxRow = row;
            for (int i = row; i < n; i++) {
                if (fabs(mat[i][col]) > fabs(mat[maxRow][col])) {
                    maxRow = i;
                }
            }
            
            if (fabs(mat[maxRow][col]) < 1e-10) {
                continue; // Пропускаем столбец, если все элементы нулевые
            }
            
            // Меняем строки местами
            swap(mat[row], mat[maxRow]);
            
            // Нормализуем ведущую строку (делаем ведущий элемент равным 1)
            double pivot = mat[row][col];
            for (int j = col; j < n; j++) {
                mat[row][j] /= pivot;
            }
            
            // Обнуляем элементы ниже ведущего
            for (int i = row + 1; i < n; i++) {
                double factor = mat[i][col];
                for (int j = col; j < n; j++) {
                    mat[i][j] -= factor * mat[row][j];
                }
            }
            
            row++;
        }
        
        return result;
    }

private:
    // Рекурсивное вычисление детерминанта
    double determinantRecursive(vector<vector<double>> matrix, int size) const {
        double det = 0;
        
        if (size == 1) {
            return matrix[0][0];
        }
        
        if (size == 2) {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }
        
        // Для матриц больше 2x2 используем разложение по первой строке
        for (int i = 0; i < size; i++) {
            vector<vector<double>> submatrix(size - 1, vector<double>(size - 1));
            
            // Создаем подматрицу
            for (int j = 1; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    if (k < i) {
                        submatrix[j - 1][k] = matrix[j][k];
                    } else if (k > i) {
                        submatrix[j - 1][k - 1] = matrix[j][k];
                    }
                }
            }
            
            det += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * determinantRecursive(submatrix, size - 1);
        }
        
        return det;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    int n;
    cout << "Введите размерность матрицы (n x n): ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Ошибка: размер матрицы должен быть положительным числом!\n";
        return 1;
    }
    
    Matrix matrix(n);
    
    // 1. Ввод данных матрицы
    matrix.inputMatrix();
    
    // 2. Вывод исходной матрицы
    matrix.printMatrix("Исходная матрица");
    
    // 3. Вывод детерминанта матрицы
    try {
        double det = matrix.determinant();
        cout << "\nДетерминант матрицы: " << fixed << setprecision(6) << det << endl;
        
        // 4. Вывод обратной матрицы
        Matrix inverseMatrix = matrix.inverse();
        inverseMatrix.printMatrix("Обратная матрица");
        
    } catch (const exception& e) {
        cout << "\nОшибка: " << e.what() << endl;
    }
    
    // 5. Вывод приведенной к ступенчатому виду матрицы
    Matrix echelonMatrix = matrix.rowEchelonForm();
    echelonMatrix.printMatrix("Матрица в ступенчатом виде");
    
    return 0;
}