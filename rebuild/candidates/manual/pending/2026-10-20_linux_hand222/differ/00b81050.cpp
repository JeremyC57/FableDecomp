// Grade: DIFFER(297v297) -- x87 operand order only: in eight cofactor products the lane loads E12 (+4) / E31 (+0x18) first where retail loads the other factor first; VC7.1 canonicalises commutative fmul order (swapping the source operands changes nothing)
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
    Owner->GetLastTransform(LastTransform);
    Owner->GetTransform(CurrentTransform);
    const CMatrix3x4& m = CurrentTransform;
    float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
    float inv = 1.0f / det;
    InverseTransform.E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
    InverseTransform.E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
    InverseTransform.E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
    InverseTransform.E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
    InverseTransform.E22 = (m.E11 * m.E33 - m.E13 * m.E31) * inv;
    InverseTransform.E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
    InverseTransform.E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
    InverseTransform.E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
    InverseTransform.E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
    InverseTransform.E41 = 0.0f;
    InverseTransform.E42 = 0.0f;
    InverseTransform.E43 = 0.0f;
}
