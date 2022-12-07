#include <iostream>

// g++ main.cpp -o main -g -Iincl/ -std=c++20

#include "./incl/Graph.hpp"
#include "./incl/Node.hpp"
#include "./incl/Relation.hpp"
#include "./incl/literals.hpp"

int main(int argC, char **argV) {
    using namespace std::string_literals;
    using DataType = std::string;
    using RelationType = Relation<Node<DataType>, NodeWeight>;

    // int_fast32_t i = 1;

    // Node<DataType> srbac("Srbac"s, i++);
    // Node<DataType> banja_luka("Banja Luka"s, i++);
    // Node<DataType> beograd("Beograd"s, i++);
    // Node<DataType> bugojno("Bugojno"s, i++);
    // Node<DataType> novi_sad("Novi Sad"s, i++);

    // Graph<DataType> graph({srbac, banja_luka, beograd, bugojno, novi_sad});

    // std::cout << "Graph nodes: " << graph << std::endl;

    // std::vector<RelationType> relations({RelationType(srbac, banja_luka, 49.5),
    //                                      RelationType(banja_luka, beograd, 275.1),
    //                                      RelationType(bugojno, novi_sad, 102),
    //                                      RelationType(banja_luka, novi_sad, 180.2)});

    // for (auto relation : relations)
    //     graph.TryConnect({relation});
    // graph.PrintConnections();

    // graph.TryDisconnect(srbac, banja_luka);
    // graph.PrintConnections();

    Graph<DataType> graph;
    graph.LoadFromFile();
    std::cout << "Graph contents:\n" << graph << std::endl;

    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}