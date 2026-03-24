#include "randutils.hpp"

hrand::hrand(int seed){
    if (seed >= 0) {
        gen.seed(seed);
        seed_val = seed;
    } else {
        seed_val = 42;
        gen.seed(seed_val);
    }
}

boost::random::mt19937& hrand::get_gen() {
    return gen;
}

int hrand::uniform_int(const int &a, const int &b){
    boost::random::uniform_int_distribution<int> dist(a,b);
    return dist(gen);
}

double hrand::uniform_real(const double& a, const double& b) {
    boost::random::uniform_real_distribution<double> dist(a, b);
    return dist(gen);
}

int hrand::generate_random_seed() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(
        1,
        std::numeric_limits<int>::max()
    );
    return dist(gen);
}