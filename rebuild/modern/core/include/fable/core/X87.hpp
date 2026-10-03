#pragma once

// Helpers for reproducing the retail x87 arithmetic bit-for-bit on any CPU.
//
// Fable.exe runs the x87 FPU with the MSVC 7.1 control word 0x027F: 53-bit
// precision, round-to-nearest-even. Under that mode every x87 operation on
// values that stay in registers rounds exactly like IEEE binary64, and every
// `fstp DWORD` rounds to binary32. A port therefore matches retail bit-for-bit
// when it (1) does register arithmetic in `double`, (2) rounds to `float`
// exactly where the original stores to a float, and (3) keeps the original
// operation order. Translation units using this header must be compiled
// without floating-point contraction (-ffp-contract=off; no FMA fusion).

#include <cmath>
#include <cstdint>
#include <limits>

namespace fable::x87 {

/// `fstp DWORD PTR`: round a register value to binary32.
[[nodiscard]] inline float store(double v) noexcept { return static_cast<float>(v); }

/// `fistp DWORD PTR`: round to nearest-even; out of range or NaN gives the
/// x87 "integer indefinite" 0x80000000.
[[nodiscard]] inline std::int32_t fistp(double v) noexcept {
    if (!(v > -2147483648.5 && v < 2147483647.5)) {
        return std::numeric_limits<std::int32_t>::min();
    }
    const double r = std::nearbyint(v);
    if (r > 2147483647.0 || r < -2147483648.0) {
        return std::numeric_limits<std::int32_t>::min();
    }
    return static_cast<std::int32_t>(r);
}

/// `fcomp` followed by `test ah,5; jp` — true when a < b (false for NaN).
[[nodiscard]] inline bool less(double a, double b) noexcept { return a < b; }

} // namespace fable::x87
