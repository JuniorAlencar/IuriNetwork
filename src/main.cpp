#include "network_construct.hpp"
#include "randutils.hpp"
#include "create_folders.hpp"
#include <iostream>
#include <string>

using namespace std;

static void print_help([[maybe_unused]] const char* prog){
    std::cout <<
R"(To run:
  ./SOP <num_vertices> <alpha_value> <seed> <num_vertices_initial>

Arguments:
  num_vertices         : Number of nodes in network (int)
  alpha_value          : Parameter model (alpha + beta = 1 and alpha <= 1) (double)
  seed                 : -1 to random seed (int)
  num_vertices_initial : Number initial of nodes (int)

Examples:
  ./exe1 1000 0.10 -1 5
  ./exe1 20000 0.5 23412 9

Tips:
  - Use seed = -1 to auto-generate a random seed.
  - alpha value must be <= 1, and beta = 1 - alpha.
)" << std::endl;
}

int main(int argc, char* argv[]){
    if (argc != 5){
        std::cerr << "[ERROR] Invalid number of arguments.\n";
        print_help(argv[0]);
        return 1;
    }

    int num_vertices = stoi(argv[1]);
    double alpha_value = stod(argv[2]);
    int seed = stoi(argv[3]);
    int num_vertices_initial = stoi(argv[4]);

    if (alpha_value > 1){
        std::cerr << "[ERROR] alpha must be <= 1.\n";
        print_help(argv[0]);
        return 1;
    }
    
    if (seed < 0) {
        seed = hrand::generate_random_seed();
    }
    hrand rng(seed);
    Network net(num_vertices, alpha_value, seed, num_vertices_initial);
    net.create_network();
    
    double beta = 1.0 - alpha_value;

    FolderCreator creator("./NET_data");
    auto data_path = creator.create_structure(num_vertices, alpha_value);
    cout << seed << endl;
    cout << num_vertices_initial << endl;
    cout << beta << endl;
    cout << data_path << endl;
    
    //int num_steps = 100;
    cout << seed << endl;


    return 0;
}