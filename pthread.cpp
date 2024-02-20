#include <iostream>
#include <fstream>
#include <pthread.h>
#include <random>
#include <chrono>
using namespace std;

const int MATRIX_SIZE = 1000;
const int BLOCK_SIZE = 50;
const int NUM_THREADS = (MATRIX_SIZE * MATRIX_SIZE) / (BLOCK_SIZE * BLOCK_SIZE);

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];
int currentBlock = 0;
pthread_mutex_t mutexCurrentBlock;

void initializeMatrix(int mat[][MATRIX_SIZE]) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 10);
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            mat[i][j] = dist(gen);
        }
    }
}

void *matrixMultiply(void *arg) {
    int blockStartRow, blockStartCol, blockEndRow, blockEndCol;

    while (true) {
        pthread_mutex_lock(&mutexCurrentBlock);
        if (currentBlock >= NUM_THREADS) {
            pthread_mutex_unlock(&mutexCurrentBlock);
            break;
        }
        blockStartRow = (currentBlock / (MATRIX_SIZE / BLOCK_SIZE)) * BLOCK_SIZE;
        blockStartCol = (currentBlock % (MATRIX_SIZE / BLOCK_SIZE)) * BLOCK_SIZE;
        blockEndRow = blockStartRow + BLOCK_SIZE;
        blockEndCol = blockStartCol + BLOCK_SIZE;
        currentBlock++;
        pthread_mutex_unlock(&mutexCurrentBlock);

        for (int i = blockStartRow; i < blockEndRow; ++i) {
            for (int j = blockStartCol; j < blockEndCol; ++j) {
                resultMatrix[i][j] = 0;
                for (int k = 0; k < MATRIX_SIZE; ++k) {
                    resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
    }

    return NULL;
}

int main() {
    initializeMatrix(matrixA);
    initializeMatrix(matrixB);

    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, matrixMultiply, (void *)&threadIds[i]);
    }

    auto start_time = chrono::high_resolution_clock::now(); // Use chrono::high_resolution_clock

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = chrono::high_resolution_clock::now(); // Use chrono::high_resolution_clock
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time); // Use chrono::microseconds

    // Write the output matrix to a file (modify filename as needed)
    ofstream outFile("matrix_multiplication_output.txt");
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            outFile << resultMatrix[i][j] << " ";
        }
        outFile << "\n";
    }
    outFile.close();

    cout << "Execution time: " << duration.count() << " microseconds" << endl;
    return 0;
}
