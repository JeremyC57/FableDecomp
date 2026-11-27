// BuildViewMatrix(matrix, forward, up, target, position) -- camera basis: right = up x forward, columns right/up/forward, translation = -dot(position), then SetViewMatrix
// Leaf: BuildViewMatrix
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BF00, 174 bytes, `ret 8`. No PDB symbol in Ego_r.pdb, FableWin.pdb or Ego_d.
//   libpk (0xC08AE0..0xC40000), no RTTI. EXTRA_RULES: where no PDB name exists, one descriptive name, marked descriptive.
//   BuildViewMatrix is a descriptive name, not a PDB name. The catalog label C3DMeshObject_UpdateQuadricMatrix is propagated.
// Callees: C3DVector::CrossProduct 0x00C25860, C3DVector::DotProduct 0x00C256A0 (class key, the landed
//   ?DotProduct@C3DVector@@QBEMABV1@@Z), and InvertMatrix3x4 0x00C1D1C0
//   (?InvertMatrix3x4@@YIXPAVCMatrix3x4@@ABV1@@Z). C3DVector is a class so those symbols match the
//   landed rows; a struct key bound a second spelling.
// Shape: a same-TU caller passes the matrix, forward and up by address, so VC7.1 keeps matrix in esi, up in edi
//   and forward in ebx, with target and position on the stack (`ret 8`). __stdcall is what makes the callee pop
//   those 8 bytes; without it the body is `ret` (172 bytes). forward is declared before up so up wins edi.
//   SetIdentity stores the nine zeros, then m[0], m[8] and m[4]. Parentheses around each DotProduct keep the
//   last fchs until target is in ecx and the matrix is in edx. A static caller is discarded and the convention
//   collapses, so the caller has external linkage. C3DVector stays a local declaration-only model: the shared
//   header's Dot is inline, and retail calls 0x00C256A0.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector CrossProduct(const C3DVector& other) const;   // 0x00C25860
    float DotProduct(const C3DVector& other) const;         // 0x00C256A0
};

struct CMatrix34_00c1bf00 {
    float m[12];
    CMatrix34_00c1bf00() { SetIdentity(); }
    void SetIdentity()
    {
        m[7] = 0.0f; m[6] = 0.0f; m[5] = 0.0f; m[3] = 0.0f; m[2] = 0.0f; m[1] = 0.0f;
        m[11] = 0.0f; m[10] = 0.0f; m[9] = 0.0f;
        m[0] = 1.0f; m[8] = 1.0f; m[4] = 1.0f;
    }
};

class CMatrix3x4;
void __fastcall InvertMatrix3x4(CMatrix3x4* out, const CMatrix3x4& m);   // 0x00C1D1C0

static __declspec(noinline) void __stdcall BuildViewMatrix(CMatrix34_00c1bf00* matrix, const C3DVector* forward,
                                                           const C3DVector* up, void* target, const C3DVector* position)
{
    C3DVector right = up->CrossProduct(*forward);
    matrix->SetIdentity();
    matrix->m[0] = right.X; matrix->m[1] = up->X; matrix->m[2] = forward->X;
    matrix->m[3] = right.Y; matrix->m[4] = up->Y; matrix->m[5] = forward->Y;
    matrix->m[6] = right.Z; matrix->m[7] = up->Z; matrix->m[8] = forward->Z;
    matrix->m[9] = -(right.DotProduct(*position));
    matrix->m[10] = -(up->DotProduct(*position));
    matrix->m[11] = -(forward->DotProduct(*position));
    InvertMatrix3x4(reinterpret_cast<CMatrix3x4*>(target), *reinterpret_cast<const CMatrix3x4*>(matrix));
}

// Same-TU caller. External linkage on purpose: a static caller is discarded and BuildViewMatrix then ends in `ret`.
void BuildViewMatrixCaller(void* target, const C3DVector* position, C3DVector up, C3DVector forward)
{
    CMatrix34_00c1bf00 matrix;
    BuildViewMatrix(&matrix, &forward, &up, target, position);
}