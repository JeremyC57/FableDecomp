#include "OracleTestSupport.hpp"

namespace fable::oracle::test {
void registerCoreMathTests(TestList& tests);

void registerPortTests(TestList& tests) {
    registerCoreMathTests(tests);
}
} // namespace fable::oracle::test
