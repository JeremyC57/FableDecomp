// CLandscapeLayerMesh::OnPostRelocation() -- after this mesh moved, re-register it as the owner of its two landscape-memory blocks: the vertex block at +0x34 (only when it is not shared, byte +0x48) and the index block at +0x38, each through the engine's landscape memory manager (0x01436EA8 -> +0x6b0, 0x00BD9AB0 with (block, this))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDD70, length 64 bytes.
// Callees: 0x00BD9AB0 (manager member, `ret 8`). Shape: `this` in esi, the manager re-read from the engine global for each call, `ret`.
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

void CLandscapeLayerMesh::OnPostRelocation()
{
    if (m_pVertexBlock && !m_vertexBlockShared)
        g_pLandscapeRenderer->MemoryManager->RebindBlockOwner(m_pVertexBlock, this);
    if (m_pIndexBlock)
        g_pLandscapeRenderer->MemoryManager->RebindBlockOwner(m_pIndexBlock, this);
}