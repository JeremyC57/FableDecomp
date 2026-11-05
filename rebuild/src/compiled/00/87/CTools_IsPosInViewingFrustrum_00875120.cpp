// CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov) -- 1 when the direction eye->pos lies inside the cone of half-angle fov/2 (fov as a fraction of a turn) around eye->lookAt
// Leaf: IsPosInViewingFrustrum
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875120, length 164 bytes (`ret 10h`, `this` unused; 176 with padding).
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (x2). Constants: 0x0122F59C = 0.5f, 0x0124F2B8 = (double)6.2831855f.
// Shape: the two differences are inline C3DVector operator- results built straight in their locals (user
// constructor); the dot product is written y, x, z so VC7.1's right-operand-first FPU order gives retail's z, x, y
// loads; `cos` is the intrinsic over `fov * 0.5f * (double)TWO_PI`; the result is a long (`mov eax,1`).
#include <math.h>
#pragma intrinsic(cos)
#include "engine/C3DVector.h"
static const float TWO_PI = 6.28318548f;

struct CTools {
    long IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov);
};

long CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov)
{
    C3DVector view = lookAt - eye;
    view.AccurateNormalise();
    C3DVector dir = pos - eye;
    dir.AccurateNormalise();
    if (view.Y * dir.Y + view.X * dir.X + view.Z * dir.Z > cos(fov * 0.5f * (double)TWO_PI))
        return 1;
    return 0;
}