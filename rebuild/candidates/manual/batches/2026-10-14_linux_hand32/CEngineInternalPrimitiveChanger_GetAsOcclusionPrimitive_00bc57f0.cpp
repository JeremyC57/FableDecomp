// CEngineInternalPrimitiveChanger::GetAsOcclusionPrimitive() -- the current child's occlusion primitive (virtual slot 27), or null when there is no current child
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC57F0, length 42 bytes.
// Callees: virtual slot 27 (GetAsOcclusionPrimitive) of the child (a sibling jump with the child in ecx). Shape: `cmp eax,-1`, the unsigned range test against
// `(last - first) / 8`, the null test, then the jump; the three failure paths share `xor eax,eax`.
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

struct CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {   // no PDB layout: the handle vector at +0x48, the current index at +0x58, the flags at +0x60
    CRetailHandleVector Primitives;                // +0x48
    unsigned char       _pad54[4];
    long                m_currentIndex;            // +0x58 (-1 = none)
    unsigned char       _pad5c[4];
    unsigned long       m_flags;                   // +0x60
    virtual CEngineInternalPrimitiveVtable* GetAsOcclusionPrimitive();
};

CEngineInternalPrimitiveVtable* CEngineInternalPrimitiveChanger::GetAsOcclusionPrimitive()
{
    long index = m_currentIndex;
    if (index != -1 && (unsigned long)index < Primitives.GetCount()) {
        CEngineInternalPrimitiveBase* child = Primitives.m_pFirst[index].m_pPrimitive;
        if (child)
            return child->GetAsOcclusionPrimitive();
    }
    return 0;
}
