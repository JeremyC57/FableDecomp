// CMatrix3x4::operator*(float scale) -- return a copy of the matrix with every element multiplied by scale
// Leaf: ??DCMatrix3x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075B6C0, length 187 bytes, `ret 8` (hidden result, scale).
// The manifest label "CEnvironmentSpellEffectInterface::SetRadialBlurPositionParams" is propagated: the body is the
// PDB's `??DCMatrix3x4@@QAE?AV0@M@Z` (CMatrix3x4 CMatrix3x4::operator*(float)), `this` is the 48-byte matrix.
// Callees: none (PostScale is inlined; it needs __forceinline, the plain in-class inline is emitted out of line).
// Layout: CMatrix3x4 (PDB, 0x30 bytes): E11..E13, E21..E23, E31..E33, E41..E43.
// Shape: the copy `CMatrix3x4 result(*this)` is the `rep movsd` into the frame, the inline PostScale scales the
// first column of all four rows, then the second, then the third (E11,E21,E31,E41,E12,...), and the result is block
// copied into the hidden return slot (VC7.1 has no NRVO).

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    __forceinline void PostScale(float s)
    {
        E11 *= s; E21 *= s; E31 *= s; E41 *= s;
        E12 *= s; E22 *= s; E32 *= s; E42 *= s;
        E13 *= s; E23 *= s; E33 *= s; E43 *= s;
    }
    CMatrix3x4 operator*(float s);
};

CMatrix3x4 CMatrix3x4::operator*(float s)
{
    CMatrix3x4 result(*this);
    result.PostScale(s);
    return result;
}