// CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace(const C3DVector* direction, float width, float height) -- build the light-space basis for a shadow direction (a helper up axis picked from the dominant of |X| / |Y|, side = up x dir, up = side x dir, both normalised) and return it as a matrix whose columns are scaled by 2/width, 2/width and 2/height
// Grade: DIFFER(ours v retail) -- 10 lines: side.Y second product retail fld st(2); fmul [dir.Z], ours fld [dir.Z]; fmul st(3) (operand order is canonicalised: 4 source orders, Cross/operator^ make up worse) + the result-copy setup scheduling (best try/00b4b8b0_a.cpp)
// Leaf: ?CalcLocalWorldToBaseShadowSpace@CEngineShadowRenderer@@AAE?AVCMatrix3x4@@ABVC3DVector@@MM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4B8B0, length 432 bytes (body 0x1a5 + int3 padding), `ret 10h` (hidden result pointer).
// Callees: C3DVector::Normalise() @ 0x00A14440 (manifest "GetScaled", propagated; landed sources use it as Normalise).
// Layout: `this` unused; CMatrix3x4 is twelve floats E11..E43 (rows of 3), returned through the hidden pointer by a 12-dword copy
//   of the local matrix.
// Shape: the helper up axis lives in two x87 registers (a ternary on a two-float pair: 0/1 when |X| > |Y|, else 1/0, Z = 0);
//   the cross products are written per component; each matrix column is `component * scale`, the fourth row `0.0f * scale`.
#include <math.h>
#pragma intrinsic(fabs)

#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
struct C3DScale { float X, Y, Z; C3DScale(float x, float y, float z) : X(x), Y(y), Z(z) {} };
#include "engine/CMatrix3x4.h"

class CEngineShadowRenderer {
private:
    CMatrix3x4 CalcLocalWorldToBaseShadowSpace(const C3DVector& direction, float width, float height);
};

CMatrix3x4 CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace(const C3DVector& direction, float width, float height)
{
    C3DVector dir(direction);
    float upX, upY;
    if (fabs(dir.X) > fabs(dir.Y)) {
        upX = 0.0f;
        upY = 1.0f;
    } else {
        upX = 1.0f;
        upY = 0.0f;
    }
    const float upZ = 0.0f;
    C3DVector side;
    side.X = upY * dir.Z - upZ * dir.Y;
    side.Y = upZ * dir.X - upX * dir.Z;
    side.Z = upX * dir.Y - upY * dir.X;
    C3DVector up;
    up.X = side.Y * dir.Z - side.Z * dir.Y;
    up.Y = side.Z * dir.X - side.X * dir.Z;
    up.Z = side.X * dir.Y - side.Y * dir.X;
    up.AccurateNormalise();
    side.AccurateNormalise();
    C3DScale scale(2.0f / width, 2.0f / width, 2.0f / height);
    CMatrix3x4 m;
    m.E11 = up.X * scale.X;
    m.E21 = up.Y * scale.X;
    m.E31 = up.Z * scale.X;
    m.E41 = 0.0f * scale.X;
    m.E12 = side.X * scale.Y;
    m.E22 = side.Y * scale.Y;
    m.E32 = side.Z * scale.Y;
    m.E42 = 0.0f * scale.Y;
    m.E13 = dir.X * scale.Z;
    m.E23 = dir.Y * scale.Z;
    m.E33 = dir.Z * scale.Z;
    m.E43 = 0.0f * scale.Z;
    return m;
}
