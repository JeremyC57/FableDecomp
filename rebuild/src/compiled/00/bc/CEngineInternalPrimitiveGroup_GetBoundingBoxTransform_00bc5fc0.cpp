// CEngineInternalPrimitiveGroup::GetBoundingBoxTransform(CMatrix3x4&) -- the matrix of the group's world-space bounding box (virtual slot 17 fills a 24-byte box local), false when the group has none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5FC0, length 46 bytes.
// Callees: virtual slot 17 (GetBoundingBoxWorldSpace, the box local by reference), C3DBoundingBox::CalculateMatrix(CMatrix3x4*) @ 0x00ADEBC0 (on the local, the
// argument matrix). Shape: the 0x18-byte local is uninitialised; the success path is the `if` body (fall-through) and the `xor al,al` failure sits at the end, each releasing the frame.
class CEngineInternalPrimitiveBase;
class CMatrix3x4 { public: float m_rows[12]; };
class C3DBoundingBox { public:
    float Min[3]; float Max[3];
    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0
};

struct CEngineInternalPrimitiveVtable {           // the retail CEngineInternalPrimitiveBase vtable slots used by the group/changer/viewport bodies
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void PostRender();                                       // slot 14 (+0x38)
    virtual void v15(); virtual void v16();
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);      // slot 17 (+0x44)
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void SetAnimation(void* animation);                      // slot 23 (+0x5c)
    virtual void SetPosition(void* position);                        // slot 24 (+0x60)
    virtual void v25(); virtual void v26();
    virtual CEngineInternalPrimitiveVtable* GetAsOcclusionPrimitive();   // slot 27 (+0x6c)
    virtual void ResetOcclusionInfo(long layer);                     // slot 28 (+0x70)
};

class CEngineInternalPrimitiveBase : public CEngineInternalPrimitiveVtable { public:   // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes)
    unsigned char  _pad04[0x2c];                   // +0x04 .. +0x30
    unsigned long  RenderLayerMask;                // +0x30
    unsigned char  PostRenderEnable;               // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char  _pad35[0x13];                   // +0x35 .. +0x48
};

class CEnginePrimitiveHandle { public:                    // 8 bytes: the handle id and the primitive
    long                          m_id;
    CEngineInternalPrimitiveBase* m_pPrimitive;
};

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70 (Vector_EraseRange)
    void clear() { erase(m_pFirst, m_pLast); }
};
void __fastcall UpdatePosition_00b91840(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00B91840

struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase {   // engine/CEngineInternalPrimitiveGroup.h: Primitives +0x50 (retail: three words), PrimitiveFlags +0x5c (PDB +0x60)
    unsigned char       _pad48[8];
    CRetailHandleVector Primitives;                // +0x50
    unsigned long       PrimitiveFlags;            // +0x5c
    bool GetBoundingBoxTransform(CMatrix3x4& transform);
};

bool CEngineInternalPrimitiveGroup::GetBoundingBoxTransform(CMatrix3x4& transform)
{
    C3DBoundingBox box;
    if (GetBoundingBoxWorldSpace(box)) {
        box.CalculateMatrix(&transform);
        return true;
    }
    return false;
}