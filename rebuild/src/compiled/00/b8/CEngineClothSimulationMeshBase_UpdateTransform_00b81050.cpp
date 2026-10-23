// CEngineClothSimulationMeshBase::UpdateTransform() -- fetch the owner primitive's last and current transforms, then rebuild InverseTransform as the inverse of the current 3x3 rotation (cofactors over the determinant) with a zero translation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81050, length 304 bytes (+ int3 padding).
// Callees: virtual slots 30 (+0x78) and 31 (+0x7c) of the owner primitive at +0xb0 (fill a CMatrix3x4: the last and the
// current world transform).
// Layout: as the landed cloth-mesh relatives (ConvertVectorToLocal 0x00B80AD0, the constructor 0x00B81AD0): the retail
// CArray is 12 bytes, so CurrentTransform +0x18, LastTransform +0x48, InverseTransform +0x78, Owner +0xb0 (the PDB
// offsets sit 4 bytes higher; no engine header for the class).
// Shape: the first cofactor is a named local (kept on the x87 stack and reused for E11), the determinant's other two
// terms are recomputed for the result rows; `1.0f / det` is `fdivr [1.0f]`; the zero translation is integer stores.
// `fld [a] ; fmul [b]` is `b * a` (the right operand is loaded first).
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline void GetInverseRotation(CMatrix3x4& out) const
    {
        float det = E11 * (E22 * E33 - E23 * E32) + E21 * (E32 * E13 - E33 * E12) + E31 * (E23 * E12 - E22 * E13);
        float inv = 1.0f / det;
        out.E11 = (E22 * E33 - E23 * E32) * inv;
        out.E21 = (E23 * E31 - E21 * E33) * inv;
        out.E31 = (E21 * E32 - E22 * E31) * inv;
        out.E12 = (E32 * E13 - E33 * E12) * inv;
        out.E22 = (E33 * E11 - E13 * E31) * inv;
        out.E32 = (E31 * E12 - E32 * E11) * inv;
        out.E13 = (E23 * E12 - E22 * E13) * inv;
        out.E23 = (E21 * E13 - E23 * E11) * inv;
        out.E33 = (E22 * E11 - E21 * E12) * inv;
        out.E41 = 0.0f;
        out.E42 = 0.0f;
        out.E43 = 0.0f;
    }
};

struct CEngineInternalPrimitiveMeshBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void GetLastTransform(CMatrix3x4& out);       // slot 30 (+0x78)
    virtual void GetTransform(CMatrix3x4& out);           // slot 31 (+0x7c)
};

struct CEngineClothSimulationMeshBase {
    unsigned char                     _pad00[0x18];  // vptr, reference count/id, ParticleSims (12-byte CArray)
    CMatrix3x4                        CurrentTransform;   // +0x18
    CMatrix3x4                        LastTransform;      // +0x48
    CMatrix3x4                        InverseTransform;   // +0x78
    unsigned char                     _pad_a8[8];         // MeshInfo, PrimitiveIndex
    CEngineInternalPrimitiveMeshBase* Owner;              // +0xb0
    void UpdateTransform();
};

void CEngineClothSimulationMeshBase::UpdateTransform()
{
    float u0 = 0.0f;
    Owner->GetLastTransform(LastTransform);
    Owner->GetTransform(CurrentTransform);
    CurrentTransform.GetInverseRotation(InverseTransform);
}