#ifndef TRANSFORM_DATA_HPP
#define TRANSFORM_DATA_HPP

#include "network_construct.hpp"
#include <vector>

struct GraphParquetData {
    std::vector<int> nodes;
    std::vector<int> offsets;
    std::vector<int> neighbors_flat;
    std::vector<int> degree;
};

class TransformData {
public:
    static GraphParquetData transform(const graph_t& G);
};

#endif // !transform_data_hpp