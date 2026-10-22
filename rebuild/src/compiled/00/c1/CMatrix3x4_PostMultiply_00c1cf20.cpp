// CMatrix3x4::PostMultiply(const CMatrix3x4& b) -- this = this * b: the 3x3 rotation rows times b's rotation, the translation row times b's rotation plus b's translation
// Leaf: PostMultiply
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1CF20, length 398 bytes (padded to 400); no direct callers found.
// Identity: the catalog label "CMatrix3x4::operator*" is wrong. The body is in-place (`this` in ecx is both the left operand
//   and the destination, the right operand arrives in edx, plain `ret`): FableWin.pdb's `?PostMultiply@CMatrix3x4@@QAEXABV1@@Z`,
//   compiled __fastcall in the TLC math library (neighbours 0xC1C900.. RotateYaw/Atan2/Matrix_Invert are the same module).
// Layout: CMatrix3x4 (PDB, 0x30 bytes): E11..E13, E21..E23, E31..E33, E41..E43 (row 4 is the translation).
// Shape: `CMatrix3x4 a(*this)` through a memberwise user copy constructor -- VC7.1 keeps a.E11..E13 on the x87 stack and
//   copies the other nine dwords into the frame (an implicit copy is a `rep movsd` block); each element is the sum written
//   in k order and VC7.1 emits it as (k3 + k2) + k1, then + the translation term for row 4.
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;

    CMatrix3x4() {}
    CMatrix3x4(const CMatrix3x4& o)
        : E11(o.E11), E12(o.E12), E13(o.E13), E21(o.E21), E22(o.E22), E23(o.E23),
          E31(o.E31), E32(o.E32), E33(o.E33), E41(o.E41), E42(o.E42), E43(o.E43) {}

    void __fastcall PostMultiply(const CMatrix3x4& matrix);
};

void __fastcall CMatrix3x4::PostMultiply(const CMatrix3x4& matrix)
{
    CMatrix3x4 a(*this);
    E11 = a.E11 * matrix.E11 + a.E12 * matrix.E21 + a.E13 * matrix.E31;
    E12 = a.E11 * matrix.E12 + a.E12 * matrix.E22 + a.E13 * matrix.E32;
    E13 = a.E11 * matrix.E13 + a.E12 * matrix.E23 + a.E13 * matrix.E33;
    E21 = a.E21 * matrix.E11 + a.E22 * matrix.E21 + a.E23 * matrix.E31;
    E22 = a.E21 * matrix.E12 + a.E22 * matrix.E22 + a.E23 * matrix.E32;
    E23 = a.E21 * matrix.E13 + a.E22 * matrix.E23 + a.E23 * matrix.E33;
    E31 = a.E31 * matrix.E11 + a.E32 * matrix.E21 + a.E33 * matrix.E31;
    E32 = a.E31 * matrix.E12 + a.E32 * matrix.E22 + a.E33 * matrix.E32;
    E33 = a.E31 * matrix.E13 + a.E32 * matrix.E23 + a.E33 * matrix.E33;
    E41 = a.E41 * matrix.E11 + a.E42 * matrix.E21 + a.E43 * matrix.E31 + matrix.E41;
    E42 = a.E41 * matrix.E12 + a.E42 * matrix.E22 + a.E43 * matrix.E32 + matrix.E42;
    E43 = a.E41 * matrix.E13 + a.E42 * matrix.E23 + a.E43 * matrix.E33 + matrix.E43;
}