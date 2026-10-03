// Retail oracle smoke tests. Needs FABLE_EXE=<path to the user's Fable.exe>;
// exits 77 (CTest SKIPPED) without it.

#include "OracleTestSupport.hpp"

#include <cmath>
#include <cstdlib>

namespace fable::oracle::test {
namespace {

// FtoL @ 0x00C6B240: fld [esp+8]; fistp -> round-to-nearest-even under the
// MSVC 7.1 default control word.
void ftolMatchesRoundToNearestEven(RetailOracle& o) {
    for (const float f : {0.0f, 0.4f, 0.5f, 1.5f, 2.5f, -0.5f, -1.5f, -2.6f, 123456.7f, -98765.25f}) {
        const std::uint32_t args[] = {RetailOracle::bits(f)};
        const auto r = o.call(0x00C6B240, CallingConvention::Cdecl, args);
        CHECK_EQ(static_cast<std::int32_t>(r.eax), static_cast<std::int32_t>(std::nearbyint(f)));
    }
}

void unstubbedImportIsReported(RetailOracle& o) {
    bool listed = false;
    for (const auto& name : o.imports()) {
        listed = listed || name == "KERNEL32.dll!GetTickCount";
    }
    CHECK(listed);
}

} // namespace

void registerSmokeTests(TestList& tests) {
    tests.push_back({"FtoL", ftolMatchesRoundToNearestEven});
    tests.push_back({"Imports", unstubbedImportIsReported});
}

} // namespace fable::oracle::test
