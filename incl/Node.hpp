#pragma once

#include "literals.hpp"
#include <iostream>
#include <type_traits>
#include <variant>
#include <vector>

template <typename T> class Node {
    private:
        T m_Data;
        uint8_t m_Id;

        int32_t m_DegreeIn;
        int32_t m_DegreeOut;

    public:
        Node() {
            m_Data = T{};
            m_Id = 0;
            m_DegreeIn = m_DegreeOut = 0;
        }

        Node(const T &data, const uint8_t &id) {
            m_Data = data;
            m_Id = id;
            m_DegreeIn = m_DegreeOut = 0;
        }

        Node(const Node &other) {
            m_Data = other.m_Data;
            m_Id = other.m_Id;
            m_DegreeIn = other.m_DegreeIn;
            m_DegreeOut = other.m_DegreeOut;
        }

        Node(Node &&other) {
            m_Data = other.m_Data;
            m_Id = other.m_Id;
            m_DegreeIn = other.m_DegreeIn;
            m_DegreeOut = other.m_DegreeOut;

            other.m_Data = T{};
            other.m_Id = 0;
            other.m_DegreeIn = 0;
            other.m_DegreeOut = 0;
        }

        T GetData() const { return m_Data; }
        T &GetData() { return m_Data; }

        const uint8_t GetId() const { return m_Id; }
        uint8_t &GetId() { return m_Id; }

        const int32_t GetDegreeIn() const { return m_DegreeIn; }
        int32_t &GetDegreeIn() { return m_DegreeIn; }

        const int32_t GetDegreeOut() const { return m_DegreeOut; }
        int32_t &GetDegreeOut() { return m_DegreeOut; }

        void SetData(const T &data) { m_Data = data; }
        void SetDegreeIn(const int32_t &degIn) { m_DegreeIn = degIn; }
        void SetDegreeOut(const int32_t &degOut) { m_DegreeOut = degOut; }

        friend std::ostream &operator<<(std::ostream &os, const Node &obj) {
            return os << obj.m_Data;
        }

        bool operator==(const Node &other) const { return m_Data == other.m_Data; }
        bool operator==(Node other) { return m_Data == other.m_Data; }

        bool operator!=(const Node &other) const { return m_Data != other.m_Data; }
        bool operator!=(Node other) { return m_Data != other.m_Data; }

        void erase() {
            m_Data = T{};
            m_Id = 0;

            m_DegreeIn = m_DegreeOut = 0;
        }

        Node<T> &operator=(const Node<T> &other) {
            m_Data = other.m_Data;
            m_Id = other.m_Id;
            m_DegreeIn = other.m_DegreeIn;
            m_DegreeOut = other.m_DegreeOut;

            return *this;
        }
};

template <typename T> class NodeHash {
    public:
        size_t operator()(const Node<T> &obj) const {
            return (
                std::hash<uint8_t>()(obj.GetId()) ^ std::hash<uint8_t>()(obj.GetId()));
        }
};
