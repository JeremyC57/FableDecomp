// CPhysicsMeshObject::GetHelperPointPositionStatic(const C3DMatrix& matrix, const C3DVector& point) -- transform a helper point by a 4x3 matrix (rows +0/+0xc/+0x18, translation +0x24) and return it by value
// Grade: DIFFER(116v110) -- the arithmetic and the copy-out match; only the hidden result pointer differs: ours loads it from the stack (`mov eax,[esp+10h]`, `ret 4`), retail takes the matrix in ecx, the point in edx and the RESULT pointer in eax (its only caller 0x00C1C573 also passes esi-based locals): a custom register convention produced by whole-program optimisation (/GL + /LTCG) for an internal static function; a single-TU cl.exe build cannot emit it (lane gap: LTCG custom calling convention); tried: __fastcall static returning C3DVector by value (one try) | retry 7 (Fable): not retried (register swap / LTCG custom convention / FP chain reassociation / pre-frame load -- known lane gaps, no untried source shape).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C3F0, length 110 bytes (+ int3 padding).
// Leaf: GetHelperPointPositionStatic
// Shape: each component is ((m[2][c]*p.z + m[1][c]*p.y) + m[0][c]*p.x) + t[c] into a 12-byte frame local, then the local
// is copied word by word to the result; `ret`.
struct C3DVector {
    float x, y, z;
};
struct C3DMatrix {
    C3DVector Row[3];                              // +0x00, +0x0c, +0x18
    C3DVector Translation;                         // +0x24
};
struct CPhysicsMeshObject {
    static C3DVector __fastcall GetHelperPointPositionStatic(const C3DMatrix& matrix, const C3DVector& point);
};

C3DVector __fastcall CPhysicsMeshObject::GetHelperPointPositionStatic(const C3DMatrix& matrix, const C3DVector& point)
{
    C3DVector result;
    result.x = matrix.Row[2].x * point.z + matrix.Row[1].x * point.y + matrix.Row[0].x * point.x + matrix.Translation.x;
    result.y = matrix.Row[2].y * point.z + matrix.Row[0].y * point.x + matrix.Row[1].y * point.y + matrix.Translation.y;
    result.z = matrix.Row[2].z * point.z + matrix.Row[0].z * point.x + matrix.Row[1].z * point.y + matrix.Translation.z;
    return result;
}
