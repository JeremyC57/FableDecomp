#pragma once

#include "fable/oracle/RetailOracle.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace fable::oracle::test {

inline int g_failures = 0;
inline std::string g_current;

#define CHECK(cond)                                                                             \
    do {                                                                                        \
        if (!(cond)) {                                                                          \
            std::cerr << "  [" << ::fable::oracle::test::g_current << "] " << __FILE__ << ':'   \
                      << __LINE__ << ": CHECK failed: " #cond "\n";                             \
            ++::fable::oracle::test::g_failures;                                                \
        }                                                                                       \
    } while (false)

#define CHECK_EQ(a, b)                                                                          \
    do {                                                                                        \
        const auto va_ = (a);                                                                   \
        const auto vb_ = (b);                                                                   \
        if (!(va_ == vb_)) {                                                                    \
            std::cerr << "  [" << ::fable::oracle::test::g_current << "] " << __FILE__ << ':'   \
                      << __LINE__ << ": " #a " == " #b " failed (" << va_ << " vs " << vb_      \
                      << ")\n";                                                                 \
            ++::fable::oracle::test::g_failures;                                                \
        }                                                                                       \
    } while (false)

struct TestCase {
    std::string name;
    std::function<void(RetailOracle&)> run;
};
using TestList = std::vector<TestCase>;

/// Deterministic inputs so a failure reproduces exactly.
inline std::mt19937& rng() {
    static std::mt19937 gen(0xFAB1E);
    return gen;
}

inline float randomFloat(float lo, float hi) {
    return std::uniform_real_distribution<float>(lo, hi)(rng());
}

inline std::uint32_t randomU32() { return rng()(); }

} // namespace fable::oracle::test
