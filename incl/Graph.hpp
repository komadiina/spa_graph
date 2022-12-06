#pragma once

#include "Node.hpp"
#include "Relation.hpp"
#include "literals.hpp"

#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <list>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @brief A user-defined directed graph class, implementing a hashset for node and a
 * hashmap with Node<T>-std::list key-values.
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

        /* used for initializing the relation map */
        std::vector<std::vector<double>> m_AdjMatrix;
        std::vector<std::string> m_NodeNames;

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

        Graph(std::initializer_list<Node<T>> nodes) {
            for (auto node : nodes) {
                m_Nodes.emplace(node);
                m_Connectivity[node];
            }
        }

        decltype(m_Nodes) &GetNodes() { return m_Nodes; }
        const decltype(m_Nodes) &GetNodes() const { return m_Nodes; }

        const size_t NodeCount() const { return m_Nodes.size(); }
        size_t NodeCount() { return m_Nodes.size(); }

        /**
         * @brief Insert multiple nodes into the graph and connect them, given the
         * weights. If a given node is not found in the map as a key, logs to the console
         * and continues execution, skipping given (source) node. Guarantees a single
         * connected-component graph.
         * @param relations std::initializer_list type containing given relations
         */
        void TryConnect(
            std::initializer_list<const Relation<Node<T>, NodeWeight>> relations) {
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
                    std::cout << "Node " << relation.template get<0>()
                              << " not found in map.\n";
                    continue;
                }
            }
        }

        /**
         * @brief Forces the insertion of given nodes into hashset and hashmap. As a
         * result, the given node will always be contained in the graph. Does not
         * guarantee a single connected-component graph.
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

        void TryDisconnect(
            std::initializer_list<Relation<Node<T>, NodeWeight>> relations) {
            for (Relation<Node<T>, NodeWeight> relation : relations) {
                Node<T> key = relation.template get<0>();

                try {
                    std::pair<Node<T>, NodeWeight> value = {relation.template get<1>(),
                                                            relation.template get<2>()};

                    m_Connectivity.at(key).remove_if(
                        [&value](std::pair<Node<T>, NodeWeight> elem) {
                            return elem == value;
                        });
                } catch (const std::out_of_range &) {
                    std::cout << "Node not in map. (" << key << ")\n";
                    continue;
                }
            }
        }

        void TryDisconnect(Relation<Node<T>, NodeWeight> relation) {
            Node<T> key = relation.template get<0>();

            try {
                std::pair<Node<T>, NodeWeight> value = {relation.template get<1>(),
                                                        relation.template get<2>()};
                m_Connectivity.at(key).remove_if(
                    [&value](std::pair<Node<T>, NodeWeight> elem) {
                        return elem == value;
                    });
            } catch (const std::out_of_range &) {
                std::cout << "Node not in map. (" << key << ")\n";
                return;
            }
        }

        void TryDisconnect(Node<T> key, Node<T> target) {
            try {
                m_Connectivity.at(key).remove_if(
                    [&target](std::pair<Node<T>, NodeWeight> elem) {
                        return elem.first == target;
                    });
            } catch (const std::out_of_range &) {
                std::cout << "Node not in map. (" << key << ")\n";
                return;
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const Graph<T> &obj) {
            std::unordered_set<Node<T>, NodeHash<T>> nodes = obj.GetNodes();
            os << "{\n";

            for (Node<T> node : nodes)
                os << "  {\n    Node: " << node.GetData()
                   << "\n    ID: " << (int)node.GetId() << "\n  },\n";
            return os << "}.";
        }

        void PrintConnections() const {
            std::cout << "-----" << std::endl;

            for (auto [dest, srcList] : m_Connectivity) {
                if (srcList.empty()) {
                    std::cout << "[X] " << dest << " does not connect to any node.\n";
                    continue;
                }

                std::cout << dest << " -> { ";
                for (const std::pair<Node<T>, NodeWeight> &src : srcList)
                    std::cout << src.first << ": " << src.second << ", ";

                std::cout << "\b\b }" << std::endl;
            }

            std::cout << "-----" << std::endl;
        }

        void LoadFromFile(const std::string &filename) {
            // FILE FORMATTED AS:
            // node_count
            // node_names
            /*
                [adjacency matrix]
            */

            uint32_t nodeCount;
            std::vector<std::vector<double>> adjMatrix;
            std::string line;

            const std::regex whitespace("\\s+");
            std::string filename = "graf1.txt";
            std::ifstream file(filename);
            file.seekg(std::ios::beg);

            std::getline(file, line, '\n');
            line.pop_back(); // removes carriage return
            nodeCount = std::stoi(line);
            line.clear();

            std::getline(file, line, '\n');
            line.pop_back(); // removes carriage return
            std::sregex_token_iterator iter(line.begin(), line.end(), whitespace, -1);
            std::sregex_token_iterator end;
            std::vector<std::string> nodeNames(iter, end);
            m_NodeNames = nodeNames;
            line.clear();

            while (std::getline(file, line, '\n')) {
                line.pop_back(); // removes carriage return
                adjMatrix.push_back(tokenize(line));
                line.clear();
            }

            m_AdjMatrix = adjMatrix;

            // initialize node hashset
            int id = 0;
            for (const auto &name : nodeNames)
                m_Nodes.emplace(Node<std::string>(name, id++));

            // initialize the relation hashmap
            // OVDJE STAO
        }
};