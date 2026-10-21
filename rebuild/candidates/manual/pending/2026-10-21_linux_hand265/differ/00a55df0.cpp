// CMatrix3x4::operator*(const CMatrix3x4& b) const -- return this * b: the 3x3 rotation rows times b's rotation, the translation row times b's rotation plus b's translation
// Grade: DIFFER(415v415) -- scheduling only: `mov ecx,0xc` (the result copy count) sits one instruction earlier than retail inside the E32 sum
// Leaf: ??DCMatrix3x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55DF0, length 416 bytes; hidden-pointer result, `ret 8`.
// Layout: CMatrix3x4 (PDB, 0x30 bytes): E11..E13, E21..E23, E31..E33, E41..E43 (row 4 is the translation), as in the landed
//   PostMultiply 0x00C1CF20.
// Shape: the result is a copy of *this multiplied in place through an inline operator*= that snapshots its left operand -- two implicit
//   (rep movsd) copies of *this, the products written in k order (VC7.1 reorders each sum by operand address), then the implicit copy of
//   the local into the hidden result.
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a = *this;
        E11 = a.E13 * b.E31 + a.E12 * b.E21 + a.E11 * b.E11;
        E12 = a.E12 * b.E22 + a.E11 * b.E12 + a.E13 * b.E32;
        E13 = a.E12 * b.E23 + a.E11 * b.E13 + a.E13 * b.E33;
        E21 = a.E23 * b.E31 + a.E22 * b.E21 + a.E21 * b.E11;
        E22 = a.E22 * b.E22 + a.E21 * b.E12 + a.E23 * b.E32;
        E23 = a.E22 * b.E23 + a.E21 * b.E13 + a.E23 * b.E33;
        E31 = a.E33 * b.E31 + a.E32 * b.E21 + a.E31 * b.E11;
        E32 = a.E33 * b.E32 + (a.E31 * b.E12 + a.E32 * b.E22);
        E33 = a.E32 * b.E23 + a.E31 * b.E13 + a.E33 * b.E33;
        E41 = a.E43 * b.E31 + a.E42 * b.E21 + a.E41 * b.E11 + b.E41;
        E42 = a.E43 * b.E32 + (a.E41 * b.E12 + a.E42 * b.E22) + b.E42;
        E43 = a.E42 * b.E23 + a.E41 * b.E13 + a.E43 * b.E33 + b.E43;
        return *this;
    }
    CMatrix3x4 operator*(const CMatrix3x4& b) const;
};

CMatrix3x4 CMatrix3x4::operator*(const CMatrix3x4& b) const
{
    CMatrix3x4 result = *this;
    result *= b;
    return result;
}
