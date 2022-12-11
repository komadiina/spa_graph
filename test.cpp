#include "./incl/Matrix.hpp"
#include <iostream>

int main() {
    Matrix<int> mat = {
        {1,  2,  3,  4 },
        {5,  6,  7,  8 },
        {9,  10, 11, 12},
        {13, 14, 15, 16}
    };

    for (size_t i = 0; i < mat.numRows(); i++)
        for (size_t j = 0; j < mat.numCols(); j++)
            printf("mat[%d][%d] = %d\n", i, j, mat[i][j]);
}