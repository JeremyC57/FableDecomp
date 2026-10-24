// _global::BuildViewMatrix(CMatrix34_00c1bf00* matrix, const C3DVector* up, const C3DVector* forward, void* target, const C3DVector* position) -- builds the view matrix of a camera basis: right = up x forward, the three basis vectors as the matrix columns, the negated dot products with `position` as the translation, then hands it to 0x00C1D1C0 for `target`
// Grade: DIFFER(ours v 174) -- LTCG register arguments (lane gap): retail takes the matrix in esi, `up` in edi and `forward` in ebx (none set or saved; the caller 0x00C1C4DD loads all three with lea) plus two stack arguments (`ret 8`); this reference body passes them in ecx/edx/stack. One grade, no dummy parameters.
// Leaf: BuildViewMatrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BF00, 174 bytes, no EH.
// Callees: C3DVector::CrossProduct @ 0x00C25860 (hidden result), C3DVector::DotProduct @ 0x00C256A0, the matrix
//   hand-off @ 0x00C1D1C0 (ecx target, edx matrix).
// Shape: the matrix's identity constructor is inlined first (its dead zero / 1.0 stores survive the column
//   writes because the vectors may alias the matrix), then the nine column stores interleaved, then the three
//   negated dot products.
// Label correction: the catalog's `C3DMeshObject_UpdateQuadricMatrix` is propagated; descriptive name.
inline void* operator new(unsigned int, void* where) { return where; }

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector CrossProduct(const C3DVector& other) const;   // 0x00C25860
    float DotProduct(const C3DVector& other) const;         // 0x00C256A0
};

struct CMatrix34_00c1bf00 {
    float m[12];                           // 3x3 basis (columns: right, up, forward) then translation
    CMatrix34_00c1bf00()
    {
        m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f;
        m[3] = 0.0f; m[4] = 1.0f; m[5] = 0.0f;
        m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;
        m[9] = 0.0f; m[10] = 0.0f; m[11] = 0.0f;
    }
};

void __fastcall SetViewMatrix(void* target, const CMatrix34_00c1bf00* matrix);   // 0x00C1D1C0

void __fastcall BuildViewMatrix(CMatrix34_00c1bf00* matrix, const C3DVector* up, const C3DVector* forward,
                                void* target, const C3DVector* position)
{
    C3DVector right = up->CrossProduct(*forward);
    new (matrix) CMatrix34_00c1bf00;
    matrix->m[0] = right.X; matrix->m[1] = up->X; matrix->m[2] = forward->X;
    matrix->m[3] = right.Y; matrix->m[4] = up->Y; matrix->m[5] = forward->Y;
    matrix->m[6] = right.Z; matrix->m[7] = up->Z; matrix->m[8] = forward->Z;
    matrix->m[9] = -right.DotProduct(*position);
    matrix->m[10] = -up->DotProduct(*position);
    matrix->m[11] = -forward->DotProduct(*position);
    SetViewMatrix(target, matrix);
}
