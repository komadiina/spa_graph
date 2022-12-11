#pragma once

#include "literals.hpp"
#include <iostream>
#include <type_traits>
#include <variant>
#include <vector>

template <typename T> class Node {
    private:
        T m_Data;

    public:
        Node() { m_Data = T{}; }

        Node(const T &data, const uint8_t &id) { m_Data = data; }

        Node(const T &data) { m_Data = data; }

        Node(const Node &other) { m_Data = other.m_Data; }

        Node(Node &&other) {
            m_Data = other.m_Data;
            other.m_Data = T{};
        }

        T GetData() const { return m_Data; }
        T &GetData() { return m_Data; }

        void SetData(const T &data) { m_Data = data; }

        friend std::ostream &operator<<(std::ostream &os, const Node &obj) {
            return os << obj.m_Data;
        }

        bool operator==(const Node &other) const { return m_Data == other.m_Data; }
        bool operator==(Node other) { return m_Data == other.m_Data; }

        bool operator!=(const Node &other) const { return m_Data != other.m_Data; }
        bool operator!=(Node other) { return m_Data != other.m_Data; }

        bool operator<(const Node &other) const {
            return sizeof(m_Data) < sizeof(other.m_Data);
        }
        bool operator<(Node other) { return sizeof(m_Data) < sizeof(other.m_Data); }

        void erase() { m_Data = T{}; }

        Node<T> &operator=(const Node<T> &other) {
            m_Data = other.m_Data;

            return *this;
        }
};

template <typename T> class NodeHash {
    public:
        size_t operator()(const Node<T> &obj) const {
            return (std::hash<size_t>()(sizeof(obj.GetData())) ^
                    std::hash<size_t>()(sizeof(obj.GetData())));
        }
};
