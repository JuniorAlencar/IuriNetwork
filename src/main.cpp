#include "network_construct.hpp"
#include "randutils.hpp"
#include "create_folders.hpp"
#include "transform_data.hpp"
#include "write_data.hpp"
#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static std::string sanitize_for_filename(std::string s) {
    for (char& c : s) {
        const bool ok =
            std::isalnum(static_cast<unsigned char>(c)) ||
            c == '-' || c == '_';
        if (!ok) c = '_';
    }
    return s;
}

static std::string get_machine_name() {
#ifdef _WIN32
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer);
    if (GetComputerNameA(buffer, &size)) {
        return sanitize_for_filename(std::string(buffer, size));
    }
    return "unknown_host";
#else
    char buffer[256];
    if (gethostname(buffer, sizeof(buffer)) == 0) {
        buffer[sizeof(buffer) - 1] = '\0';
        return sanitize_for_filename(std::string(buffer));
    }
    return "unknown_host";
#endif
}

static std::string get_timestamp_now() {
    using namespace std::chrono;
    const auto now = system_clock::now();
    const std::time_t tt = system_clock::to_time_t(now);

    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &tt);
#else
    localtime_r(&tt, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%dT%H%M%S");
    return oss.str();
}

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
    graph_t G = net.get_graph();

    FolderCreator creator("./NET_data");
    auto data_path = creator.create_structure(num_vertices, alpha_value);
    
    cout << seed << endl;    
    cout << data_path << endl;
    
    const std::string machine_name = get_machine_name();
    const std::string timestamp_now = get_timestamp_now();
    std::ostringstream base_name;
    base_name << machine_name
            << "_seed_" << seed
            << "_ts_" << timestamp_now;
    
    const std::string sample_name = base_name.str();
    std::string filename = data_path + "/" + sample_name + ".parquet";
    
    GraphParquetData data = TransformData::transform(G);
    SaveParquet::save_graph(data, filename);

    return 0;
}