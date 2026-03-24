#ifndef CREATE_FOLDERS_HPP
#define CREATE_FOLDERS_HPP

#include <string>
#include <filesystem>

class FolderCreator {
public:
    FolderCreator(const std::string& base_path);

    std::string create_structure(
        int num_vertices_m,
        double alpha_value_m
    );

private:
    std::string base_path;
};

#endif // CREATE_FOLDERS_HPP