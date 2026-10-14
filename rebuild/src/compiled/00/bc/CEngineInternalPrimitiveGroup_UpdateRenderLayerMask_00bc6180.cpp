// CEngineInternalPrimitiveGroup::UpdateRenderLayerMask() -- OR the live children's render-layer masks into a local and, when it differs from the group's, store it and re-register the position with the group's primitive flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC6180, length 53 bytes.
// Callees: UpdatePosition @ 0x00B91840 (`__fastcall`: the primitive in ecx, the flags at +0x5c in edx; a sibling jump). Shape: the end pointer is read once
// (the mask is a local, so nothing aliases it) and kept in edi; `cmp [ecx+30h],esi ; je` skips the store and the call. The chunk label
// "ClearAllChildPrimitives" belongs to the sibling @ 0x00BC6580, which inlines this after clearing the vector.
struct CEngineInternalPrimitiveBase;
struct C3DBoundingBox { float Min[3]; float Max[3]; };

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

struct CEngineInternalPrimitiveBase : CEngineInternalPrimitiveVtable {   // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes)
    unsigned char  _pad04[0x2c];                   // +0x04 .. +0x30
    unsigned long  RenderLayerMask;                // +0x30
    unsigned char  PostRenderEnable;               // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char  _pad35[0x13];                   // +0x35 .. +0x48
};

struct CEnginePrimitiveHandle {                    // 8 bytes: the handle id and the primitive
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
    void UpdateRenderLayerMask();
};

void CEngineInternalPrimitiveGroup::UpdateRenderLayerMask()
{
    unsigned long mask = 0;
    for (CEnginePrimitiveHandle* h = Primitives.m_pFirst; h != Primitives.m_pLast; ++h) {
        if (h->m_pPrimitive)
            mask |= h->m_pPrimitive->RenderLayerMask;
    }
    if (RenderLayerMask != mask) {
        RenderLayerMask = mask;
        UpdatePosition_00b91840(this, PrimitiveFlags);
    }
}