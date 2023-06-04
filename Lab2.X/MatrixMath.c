#include "MatrixMath.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>

void MatrixPrint(float mat[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f %c", mat[i][j], ' ');
        }
        printf("\n");
    }
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mat1[i][j] != mat2[i][j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] * mat2[j][i];
        }
    }
    
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] + x;
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] * x;
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    return mat[0][0] * mat[1][1] * mat[2][2];
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[j][i] = mat[i][j];
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int curRow = 0;
    for (int r = 0; r < 3; r++) {
        if (r != i) {
            int curCol = 0;
            for (int c = 0; c < 3; c++) {
                if (c != j) {
                    result[curRow][curCol] = mat[r][c];
                    curCol++;
                }
            }
            curRow++;
        }
    }
}

float MatrixDeterminant2x2(float mat[2][2]) {
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

float MatrixDeterminant(float mat[3][3]) {
    float res[2][2];
    MatrixSubmatrix(0, 0, mat, res);
    float a = mat[0][0] * MatrixDeterminant2x2(res);
    MatrixSubmatrix(0, 1, mat, res);
    float b = mat[0][1] * MatrixDeterminant2x2(res);
    MatrixSubmatrix(0, 2, mat, res);
    float c = mat[0][2] * MatrixDeterminant2x2(res);
    return a - b + c;
}