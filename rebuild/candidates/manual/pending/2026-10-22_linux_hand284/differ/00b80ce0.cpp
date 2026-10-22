// CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& delta) const -- the transform taking the cloth from its current frame to its last one: the last transform times the inverse of the current transform
// Grade: DIFFER(687v682) -- the inline 3x3 inverse (same family as the parked 0x00B393D0): VC7.1 canonicalises the fld/fmul operand of the cofactor products independent of source operand order (all-swapped source compiles identically), shares c0*inv in a frame word (sub esp,0x34 vs 0x30); the Multiply(inverse copy, LastTransform) tail is the matched 0x00BCEBE0 shape
// Leaf: GetDeltaTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80CE0, length 683 bytes (`ret 4`, virtual
// ?GetDeltaTransform@CEngineClothSimulationMeshBase@@UBEXAAVCMatrix3x4@@@Z). Constant 1.0f 0x0122DED8.
// Layout: Ego_r.pdb CEngineClothSimulationMeshBase CurrentTransform +0x1c / LastTransform +0x4c; retail's ParticleSims
// CArray is 12 bytes (not the PDB's 16), so they sit at +0x18 / +0x48 -- modelled locally.
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void GetInverse(CMatrix3x4& out) const
    {
        float c0 = E22 * E33 - E32 * E23;
        float det = (E32 * E13 - E12 * E33) * E21 + (E12 * E23 - E22 * E13) * E31 + c0 * E11;
        float inv = 1.0f / det;
        out.E11 = c0 * inv;
        out.E21 = (E31 * E23 - E21 * E33) * inv;
        out.E31 = (E32 * E21 - E31 * E22) * inv;
        out.E12 = (E32 * E13 - E12 * E33) * inv;
        out.E22 = (E11 * E33 - E31 * E13) * inv;
        out.E32 = (E31 * E12 - E11 * E32) * inv;
        out.E13 = (E12 * E23 - E22 * E13) * inv;
        out.E23 = (E21 * E13 - E11 * E23) * inv;
        out.E33 = (E11 * E22 - E12 * E21) * inv;
        out.E41 = -(E41 * out.E11 + E42 * out.E21 + E43 * out.E31);
        out.E42 = -(E41 * out.E12 + E42 * out.E22 + E43 * out.E32);
        out.E43 = -(E41 * out.E13 + E42 * out.E23 + E43 * out.E33);
    }
    __forceinline void Multiply(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
        E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32 + b.E42;
        E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
    }
};

class CEngineClothSimulationMeshBase {
public:
    virtual void GetDeltaTransform(CMatrix3x4& delta) const;
    unsigned char _pad04[0x18 - 4];
    CMatrix3x4    CurrentTransform;                     // +0x18
    CMatrix3x4    LastTransform;                        // +0x48
};

void CEngineClothSimulationMeshBase::GetDeltaTransform(CMatrix3x4& delta) const
{
    CurrentTransform.GetInverse(delta);
    CMatrix3x4 inverse(delta);
    delta.Multiply(inverse, LastTransform);
}
