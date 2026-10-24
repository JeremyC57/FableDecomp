// CMatrix3x4::operator+(const CMatrix3x4& other) -- return the element-wise sum of this matrix and other
// Leaf: ??HCMatrix3x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075B600, 178 bytes, `ret 8` (hidden result, other). No direct caller
//   or reference (an unreferenced out-of-line copy).
// Identity: Ego_r.pdb names `??HCMatrix3x4@@QAE?AV0@ABV0@@Z` (CMatrix3x4 CMatrix3x4::operator+(const CMatrix3x4&));
//   `this` is the 48-byte matrix, the twin of the landed operator*(float) 0x0075B6C0 right after it. The catalog's
//   "swap<CDebugSphere>" is propagated.
// Layout: CMatrix3x4 (PDB, 0x30 bytes): E11..E13, E21..E23, E31..E33, E41..E43.
// Shape: `CMatrix3x4 result(*this)` is the `rep movsd` into the frame, the inline element-wise add runs in memory
//   order (`fld [res] ; fadd [other] ; fstp [res]` x12), and the result is block copied into the hidden return slot
//   (VC7.1 has no NRVO).
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    __forceinline void Add(const CMatrix3x4& o)
    {
        E11 += o.E11; E12 += o.E12; E13 += o.E13;
        E21 += o.E21; E22 += o.E22; E23 += o.E23;
        E31 += o.E31; E32 += o.E32; E33 += o.E33;
        E41 += o.E41; E42 += o.E42; E43 += o.E43;
    }
    CMatrix3x4 operator+(const CMatrix3x4& other);
};

CMatrix3x4 CMatrix3x4::operator+(const CMatrix3x4& other)
{
    CMatrix3x4 result(*this);
    result.Add(other);
    return result;
}