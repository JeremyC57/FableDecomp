// CLocalDetailPrimitiveMeshZSpriteBatch::OnPostRelocation() -- after this batch moved, re-register it as the owner of its two landscape-memory blocks (+0x3c then +0x38) through the engine's landscape memory manager (0x01436EA8 -> +0x6b0, 0x00BD9AB0 with (block, this))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9B90, length 48 bytes.
// Leaf: OnPostRelocation
// Callees: 0x00BD9AB0 (manager member, `ret 8`). The chunk label (vfunc_7) is the slot; the body mirrors CLandscapeLayerMesh::OnPostRelocation @ 0x00BFDD70. Shape: the manager in edi, `this` in esi, `ret`.
struct CMovableResource;
struct CEngineLandscapeMemoryManager {
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0 (an empty `ret 8` in retail; the landed 0x00BDD8E0 model calls it too)
    void AppendFreeEdgeNode(void* block);          // 0x00BDA090
    void FreeBlock(void* block);                   // 0x00BDA0D0
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
};
struct CBatchRecord_0122f598 : CBase {             // vtable 0x0122F598
    virtual void v00();
    long m_4;
    CBatchRecord_0122f598() : CBase(), m_4(0) { }
};
struct __declspec(novtable) CMovableResource {    // the base's vtable store is folded into the derived one
    virtual void v00();
    CMovableResource() { }
};
struct __declspec(novtable) CLocalDetailPrimitiveMeshBase_00bfac70 : CMovableResource {   // the intermediate base: its inline constructor runs before the derived vtable store
    long                  m_4;                     // +0x04
    unsigned char         _pad08[0x18];
    long                  m_20;                    // +0x20
    long                  m_24;                    // +0x24
    long                  m_28;                    // +0x28
    long                  m_2c;                    // +0x2c
    void*                 m_pOwner;                // +0x30
    void*                 m_pType;                 // +0x34
    CLocalDetailPrimitiveMeshBase_00bfac70(void* owner, void* type) : CMovableResource(), m_4(0), m_20(0), m_24(0), m_28(0), m_2c(0), m_pOwner(owner), m_pType(type) { }
};
struct CLocalDetailPrimitiveMeshZSpriteBatch : CLocalDetailPrimitiveMeshBase_00bfac70 {   // vtable 0x012A82D8
    virtual void v00();
    void*                 m_pVertexBlock;          // +0x38: a landscape-memory block
    void*                 m_pIndexBlock;           // +0x3c: a landscape-memory block
    long                  m_40;
    bool                  m_44;                    // +0x44
    CBatchRecord_0122f598 m_record;                // +0x48
    CLocalDetailPrimitiveMeshZSpriteBatch(void* owner, void* type);
    void OnPostRelocation();
};

void CLocalDetailPrimitiveMeshZSpriteBatch::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
    manager->RebindBlockOwner(m_pIndexBlock, this);
    manager->RebindBlockOwner(m_pVertexBlock, this);
}