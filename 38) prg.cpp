#include <stdio.h>
#include <stdlib.h>
#define MATRIX_SIZE 3  
int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = 0;
    det = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
        - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
        + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    return det;
}
void inverseMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverse[MATRIX_SIZE][MATRIX_SIZE], int mod) {
    int det = determinant(matrix) % mod;
    if (det < 0) det += mod;
    int invDet = -1;
    for (int i = 0; i < mod; i++) {
        if ((det * i) % mod == 1) {
            invDet = i;
            break;
        }
    }
    if (invDet == -1) {
        printf("Inverse doesn't exist!\n");
        exit(1);
    }
    int temp[MATRIX_SIZE][MATRIX_SIZE];
    temp[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) % mod;
    temp[0][1] = (matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2]) % mod;
    temp[0][2] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]) % mod;
    temp[1][0] = (matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2]) % mod;
    temp[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]) % mod;
    temp[1][2] = (matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2]) % mod;
    temp[2][0] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]) % mod;
    temp[2][1] = (matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1]) % mod;
    temp[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % mod;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            inverse[i][j] = (invDet * temp[i][j]) % mod;
            if (inverse[i][j] < 0) inverse[i][j] += mod;
        }
    }
}
void matrixVectorMultiply(int matrix[MATRIX_SIZE][MATRIX_SIZE], int vector[MATRIX_SIZE], int result[MATRIX_SIZE], int mod) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] = result[i] % mod;
        if (result[i] < 0) result[i] += mod;
    }
}
void decrypt(int key[MATRIX_SIZE][MATRIX_SIZE], int ciphertext[MATRIX_SIZE], int mod) {
    int inverse[MATRIX_SIZE][MATRIX_SIZE];
    inverseMatrix(key, inverse, mod);
    int decrypted[MATRIX_SIZE];
    matrixVectorMultiply(inverse, ciphertext, decrypted, mod);
    printf("Decrypted text: ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%c", decrypted[i] + 'A');
    }
    printf("\n");
}
int main() {
    int mod = 26;
    int key[MATRIX_SIZE][MATRIX_SIZE] = {
        { 6, 24, 1 },
        { 13, 16, 10 },
        { 20, 17, 15 }
    };
    int ciphertext[MATRIX_SIZE] = {15, 14, 7};
    decrypt(key, ciphertext, mod);
    return 0;
}

