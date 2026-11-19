// TransformHelperPoint(const C3DMatrix& matrix, const C3DVector& point) -- transform a helper point by a 4x3 matrix (rows +0/+0xc/+0x18,
//   translation +0x24) and return it by value
// Leaf: ?TransformHelperPoint@@YA?AVC3DVector@@ABUC3DMatrix@@ABV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C3F0, length 110 bytes (+ int3 padding), private register convention (matrix in ecx,
//   point in edx, result pointer in eax), `ret`.
// Name: descriptive, suffix-free (hand3c batch 5; was TransformHelperPoint_00c1c3f0, the same spelling in the 0x00C1C330 TU). The parked
//   CPhysicsMeshObject::GetHelperPointPositionStatic is disproved: Ego_r.pdb declares that one as `C3DVector (const CCharString&, long)
//   const` (a by-name lookup, not a matrix transform), and reloc_bindings binds the GetHelperPointPositionStatic family to 0x00A6C9C0.
//   The only caller here is 0x00C1C573, inside the unnamed 0x00C1C460.
// Shape: as a file-static with its same-TU caller, VC7.1 /O2 picks the private convention itself (chunk 585/586/593 lever). Each
//   component is ((m[2][c]*p.z + m[1][c]*p.y) + m[0][c]*p.x) + t[c], into a 12-byte frame local that is copied word by word to the result.
// Types: the shared engine/C3DVector.h (X, Y, Z) with the libpk TU's opt-in FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23, the same
//   spelling as the 0x00C1C330 TU. C3DMatrix (three rows and a translation, 0x30 bytes) is a local POD; no shared header names it.
#define FABLE_RETAIL_C3DVECTOR_LIBPK_IMPLICIT_COPY_PASS23
#include "engine/C3DVector.h"                      // the engine vector (X, Y, Z), 12 bytes
struct C3DMatrix {
    C3DVector Row[3];                              // +0x00, +0x0c, +0x18
    C3DVector Translation;                         // +0x24
};
static __declspec(noinline) C3DVector TransformHelperPoint(const C3DMatrix& matrix, const C3DVector& point)
{
    C3DVector result;
    result.X = matrix.Row[2].X * point.Z + matrix.Row[1].X * point.Y + matrix.Row[0].X * point.X + matrix.Translation.X;
    result.Y = matrix.Row[2].Y * point.Z + matrix.Row[0].Y * point.X + matrix.Row[1].Y * point.Y + matrix.Translation.Y;
    result.Z = matrix.Row[2].Z * point.Z + matrix.Row[0].Z * point.X + matrix.Row[1].Z * point.Y + matrix.Translation.Z;
    return result;
}
// Same-TU caller (0x00C1C573 in 0x00C1C460), present so the static gets retail's private convention. Not graded.
void CallTransformHelperPoint(C3DVector* out, const C3DMatrix& m, const C3DVector& p) { *out = TransformHelperPoint(m, p); }
