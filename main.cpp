#include <fstream>
#include <iostream>

// build main
// -- or --
// g++ main.cpp -o main -g -Iincl/ -std=c++23

#include "./incl/Graph.hpp"
#include "./incl/Node.hpp"
#include "./incl/Relation.hpp"
#include "./incl/literals.hpp"

int main(int argC, char **argV) {
    using namespace std::string_literals;
    using DataType = std::string;
    using RelationType = Relation<Node<DataType>, NodeWeight>;

    std::string filename;
    std::cout << "Naziv tekstualnog fajla: ";
    std::getline(std::cin, filename, '\n');

    Graph<DataType> graph;
    graph.LoadFromFile(filename);
    graph.InitDistances();
    std::cout << graph << std::endl;

    std::string sel = "1";
    do {
        std::cout << "\t[1] - Traverse using DFS\n\t[2] - Print edge-wise "
                     "connections\n\t[3] - Print weight-wise connections\n\t[4] - "
                     "Save graph data and quit\n> ";

        std::getline(std::cin, sel, '\n');

        if (sel == "1") {
            std::cout << "Enter node name: ";
            DataType name;
            std::getline(std::cin, name, '\n');

            graph.template DFS<void>(Node<DataType>(name),
                                     [](Node<DataType> node, int16_t i) {
                                         std::cout << i++ << ". " << node << std::endl;
                                     });
        } else if (sel == "2") {
            std::cout << "Enter node name (leave blank to print all): ";
            DataType name;
            std::getline(std::cin, name, '\n');
            graph.printEdgeDistances(name);
        } else if (sel == "3") {
            std::cout << "Enter node name (leave blank to print all): ";
            DataType name;
            std::getline(std::cin, name, '\n');
            graph.printWeightDistances(name);
        } else if (sel == "4") {
            graph.DumpData();
            std::cout << "Done." << std::endl;
            return EXIT_SUCCESS;
        } else {
            std::cout << "Invalid option." << std::endl;
        }
    } while (sel == "1" || sel == "2" || sel == "3" || sel == "4");

    graph.DumpData();

    std::cout << "Done." << std::endl;
    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}