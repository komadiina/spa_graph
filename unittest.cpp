#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<double> tokenize(const std::string &feed, char delim = ' ') {
    std::stringstream ss(feed);
    std::string token;

    std::vector<double> result;

    while (std::getline(ss, token, delim))
        result.push_back(std::stod(token));

    return result;
}

int main() {
    int nodeCount;
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
    line.clear();

    for (const auto &elem : nodeNames)
        std::cout << elem << " ";
    std::cout << std::endl;

    while (std::getline(file, line, '\n')) {
        line.pop_back(); // removes carriage return
        adjMatrix.push_back(tokenize(line));
        line.clear();
    }

    for (const auto &row : adjMatrix) {
        for (const auto &elem : row)
            std::cout << elem << " ";

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}