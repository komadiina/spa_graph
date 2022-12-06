#pragma once

#include "Node.hpp"
#include "Relation.hpp"
#include "literals.hpp"
#include <initializer_list>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief A user-defined directed graph class, implementing a hashset for node and a hashmap with Node<T>-std::list
 * key-values.
 *
 * @tparam T Specifies a template for the type of contained data within the nodes.
 */
template <typename T> class Graph {
	private:
		using NodeList = std::list<std::pair<Node<T>, NodeWeight>>;

	private:
		std::unordered_set<Node<T>, NodeHash<T>> m_Nodes;
		std::unordered_map<Node<T>, NodeList, NodeHash<T>> m_Connectivity;
		NodeWeight m_TotalWeight = 0.0;

	public:
		Graph() {
			m_Nodes = std::unordered_set<Node<T>>();
			m_Connectivity = std::unordered_map<Node<T>, NodeList, NodeHash<T>>();
		}

		// initialize a graph with specified nodes
		Graph(std::initializer_list<std::pair<T, uint8_t>> nodes) {
			for (auto [nodeData, nodeId] : nodes) {
				Node<T> node(nodeData, nodeId);
				m_Nodes.emplace(node);
				m_Connectivity[node];
			}
		}

		decltype(m_Nodes) &GetNodes() { return m_Nodes; }
		const decltype(m_Nodes) &GetNodes() const { return m_Nodes; }

		const size_t NodeCount() const { return m_Nodes.size(); }
		size_t NodeCount() { return m_Nodes.size(); }

		void PrintConnections() const {
			for (auto [k, v] : m_Connectivity) {
				// stao ovdje
			}
		}

		/**
		 * @brief Insert multiple nodes into the graph and connect them, given the weights. If a given node is not found
		 * in the map as a key, logs to the console and continues execution, skipping given (source) node. Guarantees a
		 * single connected-component graph.
		 * @param relations std::initializer_list type containing given relations
		 */
		void TryConnect(std::initializer_list<const Relation<Node<T>, NodeWeight>> relations) {
			for (Relation<Node<T>, NodeWeight> relation : relations) {
				try {
					// auto src = relation.template get<0>();
					// auto dest = relation.template get<1>();
					// auto wt = relation.template get<2>();
					Node<T> src = relation.from();
					Node<T> dest = relation.to();
					NodeWeight wt = relation.weight();

					m_Connectivity.at(src);

					m_Nodes.emplace(src);
					m_Nodes.emplace(dest);
					m_Connectivity[src].push_back({dest, wt});

				} catch (const std::out_of_range &) {
					std::cout << "Node " << relation.template get<0>() << " not found in map.\n";
					continue;
				}
			}
		}

		/**
		 * @brief Forces the insertion of given nodes into hashset and hashmap. As a result, the given node will always
		 * be contained in the graph. Does not guarantee a single connected-component graph.
		 * @param relations std::initializer_list type containing given relations
		 */
		void Connect(std::initializer_list<Relation<Node<T>, NodeWeight>> relations) {
			for (Relation<Node<T>, NodeWeight> relation : relations) {
				auto src = relation.template get<0>();
				auto dest = relation.template get<1>();
				auto wt = relation.template get<2>();

				m_Nodes.emplace(src);
				m_Nodes.emplace(dest);
				m_Connectivity[src].push_back({dest, wt});
			}
		}

		friend std::ostream &operator<<(std::ostream &os, const Graph<T> &obj) {
			std::unordered_set<Node<T>, NodeHash<T>> nodes = obj.GetNodes();
			os << "{\n";

			for (Node<T> node : nodes)
				os << "  {\n    Node: " << node.GetData() << "\n    ID: " << (int)node.GetId() << "\n  },\n";
			return os << "}.";
		}
};