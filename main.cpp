#include <iostream>

// g++ main.cpp -o main -g -Iincl/ -std=c++20

#include "./incl/Graph.hpp"
#include "./incl/Node.hpp"
#include "./incl/Relation.hpp"
#include "./incl/literals.hpp"

int main(int argC, char **argV) {
	using namespace std::string_literals;
	std::string srbac = "Srbac";
	std::string bl = "Banja Luka";
	std::string bg = "Beograd";

	Graph<std::string> graph({{bl, 42}, {srbac, 44}, {bg, 15}});

	std::cout << "Graph nodes: " << graph << std::endl;
	graph.TryConnect({Relation(Node<std::string>("Banja Luka"s, 42), Node<std::string>("Beograd"s, 15), 276.5)});

	return EXIT_SUCCESS;
}