#ifndef randutils_hpp
#define randutils_hpp

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <ctime>
#include <cmath>
#include <random>
#include <limits>

class hrand {
  private:
    boost::random::mt19937 gen;
    int seed_val;

  public:
    explicit hrand(int seed);
    int get_seed() const { return seed_val; }
    boost::random::mt19937& get_gen();

    int uniform_int(const int& a, const int& b);
    double uniform_real(const double& a, const double& b);

    static int generate_random_seed();
};

namespace sam {} // namespace sam

#endif // randutils_hpp