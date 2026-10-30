// TransformHelperPoint_00c1c3f0(const C3DMatrix& matrix, const C3DVector& point) -- transform a helper point by a 4x3 matrix (rows +0/+0xc/+0x18, translation +0x24) and return it by value
// Shape (retry1): parked as an "LTCG custom calling convention" gap (matrix in ecx, point in edx, result pointer in eax); as a
//   file-static with its same-TU caller VC7.1 /O2 picks that private convention itself (chunk 585/586/593 lever).
// Name: descriptive. The parked CPhysicsMeshObject::GetHelperPointPositionStatic is disproved: Ego_r.pdb declares that one as
//   `C3DVector (const CCharString&, long) const` (a by-name lookup, not a matrix transform) and reloc_bindings binds the
//   GetHelperPointPositionStatic family to 0x00A6C9C0; the only caller here is 0x00C1C573 inside the unnamed 0x00C1C460.
// Leaf: ?TransformHelperPoint_00c1c3f0@@YA?AVC3DVector@@ABUC3DMatrix@@ABU1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C3F0, length 110 bytes (+ int3 padding).
// Shape: each component is ((m[2][c]*p.z + m[1][c]*p.y) + m[0][c]*p.x) + t[c] into a 12-byte frame local, then the local
// is copied word by word to the result; `ret`.
class C3DVector { public:
    float x, y, z;
};
struct C3DMatrix {
    C3DVector Row[3];                              // +0x00, +0x0c, +0x18
    C3DVector Translation;                         // +0x24
};
static __declspec(noinline) C3DVector TransformHelperPoint_00c1c3f0(const C3DMatrix& matrix, const C3DVector& point)
{
    C3DVector result;
    result.x = matrix.Row[2].x * point.z + matrix.Row[1].x * point.y + matrix.Row[0].x * point.x + matrix.Translation.x;
    result.y = matrix.Row[2].y * point.z + matrix.Row[0].y * point.x + matrix.Row[1].y * point.y + matrix.Translation.y;
    result.z = matrix.Row[2].z * point.z + matrix.Row[0].z * point.x + matrix.Row[1].z * point.y + matrix.Translation.z;
    return result;
}

// Same-TU caller (0x00C1C573 in 0x00C1C460), present so the static gets retail's private convention. Not graded.
void TransformHelperPoint_00c1c3f0_caller(C3DVector* out, const C3DMatrix& m, const C3DVector& p) { *out = TransformHelperPoint_00c1c3f0(m, p); }