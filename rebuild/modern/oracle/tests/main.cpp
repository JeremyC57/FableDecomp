#include "OracleTestSupport.hpp"

#include <cstdlib>

namespace fable::oracle::test {
void registerSmokeTests(TestList& tests);
void registerPortTests(TestList& tests);
} // namespace fable::oracle::test

int main(int argc, char** argv) {
    using namespace fable::oracle;
    using namespace fable::oracle::test;

    const char* exe = std::getenv("FABLE_EXE");
    if (exe == nullptr || *exe == '\0') {
        std::cout << "SKIPPED: set FABLE_EXE to your retail Fable.exe to run oracle tests\n";
        return 77;
    }
    TestList tests;
    registerSmokeTests(tests);
    registerPortTests(tests);

    RetailOracle oracle(exe);
    int ran = 0;
    for (const auto& t : tests) {
        if (argc > 1 && t.name.find(argv[1]) == std::string::npos) {
            continue;
        }
        g_current = t.name;
        const int before = g_failures;
        oracle.reset();
        try {
            t.run(oracle);
        } catch (const std::exception& e) {
            std::cerr << "  [" << t.name << "] exception: " << e.what() << '\n';
            ++g_failures;
        }
        std::cout << (g_failures == before ? "PASS " : "FAIL ") << t.name << '\n';
        ++ran;
    }
    std::cout << ran << " oracle test(s), " << g_failures << " failure(s)\n";
    return g_failures == 0 ? 0 : 1;
}
