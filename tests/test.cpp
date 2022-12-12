#include "./../incl/Matrix.hpp"
#include <iostream>

int main() {
    Matrix<int> mat(5, 3);
    std::cout << mat << std::endl;
    return EXIT_SUCCESS;
}