#pragma once

#include "literals.hpp"
#include <concepts>
#include <iostream>
#include <variant>

template <typename T, typename... Ts>
std::ostream &operator<<(std::ostream &os, const std::variant<T, Ts...> &obj) {
	std::visit([&os](auto &&arg) { os << arg << " "; }, obj);
	return os << "\b.";
}

template <typename Weight>
concept Arithmetic = std::is_arithmetic_v<Weight>;

template <typename Type, Arithmetic Weight> class Relation {
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

		/***
		 * @brief Returns an element in the given tuple using function template
		 * specialization.
		 * @param <0> source node
		 * @param <1> destination node
		 * @param <2> weight between nodes
		 */
		template <std::size_t elem> auto get() {
			if constexpr (elem == 0)
				return std::variant<Type>(m_From);
			else if constexpr (elem == 1)
				return std::variant<Type>(m_To);
			else if constexpr (elem == 2)
				return std::variant<Weight>(m_Weight);

			throw std::invalid_argument(std::to_string(elem));
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

		friend std::ostream &operator<<(std::ostream &os, const Relation &obj) {
			return os << obj.from() << " --- " << obj.weight() << " ---> " << obj.to();
		}
};