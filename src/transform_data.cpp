#include "transform_data.hpp"
#include <boost/graph/graph_traits.hpp>

GraphParquetData TransformData::transform(const graph_t& G) {
    GraphParquetData data;

    const int N = static_cast<int>(boost::num_vertices(G));

    data.nodes.reserve(N);
    data.offsets.resize(N + 1, 0);
    data.degree.resize(N, 0);

    int total_neighbors = 0;
    int idx = 0;

    auto vp = boost::vertices(G);
    for (auto vit = vp.first; vit != vp.second; ++vit, ++idx) {
        data.nodes.push_back(static_cast<int>(*vit));

        const int deg = static_cast<int>(boost::degree(*vit, G));
        data.degree[idx] = deg;
        data.offsets[idx] = total_neighbors;
        total_neighbors += deg;
    }

    data.offsets[N] = total_neighbors;
    data.neighbors_flat.reserve(total_neighbors);

    vp = boost::vertices(G);
    for (auto vit = vp.first; vit != vp.second; ++vit) {
        auto adj = boost::adjacent_vertices(*vit, G);
        for (auto ait = adj.first; ait != adj.second; ++ait) {
            data.neighbors_flat.push_back(static_cast<int>(*ait));
        }
    }

    return data;
}