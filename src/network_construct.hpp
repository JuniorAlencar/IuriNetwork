#ifndef network_construct_hpp
#define network_construct_hpp

#include "randutils.hpp"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>

#include <iostream>

using namespace boost;
using namespace std;

using Graph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_weight_t, double>
>;

typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_degree_t, int>>
    graph_t;

typedef graph_traits<graph_t>::edge_descriptor edge_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_t;
typedef std::pair<vertex_t, int> vertex_prop_int;
typedef std::pair<vertex_t, double> vertex_prop_double;

typedef graph_traits<graph_t>::vertex_iterator vertex_it;
typedef graph_traits<graph_t>::edge_iterator edge_it;

class Network{
    private:
        graph_t G; // The graph

        int num_vertices_m; // Number of nodes
        double alpha_value_m; // alpha parameter
        //double beta_value; // beta parameter
        int seed_used_m;   // seed used
        int num_vertices_initial_m; // number of initial nodes (m)
        
    public:
        Network(int num_vertices_m, double alpha_value_m,
            int seed_used_m, int num_vertices_initial_m);
        
        void create_network();
        void make_initial_complete_graph();
        const graph_t& get_graph() const { return G; }
};

#endif // !network_construct_hpp