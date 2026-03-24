#include "write_data.hpp"

#include <stdexcept>
#include <memory>

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <parquet/arrow/writer.h>

namespace {

void check_graph_data_consistency(const GraphParquetData& data) {
    const std::size_t n = data.nodes.size();

    if (data.degree.size() != n) {
        throw std::runtime_error("Erro: degree.size() deve ser igual a nodes.size().");
    }

    if (data.offsets.size() != n + 1) {
        throw std::runtime_error("Erro: offsets.size() deve ser nodes.size() + 1.");
    }

    if (data.offsets.empty()) {
        throw std::runtime_error("Erro: offsets não pode ser vazio.");
    }

    if (data.offsets[0] != 0) {
        throw std::runtime_error("Erro: offsets[0] deve ser 0.");
    }

    for (std::size_t i = 0; i + 1 < data.offsets.size(); ++i) {
        if (data.offsets[i] > data.offsets[i + 1]) {
            throw std::runtime_error("Erro: offsets deve ser não-decrescente.");
        }
    }

    if (static_cast<std::size_t>(data.offsets.back()) != data.neighbors_flat.size()) {
        throw std::runtime_error("Erro: offsets.back() deve ser igual a neighbors_flat.size().");
    }

    for (std::size_t i = 0; i < n; ++i) {
        const int deg_from_offsets = data.offsets[i + 1] - data.offsets[i];
        if (deg_from_offsets != data.degree[i]) {
            throw std::runtime_error("Erro: degree[i] inconsistente com offsets.");
        }
    }
}

std::shared_ptr<arrow::Array> build_int32_array(const std::vector<int>& values) {
    arrow::Int32Builder builder;
    auto status = builder.AppendValues(values);
    if (!status.ok()) {
        throw std::runtime_error("Erro ao construir array int32: " + status.ToString());
    }

    std::shared_ptr<arrow::Array> array;
    status = builder.Finish(&array);
    if (!status.ok()) {
        throw std::runtime_error("Erro ao finalizar array int32: " + status.ToString());
    }

    return array;
}

std::shared_ptr<arrow::Array> build_neighbors_list_array(const GraphParquetData& data) {
    arrow::MemoryPool* pool = arrow::default_memory_pool();

    auto value_builder = std::make_shared<arrow::Int32Builder>(pool);
    arrow::ListBuilder list_builder(pool, value_builder);
    auto* int_builder = static_cast<arrow::Int32Builder*>(list_builder.value_builder());

    const std::size_t n = data.nodes.size();

    for (std::size_t i = 0; i < n; ++i) {
        auto status = list_builder.Append();
        if (!status.ok()) {
            throw std::runtime_error("Erro ao iniciar lista de neighbors: " + status.ToString());
        }

        const int begin = data.offsets[i];
        const int end   = data.offsets[i + 1];

        for (int j = begin; j < end; ++j) {
            status = int_builder->Append(data.neighbors_flat[j]);
            if (!status.ok()) {
                throw std::runtime_error("Erro ao adicionar neighbor: " + status.ToString());
            }
        }
    }

    std::shared_ptr<arrow::Array> neighbors_array;
    auto status = list_builder.Finish(&neighbors_array);
    if (!status.ok()) {
        throw std::runtime_error("Erro ao finalizar coluna neighbors: " + status.ToString());
    }

    return neighbors_array;
}

} // namespace

void SaveParquet::save_graph(const GraphParquetData& data, const std::string& output_file) {
    check_graph_data_consistency(data);

    std::shared_ptr<arrow::Array> nodes_array     = build_int32_array(data.nodes);
    std::shared_ptr<arrow::Array> degree_array    = build_int32_array(data.degree);
    std::shared_ptr<arrow::Array> neighbors_array = build_neighbors_list_array(data);

    auto schema = arrow::schema({
        arrow::field("nodes", arrow::int32()),
        arrow::field("neighbors", arrow::list(arrow::int32())),
        arrow::field("degree", arrow::int32())
    });

    auto table = arrow::Table::Make(
        schema,
        {nodes_array, neighbors_array, degree_array}
    );

    std::shared_ptr<arrow::io::FileOutputStream> outfile;
    auto result_outfile = arrow::io::FileOutputStream::Open(output_file);
    if (!result_outfile.ok()) {
        throw std::runtime_error("Erro ao abrir arquivo Parquet: " + result_outfile.status().ToString());
    }
    outfile = *result_outfile;

    constexpr int64_t chunk_size = 1024;

    auto status = parquet::arrow::WriteTable(
        *table,
        arrow::default_memory_pool(),
        outfile,
        chunk_size
    );

    if (!status.ok()) {
        throw std::runtime_error("Erro ao escrever arquivo Parquet: " + status.ToString());
    }

    status = outfile->Close();
    if (!status.ok()) {
        throw std::runtime_error("Erro ao fechar arquivo Parquet: " + status.ToString());
    }
}