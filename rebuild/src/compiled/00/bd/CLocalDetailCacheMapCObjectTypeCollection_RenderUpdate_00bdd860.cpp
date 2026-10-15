// CLocalDetailCacheMap::CObjectTypeCollection::RenderUpdate(const CEngineCamera&, float, float, unsigned long, float) -- if this object type is enabled and still visible at the near distance, let every active cached primitive of it update for rendering, telling them whether the far distance is still inside the fade
// Leaf: ?RenderUpdate@CObjectTypeCollection
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDD860, 127 bytes (+ int3 padding; `ret 14h`).
// Label correction: the catalog says CObjectCacheGroupCollection::RenderUpdate; that function is the caller at 0x00BDD930, which walks
//   its ObjectTypeCollections list (+0x20) and calls this per collection. The PDB signature
//   `?RenderUpdate@CObjectTypeCollection@CLocalDetailCacheMap@@QAEXABVCEngineCamera@@MMKM@Z` matches the five arguments.
// Callees: CLocalDetailObjectCollectionType::PeekFadeEnd() @ 0x00BE2740 (twice), the primitive's virtual slots 6 (+0x18, active test)
//   and 4 (+0x10, render update).
// Layout (retail): object type +4, first primitive +8 (linked through +4), enabled flag +0x14.
// Shape: `if (fadeEnd > near)` is `test ah,41h ; jne` to the exit; the second compare result is a named bool homed in the dead
//   `near` slot (`mov byte [esp+0Ch],1 ... mov byte [esp+0Ch],0`) and pushed as a dword in the loop.
struct CEngineCamera;
struct CLocalDetailObjectCollectionType {
    float PeekFadeEnd();                                    // 0x00BE2740
};
struct CLocalDetailPrimitiveBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void RenderUpdate(const CEngineCamera& camera, bool insideFade, unsigned long frame,
                              CLocalDetailObjectCollectionType* type, float time);   // slot 4 (+0x10)
    virtual void v05();
    virtual bool IsActive();                                // slot 6 (+0x18)
    CLocalDetailPrimitiveBase* m_pNext;                     // +0x04
};
struct CLocalDetailCacheMap {
    struct CObjectTypeCollection {
        void*                             __vftable;        // +0x00
        CLocalDetailObjectCollectionType* m_pType;          // +0x04
        CLocalDetailPrimitiveBase*        m_pFirstPrimitive;// +0x08
        unsigned char                     _pad0c[0x08];
        bool                              m_bEnabled;       // +0x14
        void RenderUpdate(const CEngineCamera& camera, float nearDistance, float farDistance, unsigned long frame, float time);
    };
};

void CLocalDetailCacheMap::CObjectTypeCollection::RenderUpdate(const CEngineCamera& camera, float nearDistance, float farDistance,
                                                               unsigned long frame, float time)
{
    if (m_bEnabled && m_pType->PeekFadeEnd() > nearDistance) {
        bool insideFade = m_pType->PeekFadeEnd() > farDistance;
        for (CLocalDetailPrimitiveBase* primitive = m_pFirstPrimitive; primitive; primitive = primitive->m_pNext) {
            if (primitive->IsActive())
                primitive->RenderUpdate(camera, insideFade, frame, m_pType, time);
        }
    }
}