#ifndef FABLE_NVECTORMATH_NORMALISE_PORTABLE_H
#define FABLE_NVECTORMATH_NORMALISE_PORTABLE_H
#ifdef FABLE_RETAIL_VECTOR_NORMALISE_PASS29
#include <math.h>
#include "engine/C4DVector.h"
namespace fable_portable {
// The x87 compiler can retain a wider sum even across a float assignment.
// Round intermediate SIMD lanes explicitly to the IEEE binary32 grid.
inline double RoundLane(double value)
{
    if (!(value > 0.0))
        return value;
    if (value >= ldexp(2.0 - ldexp(1.0, -24), 127))
        return HUGE_VAL;
    int exponent;
    frexp(value, &exponent);
    const int step = exponent < -125 ? -149 : exponent - 24;
    const double units = ldexp(value, -step);
    double rounded = floor(units);
    const double tail = units - rounded;
    if (tail > 0.5 || (tail == 0.5 && fmod(rounded, 2.0) != 0.0))
        rounded += 1.0;
    return (float)ldexp(rounded, step);
}
// SSE round-to-nearest. Each pair clears both source W components and loads both
// vectors before a destination store. Byte strides advance between pairs only.
inline void Normalise3DVectors(const C4DVector* src, C4DVector* dest, unsigned long count,
                              unsigned long src_stride_2, unsigned long dest_stride_2, float md)
{
    while (count) {
        const unsigned long n = count > 1 ? 2 : 1;
        C4DVector result[2];
        for (unsigned long i = 0; i < n; ++i) {
            C4DVector& v = const_cast<C4DVector&>(src[i]);
            v.W = 0.0f;
            const double xx = RoundLane((double)v.X * v.X);
            const double yy = RoundLane((double)v.Y * v.Y);
            const double zz = RoundLane((double)v.Z * v.Z);
            const double sum = RoundLane(RoundLane(xx + zz) + yy);
            float len = (float)RoundLane(sqrt(sum));
            float divisor = len > md ? len : md;
            result[i] = C4DVector(v.X / divisor, v.Y / divisor, v.Z / divisor, v.W / divisor);
        }
        for (unsigned long j = 0; j < n; ++j)
            dest[j] = result[j];
        count -= n;
        if (count) {
            src = reinterpret_cast<const C4DVector*>(reinterpret_cast<const char*>(src) + src_stride_2);
            dest = reinterpret_cast<C4DVector*>(reinterpret_cast<char*>(dest) + dest_stride_2);
        }
    }
}
}
#endif
#endif
