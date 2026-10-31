// TransformPointThroughMirroredFrame_00c1c330(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b) -- transform a point by matrix a, mirror a negative z, then transform the result by matrix b (descriptive name; the catalog GetHelperPointPositionStatic is disproved)
// Grade: DIFFER(192v192) -- 2 lines (was 20): the second transform written as named float locals with += (rx = R1.x*y; rx += R2.x*z; rx += R0.x*x) gives retail's term order; left: the result.y word is stored after the final fstp (retail before). Tried: += translation, aggregate result, user copy ctor (196), pointer store, result declared first: no change. Lane gap: store scheduling
// Same-TU sibling TransformHelperPoint_00c1c3f0(const C3DMatrix& matrix, const C3DVector& point) (matched) -- transform a helper point by a 4x3 matrix (rows +0/+0xc/+0x18, translation +0x24) and return it by value
// Earlier grade: IMPROVED(192v192) -- retry1: in one TU with the matched TransformHelperPoint_00c1c3f0 and a same-TU caller VC7.1 gives this static retail's convention (a in eax, point in ecx, b in edx, result pointer in esi, plain ret). The body with scalar x/y/z locals is the right length; left is the x87 stack order in the second transform (ours y on top after the mirror test: fld st(0) where retail has fld st(1), z on top) -- next: the order of the three scalar computations / the mirror test written on a member.
// Shape (retry1): parked as an "LTCG custom calling convention" gap (matrix in ecx, point in edx, result pointer in eax); as a
//   file-static with its same-TU caller VC7.1 /O2 picks that private convention itself (chunk 585/586/593 lever).
// Name: descriptive. The parked CPhysicsMeshObject::GetHelperPointPositionStatic is disproved: Ego_r.pdb declares that one as
//   `C3DVector (const CCharString&, long) const` (a by-name lookup, not a matrix transform) and reloc_bindings binds the
//   GetHelperPointPositionStatic family to 0x00A6C9C0; the only caller here is 0x00C1C573 inside the unnamed 0x00C1C460.
// Leaf: TransformPointThroughMirroredFrame_00c1c330
// Retail: this row Fable.exe (TLC, VC7.1) @ 0x00C1C330, 192 bytes, private convention (a eax, point ecx, b edx, result esi), `ret`;
//   the sibling @ 0x00C1C3F0, 110 bytes.
// Shape: each component is ((m[2][c]*p.z + m[1][c]*p.y) + m[0][c]*p.x) + t[c] into a 12-byte frame local, then the local
// is copied word by word to the result; `ret`.
struct C3DVector {
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

static __declspec(noinline) C3DVector TransformPointThroughMirroredFrame_00c1c330(const C3DMatrix& a, const C3DVector& point, const C3DMatrix& b)
{
    float x = a.Row[2].x * point.z + a.Row[1].x * point.y + a.Row[0].x * point.x + a.Translation.x;
    float y = a.Row[2].y * point.z + a.Row[0].y * point.x + a.Row[1].y * point.y + a.Translation.y;
    float z = a.Row[2].z * point.z + a.Row[0].z * point.x + a.Row[1].z * point.y + a.Translation.z;
    if (z < 0.0f)
        z *= -1.0f;
    C3DVector result;
    float rx = b.Row[1].x * y;
    rx += b.Row[2].x * z;
    rx += b.Row[0].x * x;
    result.x = rx + b.Translation.x;
    float ry = b.Row[1].y * y;
    ry += b.Row[0].y * x;
    ry += b.Row[2].y * z;
    result.y = ry + b.Translation.y;
    float rz = b.Row[1].z * y;
    rz += b.Row[0].z * x;
    rz += b.Row[2].z * z;
    result.z = rz + b.Translation.z;
    return result;
}
// Same-TU caller (0x00C1C573 / 0x00C1C590 in 0x00C1C460), present so the statics get retail's private conventions. Not graded.
void TransformHelperPoint_00c1c3f0_caller(C3DVector* out, const C3DMatrix& m, const C3DVector& p, const C3DMatrix& b, bool mirrored)
{
    if (mirrored)
        *out = TransformPointThroughMirroredFrame_00c1c330(m, p, b);
    else
        *out = TransformHelperPoint_00c1c3f0(m, p);
}
