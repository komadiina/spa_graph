#pragma once

#include "Matrix.hpp"
#include "Node.hpp"
#include "Relation.hpp"
#include "literals.hpp"

#include <algorithm>
#include <climits>
#include <fstream>
#include <initializer_list>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T> class Graph {
    private:
        using NodeSet = std::multiset<std::pair<Node<T>, NodeWeight>>;
        using NodeWeight = double;

    private:
        std::unordered_set<Node<T>, NodeHash<T>> m_Nodes;
        std::unordered_map<Node<T>, NodeSet, NodeHash<T>> m_Connectivity;
        NodeWeight m_TotalWeight = 0.0;

        Matrix<NodeWeight> m_AdjMatrix;
        std::vector<std::string> m_NodeNames;
        std::unordered_map<Node<T>, bool, NodeHash<T>> m_Visited;
        std::vector<std::pair<Node<T>, int32_t>> m_Indices;
        std::unordered_map<Node<T>, NodeSet, NodeHash<T>> m_EdgeDistances;
        std::unordered_map<Node<T>, NodeSet, NodeHash<T>> m_WeightDistances;

        bool m_edges_initialized = false;
        bool m_weights_initialized = false;

    public:
        Graph() {}

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

        const auto GetMatrix() const { return m_AdjMatrix; }
        auto &GetMatrix() { return m_AdjMatrix; }

        void TryConnect(std::initializer_list<Relation<Node<T>, NodeWeight>> relations) {
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
                    m_Connectivity[src].insert({dest, wt});

                    m_edges_initialized = false;

                } catch (const std::out_of_range &) {
                    std::cout << "Node " << relation.template get<0>()
                              << " not found in map.\n";
                    continue;
                }
            }
        }

        void Connect(std::initializer_list<Relation<Node<T>, NodeWeight>> relations) {
            for (Relation<Node<T>, NodeWeight> relation : relations) {
                Node<T> from = relation.from();
                Node<T> to = relation.to();
                NodeWeight wt = relation.weight();

                m_Nodes.emplace(from);
                m_Nodes.emplace(to);
                m_Connectivity[from]; // ensures insertion
                m_Connectivity.at(from).insert(std::make_pair(to, wt));

                m_edges_initialized = false;
            }
        }

        void Connect(Relation<Node<T>, NodeWeight> relation) {
            Node<T> from = relation.from();
            Node<T> to = relation.to();
            NodeWeight wt = relation.weight();

            m_Nodes.emplace(from);
            m_Nodes.emplace(to);
            m_Connectivity[from]; // ensures insertion
            m_Connectivity.at(from).insert(std::make_pair(to, wt));

            m_edges_initialized = false;
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

                    m_edges_initialized = false;
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

                m_edges_initialized = false;
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

                m_edges_initialized = false;
            } catch (const std::out_of_range &) {
                std::cout << "Node not in map. (" << key << ")\n";
                return;
            }
        }

        friend std::ostream &operator<<(std::ostream &os, const Graph<T> &obj) {
            std::unordered_set<Node<T>, NodeHash<T>> nodes = obj.GetNodes();

            os << "-------\nNodes:\n";
            for (Node<T> node : nodes)
                os << "{ " << node.GetData() << " }\n";

            os << "\nDelta:\n";
            obj.PrintConnections(os);

            return os << "-------";
        }

        void PrintConnections(std::ostream &os) const {
            size_t max = 0;
            for (const std::string &name : m_NodeNames)
                if (name.length() > max)
                    max = name.length();

            for (auto [dest, srcList] : m_Connectivity) {
                if (srcList.empty()) {
                    os << "[X] " << std::setw(max - 3) << dest
                       << " does not connect to any node.\n";
                    continue;
                }

                os << std::setw(max) << dest << " -> { ";
                for (const std::pair<Node<T>, NodeWeight> &src : srcList)
                    os << src.first << ": " << src.second << ", ";

                os << "\b\b }" << std::endl;
            }
        }

        void LoadFromFile(const std::string &filename = ".\\src\\graf2.txt") {
            // FILE FORMATTED AS:
            // node_count
            // node_names
            /*
                [adjacency matrix]
            */
            m_edges_initialized = false;

            uint32_t nodeCount;
            std::vector<std::vector<NodeWeight>> adjMatrix;
            std::string line;

            const std::regex whitespace("\\s+");
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
            std::vector<T> nodeNames(iter, end);
            m_NodeNames = nodeNames;
            line.clear();

            while (std::getline(file, line, '\n')) {
                if (line[line.size()] == '\r')
                    line.pop_back(); // removes carriage return

                adjMatrix.push_back(tokenizeWeights(line));
                line.clear();
            }

            m_AdjMatrix = adjMatrix;

            // initialize node hashset
            for (const auto &name : nodeNames) {
                Node<T> node(name);
                m_Nodes.emplace(node);
                m_Connectivity[node];
            }

            // initialize the relation hashmap
            for (int i = 0; i < nodeCount; i++) {
                Node<T> source(m_NodeNames.at(i));

                for (int j = 0; j < nodeCount; j++) {
                    NodeWeight wt = (adjMatrix.at(i)).at(j);
                    Node<T> dest(m_NodeNames.at(j));

                    if (wt) {
                        Relation<Node<T>, NodeWeight> rel(source, dest, wt);
                        Connect(rel);
                    }
                }
            }

            init_edge_distances();
        }

        template <typename RType>
        void DFS(Node<T> start, const std::function<RType(Node<T>, int16_t)> &action) {
            if (m_Nodes.find(start) == m_Nodes.end()) {
                std::cout << "[!] Node \"" << start
                          << "\" not found in graph - returning..." << std::endl;
                return;
            }

            std::cout << "DFS traversal starting from node [" << start
                      << "]:" << std::endl;

            std::stack<Node<T>> st({start});
            uint16_t iteration = 1;

            // refresh the visited-map and init with a new, false-filled one
            m_Visited.clear();
            for (Node<T> node : m_Nodes)
                m_Visited[node] = false;

            while (st.size() > 0) {
                Node<T> current = st.top();
                st.pop();

                if (m_Visited.at(current) == false) {
                    action(current, iteration++);
                    m_Visited.at(current) = true;

                    std::unordered_set<Node<T>, NodeHash<T>> neighbors =
                        neighborsOf(current);
                    for (Node<T> w : neighbors)
                        if (m_Visited.at(w) == false)
                            st.push(w);
                }
            }
        }

        std::vector<std::pair<Node<T>, NodeWeight>> GetClosest(Node<T> target,
                                                               int16_t limit = 5) {
            std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> spt = Dijkstra(target);

            std::vector<std::pair<Node<T>, NodeWeight>> vec = hashmapToVector(spt),
                                                        result;
            std::sort(vec.begin(), vec.end(), [](const auto &pA, const auto &pB) {
                return pA.second < pB.second;
            });

            for (const auto &elem : vec)
                if (elem.second != 0 && elem.second != INF)
                    result.push_back(elem);

            if (limit < result.size())
                result.resize(limit);

            return result;
        }

        std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> Dijkstra(
            Node<T> source, std::string flag = "weights") {
            const size_t numNodes = m_Nodes.size();
            std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> distances, numEdges;
            std::unordered_set<Node<T>, NodeHash<T>> spt;

            m_Visited.clear();
            init_indices();

            for (Node<T> node : m_Nodes) {
                distances[node] = INF;
                numEdges[node] = INF;
                m_Visited[node] = false;
            }

            distances[source] = 0;
            numEdges[source] = 0;

            while (spt.size() < m_Nodes.size()) {
                Node<T> current = findMinNode(spt, distances);
                spt.emplace(current);
                m_Visited[current] = true;

                NodeSet adjacent = m_Connectivity[current];

                for (std::pair<Node<T>, NodeWeight> w : adjacent) {
                    if (m_Visited[w.first] == false) {
                        m_Visited[w.first] = true;
                        if (distances[current] + w.second < distances[w.first]) {
                            distances[w.first] = distances[current] + w.second;
                            numEdges[w.first] = numEdges[current] + 1;
                        }
                    }
                }
            }

            std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> edges, weights;

            for (auto &[k, v] : numEdges)
                if (v != INF)
                    edges[k] = v;

            for (auto &[k, v] : distances)
                if (v != INF)
                    weights[k] = v;

            if (flag == "weights")
                return weights;
            else
                return edges;
        }

        void printEdgeDistances() {
            init_edge_distances();

            for (auto [node, list] : m_EdgeDistances) {
                std::cout << "[" << node << "]" << std::endl;

                for (const auto &elem : list)
                    if (elem.second > 0)
                        std::cout << elem.first << " = " << elem.second << std::endl;

                std::cout << std::endl;
            }
        }

        void printWeightDistances() {
            init_weights();
            for (auto [node, list] : m_WeightDistances) {
                std::cout << "[" << node << "]" << std::endl;

                for (const auto &elem : list)
                    if (elem.second > 0)
                        std::cout << elem.first << " = " << elem.second << std::endl;

                std::cout << std::endl;
            }
        }

    private:
        void init_weights() {
            if (m_weights_initialized == true)
                return;
            for (const Node<T> &node : m_Nodes) {
                std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> distanceTo =
                    Dijkstra(node, "weights");
                m_WeightDistances[node];

                for (auto [k, v] : distanceTo)
                    m_WeightDistances[node].insert({k, v});
            }

            m_weights_initialized = true;
        }

        void init_edge_distances() {
            if (m_edges_initialized == true)
                return;

            for (const Node<T> &node : m_Nodes) {
                std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> distanceTo =
                    Dijkstra(node, "edges");
                m_EdgeDistances[node];

                for (auto [k, v] : distanceTo)
                    m_EdgeDistances[node].insert({k, v});
            }

            m_edges_initialized = true;
        }

        // Returns an unsorted vector
        std::vector<std::pair<Node<T>, NodeWeight>> hashmapToVector(
            std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> dict) {
            std::vector<std::pair<Node<T>, NodeWeight>> result;

            for (const auto &elem : dict)
                result.push_back(elem);

            return result;
        }

        Node<T> findMinNode(
            const std::unordered_set<Node<T>, NodeHash<T>> &spt,
            const std::unordered_map<Node<T>, NodeWeight, NodeHash<T>> &distances) {

            Node<T> minNode;
            NodeWeight minValue = INF + 1; // :DDDD

            for (const auto &[node, value] : distances) {
                if (spt.contains(node) == false && value <= minValue) {
                    minNode = node;
                    minValue = value;
                }
            }

            return minNode;
        }

        std::pair<Node<T>, NodeWeight> closest(Node<T> source) {
            NodeSet distances = m_Connectivity[source];

            std::pair<Node<T>, NodeWeight> nearest = distances.front();

            for (const std::pair<Node<T>, NodeWeight> &elem : distances)
                if (elem.second < nearest.second)
                    nearest = elem;

            return nearest;
        }

        std::vector<NodeWeight> tokenizeWeights(const std::string &feed,
                                                char delim = ' ') {
            std::stringstream ss(feed);
            std::string token;

            std::vector<NodeWeight> result;

            while (std::getline(ss, token, delim))
                result.push_back(std::stod(token));

            return result;
        }

        std::unordered_set<Node<T>, NodeHash<T>> neighborsOf(Node<T> target) {
            std::unordered_set<Node<T>, NodeHash<T>> neighbors;
            NodeSet connectedNodes = m_Connectivity[target];

            for (auto [node, wt] : connectedNodes)
                neighbors.emplace(node);

            return std::unordered_set<Node<T>, NodeHash<T>>(neighbors);
        }

        void init_indices() {
            m_Indices.clear();
            int32_t i = 0;

            for (Node<T> node : m_Nodes)
                m_Indices.push_back(std::make_pair(node, i++));
        }
        /* --- */
};