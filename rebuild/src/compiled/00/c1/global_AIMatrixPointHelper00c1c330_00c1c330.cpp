// TransformPointThroughMirroredFrame(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b) -- transform a point by the
//   4x3 matrix a, mirror the result's z into the non-negative half-space, then transform it by the 4x3 matrix b and return it by value
//   (rows +0/+0xc/+0x18, translation +0x24)
// Leaf: ?TransformPointThroughMirroredFrame@@YA?AVC3DVector@@ABUC3DMatrix@@ABV1@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1C330, 192 bytes, private register convention (a in eax, point in ecx, b in edx, result
//   pointer in esi), plain `ret`. It sits in the audio wrapper TU next to the landed TransformHelperPoint_00c1c3f0 (0x00C1C3F0), and
//   its only caller is 0x00C1C590 in the unnamed 0x00C1C460, which also calls 0x00C1C3F0 at 0x00C1C573.
// Name: descriptive (no PDB covers this TU: Ego_r / FableWin / Ego_d hold neither the audio wrapper nor these statics). The sibling keeps its landed address-suffixed name; this one
//   says what it does. The manifest label "AIMatrixPointHelper" is propagated.
// Shape:
//   - As in the sibling's TU, both statics with a same-TU caller let VC7.1 /O2 choose retail's private convention (chunk 585/586/593
//     lever; not LTCG).
//   - The first transform is the sibling's expression text: its x87 term order (R2*z, R1*y, R0*x) matches as written.
//   - The mirror is `if (z < 0.0f) z *= -1.0f;` (fcom / fmul by -1.0f).
//   - The second transform accumulates in steps (`r = R1*y; r += ...; r = r + ... + T;`). VC7.1 keeps those step boundaries, and this
//     gives retail's term order (y first). One expression per component is reassociated to z first, whatever the written order or
//     parentheses.
//   - The closing `r = r + ... + T` (rather than two more `+=`) drops the stack-copy pseudo-op before the final fadd. That lets c2 issue
//     the result's y copy before the z fstp, as retail does (c2 trace tools/sched t20.py + sim.py: the tail 42/41/43/38/44/45 needs node 36 gone).
// Types: the shared engine/C3DVector.h (X, Y, Z) with the libpk TU's opt-in FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23. The default
//   user copy constructor costs a frame word and a spill, 196 bytes. C3DMatrix (three rows and a translation, 0x30 bytes) is the
//   sibling's local POD; no shared header names it.
#define FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#include "engine/C3DVector.h"                      // the engine vector (X, Y, Z), 12 bytes
struct C3DMatrix {
    C3DVector Row[3];                              // +0x00, +0x0c, +0x18
    C3DVector Translation;                         // +0x24
};
static __declspec(noinline) C3DVector TransformHelperPoint_00c1c3f0(const C3DMatrix& matrix, const C3DVector& point)
{
    C3DVector result;
    result.X = matrix.Row[2].X * point.Z + matrix.Row[1].X * point.Y + matrix.Row[0].X * point.X + matrix.Translation.X;
    result.Y = matrix.Row[2].Y * point.Z + matrix.Row[0].Y * point.X + matrix.Row[1].Y * point.Y + matrix.Translation.Y;
    result.Z = matrix.Row[2].Z * point.Z + matrix.Row[0].Z * point.X + matrix.Row[1].Z * point.Y + matrix.Translation.Z;
    return result;
}

static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].X * point.Z + a.Row[1].X * point.Y + a.Row[0].X * point.X + a.Translation.X;
    float y = a.Row[2].Y * point.Z + a.Row[0].Y * point.X + a.Row[1].Y * point.Y + a.Translation.Y;
    float z = a.Row[2].Z * point.Z + a.Row[0].Z * point.X + a.Row[1].Z * point.Y + a.Translation.Z;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    float rx = b.Row[1].X * y; rx += b.Row[2].X * z; rx = rx + b.Row[0].X * x + b.Translation.X;
    float ry = b.Row[1].Y * y; ry += b.Row[0].Y * x; ry = ry + b.Row[2].Y * z + b.Translation.Y;
    float rz = b.Row[1].Z * y; rz += b.Row[0].Z * x; rz = rz + b.Row[2].Z * z + b.Translation.Z;
    result.X = rx; result.Y = ry; result.Z = rz;
    return result;
}
// Same-TU caller (0x00C1C573 / 0x00C1C590 in 0x00C1C460), present so the statics get retail's private conventions. Not graded.
void TransformHelperPoint_00c1c3f0_caller(C3DVector* out, const C3DMatrix& m, const C3DVector& p, const C3DMatrix& b, bool mirrored)
{
    if (mirrored)
        *out = TransformPointThroughMirroredFrame(m, p, b);
    else
        *out = TransformHelperPoint_00c1c3f0(m, p);
}