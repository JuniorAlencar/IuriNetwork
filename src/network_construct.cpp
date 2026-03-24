#include "network_construct.hpp"
#include "randutils.hpp"


void Network::make_initial_complete_graph() {
    const int m_init = num_vertices_initial_m + 1;

    for (int i = 0; i < m_init; ++i) {
        boost::add_vertex(G);
    }

    for (int i = 0; i < m_init; ++i) {
        for (int j = i + 1; j < m_init; ++j) {
            boost::add_edge(i, j, G);
        }
    }
}

Network::Network(int num_vertices_m, double alpha_value_m,
                 int seed_used_m, int num_vertices_initial_m)
    : num_vertices_m(num_vertices_m),
      alpha_value_m(alpha_value_m),
      seed_used_m(seed_used_m),
      num_vertices_initial_m(num_vertices_initial_m)
{
    if (num_vertices_initial_m < 1)
        throw std::runtime_error("num_vertices_initial_m must be >= 1");

    if (num_vertices_m < num_vertices_initial_m + 1)
        throw std::runtime_error("For minimum degree m0, require num_vertices_m >= m0 + 1");

    make_initial_complete_graph();
}

void Network::create_network() {
    hrand rnd(seed_used_m);

    if (num_vertices_initial_m < 1)
        throw std::runtime_error("num_vertices_initial_m must be >= 1");

    if (num_vertices_m < num_vertices_initial_m + 1)
        throw std::runtime_error("For minimum degree m0, require num_vertices_m >= m0 + 1");
    
    int m_init = num_vertices_initial_m + 1;
    int counter_alpha = 0;
    for (int i = m_init; i < num_vertices_m; ++i) {
        boost::add_vertex(G);
        vertex_t new_node = i;

        double random_number_method = rnd.uniform_real(0.0, 1.0);
        int random_number_node = rnd.uniform_int(0, i - 1);
        vertex_t select_node = static_cast<vertex_t>(random_number_node);
        
        if (random_number_method < alpha_value_m) {
            std::vector<vertex_t> neighbors;
            neighbors.reserve(boost::degree(select_node, G));

            auto [it, end] = adjacent_vertices(select_node, G);
            for (; it != end; ++it) {
                neighbors.push_back(*it);
            }

            for (vertex_t j : neighbors) {
                if (j != new_node && !boost::edge(new_node, j, G).second) {
                    boost::add_edge(new_node, j, G);
                }
            }
            counter_alpha += 1;
        } else {
            if (!boost::edge(new_node, select_node, G).second) {
                boost::add_edge(new_node, select_node, G);
            }
        }
    }
    std::cout << "Initial number of vertices: " << counter_alpha << '\n';
    std::cout << "Initial number of vertices: " << boost::num_vertices(G) << '\n';
    std::cout << "Initial number of edges: " << boost::num_edges(G) << '\n';
}