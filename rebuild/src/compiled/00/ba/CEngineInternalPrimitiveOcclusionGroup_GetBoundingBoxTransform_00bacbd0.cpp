// CEngineInternalPrimitiveOcclusionGroup::GetBoundingBoxTransform(CMatrix3x4&) -- the matrix of the group's world-space bounding box (virtual slot 17 fills a 24-byte box local), false when the group has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACBD0, length 46 bytes (48 with padding).
// Callees: virtual slot 17 (GetBoundingBoxWorldSpace, the box local by reference), C3DBoundingBox::CalculateMatrix(CMatrix3x4*) @ 0x00ADEBC0 (on the local, the
// argument matrix). Shape: byte-identical to CEngineInternalPrimitiveGroup::GetBoundingBoxTransform @ 0x00BC5FC0 (landed): the 0x18-byte local is uninitialised;
// the success path is the `if` body (fall-through) and the `xor al,al` failure sits at the end, each releasing the frame.
struct CMatrix3x4 { float m_rows[12]; };
struct C3DBoundingBox {
    float Min[3]; float Max[3];
    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0
};

struct CEngineInternalPrimitiveVtable {           // the retail CEngineInternalPrimitiveBase vtable slots used here
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16();
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);      // slot 17 (+0x44)
};

struct CEngineInternalPrimitiveOcclusionGroup : CEngineInternalPrimitiveVtable {   // engine/CEngineInternalPrimitiveOcclusionGroup.h (PDB, 0x64 bytes); only the vtable is touched
    unsigned char _pad04[0x60];
    bool GetBoundingBoxTransform(CMatrix3x4& transform);
};

bool CEngineInternalPrimitiveOcclusionGroup::GetBoundingBoxTransform(CMatrix3x4& transform)
{
    C3DBoundingBox box;
    if (GetBoundingBoxWorldSpace(box)) {
        box.CalculateMatrix(&transform);
        return true;
    }
    return false;
}