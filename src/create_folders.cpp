#include "create_folders.hpp"
#include "network_construct.hpp"

namespace fs = std::filesystem;

FolderCreator::FolderCreator(const std::string& base_path)
    : base_path(base_path) {}

std::string FolderCreator::create_structure(
    int num_vertices_m,
    double alpha_value_m
) {
    char main_folder[256];
    std::snprintf(
        main_folder,
        sizeof(main_folder),
        "%s/raw/N_%d/alpha_%.3f",
        base_path.c_str(),
        num_vertices_m,
        alpha_value_m
    );

    std::string full_path = main_folder;

    fs::create_directories(full_path);

    return full_path;
}