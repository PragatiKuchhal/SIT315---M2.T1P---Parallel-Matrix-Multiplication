#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
using namespace std;

// Function to multiply two matrices
void multiplyMatrices(double** A, double** B, double** C, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double sum = 0;
            for (int k = 0; k < size; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    int matrixSize = 1000; // Adjust matrix size as needed

    // Allocate memory for matrices
    double** matrixA = new double*[matrixSize];
    double** matrixB = new double*[matrixSize];
    double** resultMatrix = new double*[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        matrixA[i] = new double[matrixSize];
        matrixB[i] = new double[matrixSize];
        resultMatrix[i] = new double[matrixSize];
    }

    // Initialize matrices with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10, 10);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            matrixA[i][j] = dis(gen);
            matrixB[i][j] = dis(gen);
        }
    }

    // Multiply matrices and measure execution time
    auto startTime = chrono::high_resolution_clock::now();
    multiplyMatrices(matrixA, matrixB, resultMatrix, matrixSize);
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    // Write output to a file (modify filename as needed)
    ofstream outputFile("matrix_multiplication_output.txt");
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            outputFile << resultMatrix[i][j] << " ";
        }
        outputFile << endl;
    }
    outputFile.close();

    // Free memory
    for (int i = 0; i < matrixSize; i++) {
        delete[] matrixA[i];
        delete[] matrixB[i];
        delete[] resultMatrix[i];
    }
    delete[] matrixA;
    delete[] matrixB;
    delete[] resultMatrix;

    return 0;
}
