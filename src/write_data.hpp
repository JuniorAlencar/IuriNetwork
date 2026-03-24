#ifndef WRITE_DATA_HPP
#define WRITE_DATA_HPP

#include "transform_data.hpp"
#include <string>

class SaveParquet {
public:
    static void save_graph(const GraphParquetData& data, const std::string& output_file);
};

#endif // !write_data_hpp