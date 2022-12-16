#pragma once

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "literals.hpp"

/**
 * @brief A primitve square matrix class, does not implement bound checking.
 *
 * @tparam T contained element data type
 */
template <typename T> class Matrix {
    private:
        std::vector<std::vector<T>> m_Matrix;

    public:
        Matrix() {}
        Matrix(const Matrix &other) : m_Matrix(other.m_Matrix) {}
        Matrix(Matrix &&other) : m_Matrix(other.m_Matrix) { other.m_Matrix.clear(); }
        Matrix(const std::vector<std::vector<T>> &obj) : m_Matrix(obj){};
        Matrix(const std::initializer_list<std::initializer_list<T>> &il) {
            for (const std::initializer_list<T> &il_row : il) {
                std::vector<T> row;

                for (const T &elem : il_row)
                    row.push_back(elem);

                m_Matrix.push_back(row);
            }
        }
        Matrix(T value, size_t dim) {
            for (size_t i = 0; i < dim; i++)
                m_Matrix.push_back(std::vector<T>(dim, value));
        }

        Matrix &operator=(const std::vector<std::vector<T>> &other) {
            m_Matrix = other;
            return *this;
        }

        Matrix &operator=(const std::vector<std::vector<T>> &other) const = delete;
        Matrix &operator=(std::vector<std::vector<T>> other) const = delete;

        std::vector<std::vector<T>> &get() { return m_Matrix; }
        const std::vector<std::vector<T>> &get() const { return m_Matrix; }

        size_t numRows() const { return m_Matrix.size(); }
        size_t numRows() { return m_Matrix.size(); }
        size_t numCols() const { return m_Matrix.at(0).size(); }
        size_t numCols() { return m_Matrix.at(0).size(); }

        void AppendRow(std::vector<T> row) { m_Matrix.push_back(row); }
        void Fill(const T &value) {
            for (std::vector<T> &row : m_Matrix)
                for (T &element : row)
                    element = value;
        }

        std::vector<T> &operator[](size_t idx) {
            if (idx > m_Matrix.size())
                throw(std::out_of_range(std::to_string(idx)));

            return m_Matrix.at(idx);
        }

        const std::vector<T> &operator[](size_t idx) const {
            if (idx > m_Matrix.size())
                throw(std::out_of_range(std::to_string(idx)));

            return m_Matrix.at(idx);
        }

        // https://stackoverflow.com/a/6969904
        class Proxy {
            private:
                std::vector<T> &_arr;

            public:
                Proxy(std::vector<T> &arr) : _arr(arr) {}
                T &operator[](size_t idx) {
                    if (idx <= _arr.size())
                        return _arr[idx];

                    else
                        throw(std::out_of_range(std::to_string(idx)));
                }
        };

        std::vector<T> &At(size_t idx) { return m_Matrix.at(idx); }
        const std::vector<T> &At(size_t idx) const { return m_Matrix.at(idx); }

        Proxy operator[](int idx) {
            try {
                return Proxy(m_Matrix.at(idx));
            } catch (const std::out_of_range &e) {
                std::cout << e.what() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix &obj) {
            for (const auto &row : obj.get()) {
                for (const T &elem : row)
                    os << std::setw(5) << std::left << elem << " ";

                os << std::endl;
            }

            return os << '\b';
        }
};