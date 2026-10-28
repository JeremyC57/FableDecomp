// CMatrix3x4::PreMultiply(const CMatrix3x4& matrix) -- this = matrix * this: each rotation row of `matrix` times this rotation, and matrix's translation row times this rotation plus this translation
// Leaf: ?PreMultiply@CMatrix3x4@@QAIXABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1CDA0, 380 bytes (padded to 384), `this` in ecx, the matrix in edx, plain `ret`.
// Identity (probable): FableWin.pdb ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z, the mirror of the landed CMatrix3x4::PostMultiply
//   0x00C1CF20 that immediately follows it (same module as GetRotationAngles 0x00C1CC10 / RotateA/B 0x00C1C900/0x00C1C990):
//   the body is in place with `this` as the right operand (E11 = m.E11*a.E11 + m.E12*a.E21 + m.E13*a.E31, translation adds
//   a.E41..E43). The catalog label "CEngineClothSimulationMeshBase::ConvertVectorToLocal" is propagated and wrong.
// Convention: the TLC math library passes the right operand in edx (no stack argument, plain `ret`); modelled `__fastcall`
//   like the landed PostMultiply, so the compiled symbol is the QAI spelling of the PDB's QAE name (see NOTES, Review at landing).
// Shape: PostMultiply's model -- `CMatrix3x4 a(*this)` through a memberwise user copy constructor (E11/E12/E21/E31 stay on the
//   x87 stack, the rest go to the frame); each element written in k order.
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

    void __fastcall PreMultiply(const CMatrix3x4& matrix);
};

void __fastcall CMatrix3x4::PreMultiply(const CMatrix3x4& matrix)
{
    CMatrix3x4 a(*this);
    E11 = matrix.E11 * a.E11 + matrix.E12 * a.E21 + matrix.E13 * a.E31;
    E12 = matrix.E11 * a.E12 + matrix.E12 * a.E22 + matrix.E13 * a.E32;
    E13 = matrix.E11 * a.E13 + matrix.E12 * a.E23 + matrix.E13 * a.E33;
    E21 = matrix.E21 * a.E11 + matrix.E22 * a.E21 + matrix.E23 * a.E31;
    E22 = matrix.E21 * a.E12 + matrix.E22 * a.E22 + matrix.E23 * a.E32;
    E23 = matrix.E21 * a.E13 + matrix.E22 * a.E23 + matrix.E23 * a.E33;
    E31 = matrix.E31 * a.E11 + matrix.E32 * a.E21 + matrix.E33 * a.E31;
    E32 = matrix.E31 * a.E12 + matrix.E32 * a.E22 + matrix.E33 * a.E32;
    E33 = matrix.E31 * a.E13 + matrix.E32 * a.E23 + matrix.E33 * a.E33;
    E41 = matrix.E41 * a.E11 + matrix.E42 * a.E21 + matrix.E43 * a.E31 + a.E41;
    E42 = matrix.E41 * a.E12 + matrix.E42 * a.E22 + matrix.E43 * a.E32 + a.E42;
    E43 = matrix.E41 * a.E13 + matrix.E42 * a.E23 + matrix.E43 * a.E33 + a.E43;
}