#include "network_construct.hpp"
#include "randutils.hpp"


string Network::method_used(const double random_value){
    if(random_value < alpha_value_m){
        return "alpha_method";
    }
    else{
        return "beta_method";
    }
}


// void Network::create_network(const int num_vertices_m, const double alpha_value_m, 
//                         const int seed_used_m,  const int num_vertices_initial_m){
        
    


// }