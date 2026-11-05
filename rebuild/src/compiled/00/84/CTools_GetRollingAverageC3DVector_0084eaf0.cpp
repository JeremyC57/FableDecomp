// CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float) -- folds a new sample into a rolling average: (average * count + value) / (count + 1), returned by value
// Leaf: GetRollingAverage
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084EAF0, length 101 bytes (`ret 10h`: hidden result, two vector
// references, the float count; `this` unused). The C3DVector twin of the landed C2DVector instantiation 0x0069BF30.
// Constant: 0x0122DED8 = 1.0f (twice: `count + 1.0f` and the reciprocal `1.0f / (count + 1.0f)`).
// Shape: inline C3DVector operators with a user constructor (hidden-pointer result); `operator/` multiplies by one
// reciprocal.
#include "engine/C3DVector.h"

struct CTools {
    template <class T> T GetRollingAverage(const T& average, const T& value, float count);
};

template <class T>
T CTools::GetRollingAverage(const T& average, const T& value, float count)
{
    return (average * count + value) / (count + 1.0f);
}

template C3DVector CTools::GetRollingAverage<C3DVector>(const C3DVector&, const C3DVector&, float);