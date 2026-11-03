// fable_x87_twins.h -- plain C++ models of the x87 integer stores the engine's inline-assembly helpers run, for the
// FABLE_PORTABLE twins of engine/GFFloatToLong*.h (and the dual-path tests, FABLE_DUAL_PATH_TEST). Nothing here is
// compiled into the byte-matching build: every definition sits behind FABLE_PORTABLE / FABLE_DUAL_PATH_TEST.
//
// The model is the x87 FPU state the retail code runs under: rounding control = round to nearest, ties to even (the
// power-on / CRT default; Direct3D 9 does not change RC), invalid-operation exception masked (so an out-of-range or NaN
// source stores the "integer indefinite" 0x80000000), and arithmetic on float operands rounded no coarser than double
// (precision control 53 or 64 bits). Under the single-precision control word Direct3D 9 sets on device creation
// (no D3DCREATE_FPU_PRESERVE) an `fadd`/`fsub` of a float and 0.5 is rounded to 24 bits first; the twins follow the
// double-precision result (identical for every |x| >= 2^-24 and for every input the dual-path tests probe).
#ifndef FABLE_X87_TWINS_H
#define FABLE_X87_TWINS_H
#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#include <string.h>

namespace fable_portable
{
    // `fistp dword` of an x87 value under RC = nearest-even: the same long a bare `fistp dword ptr [m]` stores.
    inline long X87FistpNear(double v)
    {
        // Valid iff the nearest-even integer fits in 32 bits: -2147483648.5 rounds (to even) to -2^31, 2147483647.5 to
        // 2^31. NaN fails both compares.
        if (!(v >= -2147483648.5 && v < 2147483647.5))
            return -2147483647L - 1L;                                   // integer indefinite 0x80000000
        long t = (long)v;                                               // truncation toward zero, exact in range
        double r = v - (double)t;                                       // exact, in (-1, 1)
        if (r > 0.5)
            ++t;
        else if (r < -0.5)
            --t;
        else if ((r == 0.5 || r == -0.5) && (t & 1L) != 0)
            t += (r > 0.0) ? 1L : -1L;                                  // a tie goes to the even neighbour
        return t;
    }

    // The 32-bit pattern of a float, as `mov eax, <float>` / `cmp eax, <float>` see it.
    inline unsigned int FloatBits(float f)
    {
        unsigned int u;
        memcpy(&u, &f, sizeof u);
        return u;
    }
}   // namespace fable_portable

#endif  // FABLE_PORTABLE || FABLE_DUAL_PATH_TEST
#endif  // FABLE_X87_TWINS_H
