// CLandscapeLayerMesh::~CLandscapeLayerMesh() -- this class's vtable (0x012A835C), the vertex block at +0x34 handed back to the landscape memory manager's free edge list (0x00BDA090) unless it is shared (+0x48), the index block at +0x38 freed (0x00BDA0D0) and cleared, the owned object at +0x3c deleted through its virtual destructor, then the CMovableResource base's inline destructor stores its vtable (0x0129B860)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDE20, length 96 bytes.
// Leaf: ??1CLandscapeLayerMesh@@QAE@XZ
// Shape: a non-virtual destructor in a polymorphic class (the lander would otherwise pick the deleting `??_G`), `this` in esi, `ret`.
class CMovableResource;
class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0 (an empty `ret 8` in retail; the landed 0x00BDD8E0 model calls it too)
    void AppendFreeEdgeNode(void* block);          // 0x00BDA090
    void FreeBlock(void* block);                   // 0x00BDA0D0
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8 (descriptive)
class CMovableResource { public:                          // vtable 0x0129B860
    virtual void v00();
    CMovableResource() { }
    ~CMovableResource() { }                        // inline: the vtable store
};
struct CLandscapeLayerMeshOwned_00bfde20 {         // the object at +0x3c: deleted through its virtual destructor (slot 0 with 1)
    virtual ~CLandscapeLayerMeshOwned_00bfde20();
};
class CLandscapeLayerMesh : public CMovableResource { public:    // vtable 0x012A835C
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
        g_pLandscapeRenderer->MemoryManager->AppendFreeEdgeNode(m_pVertexBlock);
    if (m_pIndexBlock) {
        g_pLandscapeRenderer->MemoryManager->FreeBlock(m_pIndexBlock);
        m_pIndexBlock = 0;
    }
    if (m_pOwned)
        delete m_pOwned;
}