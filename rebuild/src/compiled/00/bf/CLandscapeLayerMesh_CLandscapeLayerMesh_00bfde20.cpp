// CLandscapeLayerMesh::~CLandscapeLayerMesh() -- this class's vtable (0x012A835C), the index buffer at +0x34 queued for disposal (DeleteIndexBuffer 0x00BDA090) unless it is shared (+0x48), the foreground vertex buffer at +0x38 queued (DeleteVertexBuffer 0x00BDA0D0) and cleared, the owned object at +0x3c deleted through its virtual destructor, then the CMovableResource base's inline destructor stores its vtable (0x0129B860)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDE20, length 96 bytes.
// Leaf: ??1CLandscapeLayerMesh@@QAE@XZ
// Callee names (NF269, 2026-11-22; were the stand-ins AppendFreeEdgeNode(void*) / FreeBlock(void*)): Ego_r.pdb 009c25e0 / 009c2640 and
//   FableWin.pdb 02cb7350 / 02cb7450 ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z /
//   ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, the spellings the landed self rows define; respelled in
//   place in the row's existing local model (its row-local CMovableResource model blocks the header's retail branch: HEAD debt, port queued).
//   Members +0x34 / +0x38 / +0x48: names and types of the Ego_r layout (engine/_quarantine/CLandscapeLayerMesh.h: IndexBuffer,
//   ForegroundVertexBuffer, UsesSharedIndexBuffer); the HEAD names had vertex / index the wrong way round.
// Shape: a non-virtual destructor in a polymorphic class (the lander would otherwise pick the deleting `??_G`), `this` in esi, `ret`.
class CMovableResource;
class CIndexBuffer;
class CVertexBufferWin32;
class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0 (an empty `ret 8` in retail; the landed 0x00BDD8E0 model calls it too)
    void DeleteIndexBuffer(CIndexBuffer* buffer);          // 0x00BDA090
    void DeleteVertexBuffer(CVertexBufferWin32* buffer);   // 0x00BDA0D0
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
// Name-fix pass (2026-11-12, NF29): the engine global 0x01436EA8 is g_pLandscapeRenderer (descriptive, CEngineLandscapeRenderer*, MemoryManager
//   +0x6B0: engine/CEngineLandscapeRenderer.h), the spelling of the landed landscape rows; was SEngine_01436ea8 / g_pEngine_01436ea8.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
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
    CIndexBuffer*                      IndexBuffer;              // +0x34
    CVertexBufferWin32*                ForegroundVertexBuffer;   // +0x38
    CLandscapeLayerMeshOwned_00bfde20* m_pOwned;         // +0x3c
    unsigned char                      _pad40[8];
    bool                               UsesSharedIndexBuffer;    // +0x48: the index buffer belongs to someone else
    void OnPostRelocation();
    ~CLandscapeLayerMesh();
    void RelocateData(long reason, void* oldBase, long newBase, long size);
};

CLandscapeLayerMesh::~CLandscapeLayerMesh()
{
    if (IndexBuffer && !UsesSharedIndexBuffer)
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(IndexBuffer);
    if (ForegroundVertexBuffer) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(ForegroundVertexBuffer);
        ForegroundVertexBuffer = 0;
    }
    if (m_pOwned)
        delete m_pOwned;
}