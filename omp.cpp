#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <omp.h>
using namespace std;

// Function to print a matrix
void printMatrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to multiply two matrices
void multiplyMatrices(double** X, double** Y, double** Z, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double sum = 0;
            for (int k = 0; k < size; k++) {
                sum += X[i][k] * Y[k][j];
            }
            Z[i][j] = sum;
        }
    }
}

int main() {
    int matrixSize = 1000;

    // Allocate memory for matrices
    double** matrix1 = new double*[matrixSize];
    double** matrix2 = new double*[matrixSize];
    double** resultMatrix = new double*[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        matrix1[i] = new double[matrixSize];
        matrix2[i] = new double[matrixSize];
        resultMatrix[i] = new double[matrixSize];
    }

    // Initialize matrices with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10, 10);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            matrix1[i][j] = dis(gen);
            matrix2[i][j] = dis(gen);
        }
    }

    // Multiply matrices and measure execution time
    auto start = chrono::high_resolution_clock::now();
    multiplyMatrices(matrix1, matrix2, resultMatrix, matrixSize);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    // Write output to file
    ofstream output("output.txt");
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            output << resultMatrix[i][j] << " ";
        }
        output << endl;
    }
    output.close();

    // Free memory
    for (int i = 0; i < matrixSize; i++) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] resultMatrix[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] resultMatrix;

    return 0;
}
