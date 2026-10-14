// CLandscapeLayerMesh::~CLandscapeLayerMesh() -- this class's vtable (0x012A835C), the vertex block at +0x34 handed back to the landscape memory manager's free edge list (0x00BDA090) unless it is shared (+0x48), the index block at +0x38 freed (0x00BDA0D0) and cleared, the owned object at +0x3c deleted through its virtual destructor, then the CMovableResource base's inline destructor stores its vtable (0x0129B860)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDE20, length 96 bytes.
// Leaf: ??1CLandscapeLayerMesh@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class (the lander would otherwise pick the deleting `??_G`), `this` in esi, `ret`.
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
struct CMovableResource {                          // vtable 0x0129B860
    virtual void v00();
    CMovableResource() { }
    ~CMovableResource() { }                        // inline: the vtable store
};
struct CLandscapeLayerMeshOwned_00bfde20 {         // the object at +0x3c: deleted through its virtual destructor (slot 0 with 1)
    virtual ~CLandscapeLayerMeshOwned_00bfde20();
};
struct CLandscapeLayerMesh : CMovableResource {    // vtable 0x012A835C
    unsigned char                      _pad04[0x30];
    void*                              m_pVertexBlock;   // +0x34: a landscape-memory block (an edge-list node when +0x48 is clear)
    void*                              m_pIndexBlock;    // +0x38: a landscape-memory block
    CLandscapeLayerMeshOwned_00bfde20* m_pOwned;         // +0x3c
    unsigned char                      _pad40[8];
    bool                               m_vertexBlockShared;   // +0x48: the vertex block belongs to someone else
    void OnPostRelocation();
    ~CLandscapeLayerMesh();
    void RelocateData(long reason, void* oldBase, long newBase, long size);
};

CLandscapeLayerMesh::~CLandscapeLayerMesh()
{
    if (m_pVertexBlock && !m_vertexBlockShared)
        g_pEngine_01436ea8->m_pLandscapeMemory->AppendFreeEdgeNode(m_pVertexBlock);
    if (m_pIndexBlock) {
        g_pEngine_01436ea8->m_pLandscapeMemory->FreeBlock(m_pIndexBlock);
        m_pIndexBlock = 0;
    }
    if (m_pOwned)
        delete m_pOwned;
}