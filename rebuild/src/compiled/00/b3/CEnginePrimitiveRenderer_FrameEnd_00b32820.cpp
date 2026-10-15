// CEnginePrimitiveRenderer::FrameEnd(const CEngineComponentParams&) -- end-of-frame pass: every scene's FrameEnd, slot 4 on each non-sorted render entry, a reset on every scene, then clears a frame flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32820, length 158 bytes (+ int3 padding).
// Callees: 0x00B94A30 CEngineScene::FrameEnd() (per scene), virtual slot 4 (+0x10) of each non-sorted entry's
// object, 0x00B93DF0 the scene's per-frame list reset (labelled CSkeletalMorphResourceManager::UnloadBoneConfigs,
// propagated; it clears the scene's render lists). The parameter block is unused (`ret 4`).
// Layout: the renderer derives from a 0x268-byte primary base (vtable 0x012A1380) and a secondary engine-component
// base at +0x268 (vtable 0x012A1348, FrameEnd = its slot 2): FrameEnd is first declared by that secondary base, so
// VC compiles the override with `this` at the subobject and every member offset is relative to +0x268 -- the
// non-sorted entry vector at +0x274 (+0xc; the landed RenderNonsortedPrimitives 0x00B32610 reads it from the
// primary `this`), the scene list at +0x338 (+0xd0; the PDB's SceneList at +0x33c with retail's 12-byte CArray),
// and the frame flag in the primary base at +0x98 (-0x1d0). The engine header carries the PDB offsets of the
// derived members only (it has no base split), so the class is modelled locally.
// Shape: the first loop walks the scene list with an iterator, re-reading `end` after every call; the second and
// third loops are indexed (`GetCount()` re-evaluated after every call: `sub ; sar`), the third with an aligned
// loop head (speed TU); the element is read through the array's first pointer each time.
// Leaf: ?FrameEnd@CEnginePrimitiveRenderer@@UAEXABVCEngineComponentParams@@@Z

class CEngineComponentParams;
struct CEngineScene {
    void FrameEnd();                                      // 0x00B94A30
    void ResetRenderLists();                              // 0x00B93DF0
};
struct CRenderable {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void EndFrame();                              // slot 4 (+0x10)
};
struct SRenderEntry { CRenderable* m_pObject; long m_key; };   // 8 bytes

struct CRetailEntryVector {                               // retail 12-byte vector: first, last, end
    SRenderEntry* m_pFirst;
    SRenderEntry* m_pLast;
    SRenderEntry* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};
struct CSceneList {                                       // retail CArray<CEngineScene*> (12 bytes)
    CEngineScene** m_pFirst;
    CEngineScene** m_pLast;
    CEngineScene** m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
};

struct CEnginePrimitiveRendererBase {                     // primary base, vtable 0x012A1380 (0x268 bytes)
    virtual void v00();
    unsigned char _pad04[0x94];
    bool          m_frameFlag;                            // +0x98
    unsigned char _pad99[0x268 - 0x99];
};
struct CEngineComponent {                                 // secondary base at +0x268, vtable 0x012A1348
    virtual void FrameStartHook();
    virtual void v01();
    virtual void FrameEnd(const CEngineComponentParams& params);   // slot 2
};

struct CEnginePrimitiveRenderer : CEnginePrimitiveRendererBase, CEngineComponent {
    unsigned char      _pad26c[0x8];
    CRetailEntryVector m_nonsortedPrimitives;             // +0x274
    unsigned char      _pad280[0x338 - 0x280];
    CSceneList         SceneList;                         // +0x338
    virtual void FrameEnd(const CEngineComponentParams& params);
};

void CEnginePrimitiveRenderer::FrameEnd(const CEngineComponentParams& params)
{
    for (CEngineScene** scene = SceneList.m_pFirst; scene != SceneList.m_pLast; ++scene)
        (*scene)->FrameEnd();
    for (unsigned long i = 0; i < m_nonsortedPrimitives.GetCount(); ++i)
        m_nonsortedPrimitives.m_pFirst[i].m_pObject->EndFrame();
    for (unsigned long j = 0; j < SceneList.GetCount(); ++j)
        SceneList.m_pFirst[j]->ResetRenderLists();
    m_frameFlag = false;
}