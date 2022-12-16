#pragma once

#include "literals.hpp"
#include <concepts>
#include <iostream>
#include <variant>

//template <typename Weight>
//concept Arithmetic = std::is_arithmetic_v<Weight>;

template <typename Type, typename Weight> class Relation {
    private:
        Type m_From;
        Type m_To;
        Weight m_Weight;

    public:
        Relation() {
            m_From = Type();
            m_Weight = 0;
        }

        Relation(const Type &from, const Type &to, const Weight &weight)
            : m_From(from), m_To(to), m_Weight(weight) {}

        Relation(const Relation &other) {
            m_From = other.m_From;
            m_To = other.m_To;
            m_Weight = other.m_Weight;
        }

        Relation(Relation &&other) {
            m_From = other.m_From;
            m_To = other.m_To;
            m_Weight = other.m_Weight;

            other.m_To = other.m_From = (Type)0;
            other.m_Weight = (Weight)0;
        }

        inline void set(const Type &from, const Type &to, const Weight &weight) {
            m_From = from;
            m_To = to;
            m_Weight = weight;
        }

        Type &from() { return m_From; }
        Type &to() { return m_To; }
        Weight &weight() { return m_Weight; }

        const Type from() const { return m_From; }
        const Type to() const { return m_To; }
        const Weight weight() const { return m_Weight; }

        bool operator<(const Relation &other) const { return m_Weight < other.m_Weight; }
        bool operator<(const Relation &other) { return m_Weight < other.m_Weight; }

        bool operator==(const Relation &other) const { return m_Weight == other.m_Weight; }
        bool operator==(const Relation &other) { return m_Weight == other.m_Weight; }

        friend std::ostream &operator<<(std::ostream &os, const Relation &obj) {
            return os << obj.from() << " --- " << obj.weight() << " ---> " << obj.to();
        }
};