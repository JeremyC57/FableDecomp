// CEngineInternalPrimitiveChanger::ClearAllChildPrimitives() -- erase every child handle, then recompute the render-layer mask from the current child (none left: zero) and, when it changed, re-register the primitive's position with the flags at +0x60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5C00, length 79 bytes.
// Callees: vector<CEnginePrimitiveHandle>::erase(first, last) @ 0x00440D70 (on the vector at +0x48), UpdatePosition @ 0x00B91840 (`__fastcall`: the primitive in
// ecx, the flags in edx; a sibling jump). Shape: the mask starts as `xor ecx,ecx`, the current-index path (`cmp eax,-1`, unsigned range test, null test) reads
// the child's mask at +0x30; `cmp [edi+30h],ecx ; je` skips the store and the call when unchanged.
// Fixed (rename1, 2026-10-24): CEnginePrimitiveHandle was modelled as a plain {long id; primitive*} pair; it is
//   polymorphic -- PDB (Ego_r) 8 bytes with the vptr slot at +0 and Data at +4, RTTI vtable 0x0122F598 (slot 0
//   0x0041C500). Same size and the primitive still at +4, so the bytes are unchanged; the handle vector comment said
//   VC6 Dinkumware, retail is STLport.
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

struct CEnginePrimitiveHandle {                    // 8 bytes: vptr + the primitive (PDB: _padding_ +0, Data +4; RTTI vtable 0x0122F598)
    virtual ~CEnginePrimitiveHandle();             // slot 0 (0x0041C500)
    CEngineInternalPrimitiveBase* m_pPrimitive;
};

struct CRetailHandleVector {                       // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70 (Vector_EraseRange)
    void clear() { erase(m_pFirst, m_pLast); }
};
void __fastcall UpdatePosition_00b91840(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00B91840

struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {   // no PDB layout: the handle vector at +0x48, the current index at +0x58, the flags at +0x60
    CRetailHandleVector Primitives;                // +0x48
    unsigned char       _pad54[4];
    long                m_currentIndex;            // +0x58 (-1 = none)
    unsigned char       _pad5c[4];
    unsigned long       m_flags;                   // +0x60
    unsigned long ComputeRenderLayerMask()
    {
        unsigned long mask = 0;
        long index = m_currentIndex;
        if (index != -1 && (unsigned long)index < Primitives.GetCount()) {
            CEngineInternalPrimitiveBase* child = Primitives.m_pFirst[index].m_pPrimitive;
            if (child)
                mask = child->RenderLayerMask;
        }
        return mask;
    }
    virtual void ClearAllChildPrimitives();
};

void CEngineInternalPrimitiveChanger::ClearAllChildPrimitives()
{
    Primitives.clear();
    unsigned long mask = ComputeRenderLayerMask();
    if (RenderLayerMask != mask) {
        RenderLayerMask = mask;
        UpdatePosition_00b91840(this, m_flags);
    }
}