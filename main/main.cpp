#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <cmath>
#include <list>
#include "../kmean/graph.hpp"
#define NUM_POINTS 100001

void generate_infile() {
    std::ofstream outfile;
    outfile.open("input_points");
    int points = 1;
    int x, y;
    std::random_device device;
    std::mt19937 mt(device());
    std::uniform_int_distribution<int> distribution(1,2000);
    while(points < NUM_POINTS) {
        outfile << "p" << std::to_string(points) << " ";
        x = distribution(mt);
        y = distribution(mt);
        if(points < NUM_POINTS - 1) {
            outfile << x << " " << y << " ;\n";
        }
        else {
            outfile << x << " " << y << " ;";
        }

        points++;
    }
    outfile.close();
}


int main(int argc, char** argv) {
    //generate points
    //generate_infile();
    int k = std::stoi(argv[1]);
    Graph graph(k);
    graph.create_graph(argv[2]);
    graph.init_cluster();
    bool if_coverage = false;
    //int it = 0;
    while(!if_coverage) {
        if_coverage = graph.find_cluster();
        graph.update_mean();
    }
    std::cout << "finsihed clustering! \n";
    graph.output_graph(argv[3]);
    return 0;
}
