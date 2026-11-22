// CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch() -- when the sub-patch owns its buffers, hand every index buffer back to the landscape memory manager's free edge list and free every vertex block (main pair first, then the four bridge pairs); then the CMovableResource base destructor
// Leaf: ??1CEngineWaterBackgroundSubPatch@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD480, length 144 bytes.
// Callees: the landscape memory manager (engine global @ 0x01436EA8 -> +0x6b0, re-read for every call) DeleteIndexBuffer
// @ 0x00BDA090 (index buffers) and DeleteVertexBuffer @ 0x00BDA0D0 (vertex buffers), as in the landed ~CLandscapeLayerMesh @ 0x00BFDE20.
// Callee names (NF269, 2026-11-22; were the stand-ins AppendFreeEdgeNode(void*) / FreeBlock(void*)): Ego_r.pdb 009c25e0 / 009c2640 and
//   FableWin.pdb 02cb7350 / 02cb7450 ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z /
//   ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, the spellings the landed self rows define; respelled in
//   place in the row's existing local model (its row-local CMovableResource model blocks the header's retail branch: HEAD debt, port queued).
// Vtables: 0x012A8334 (this class) on entry, 0x0129B860 (CMovableResource, its inline destructor) on exit.
// Layout: the PDB member layout of engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +0x08, VertexBuffer +0x0c,
// BridgeIndexBuffers +0x10, BridgeVertexBuffers +0x20, DeletableBuffers +0x44) -- modelled locally because the header's
// layout struct carries its vptr as a data word and cannot take the vtable stores.
// Shape: a non-virtual destructor in a polymorphic class (keeps the deleting `??_G` out of the TU); the bridge loop is a
// `dec ebx ; jne` count with one pointer (`[esi-10h]` reaches the index array).
class CIndexBuffer;
class CVertexBufferWin32;
class CEngineLandscapeMap;

class CEngineLandscapeMemoryManager { public:
    void DeleteIndexBuffer(CIndexBuffer* buffer);          // 0x00BDA090
    void DeleteVertexBuffer(CVertexBufferWin32* buffer);   // 0x00BDA0D0
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

class CEngineWaterBackgroundSubPatch : public CMovableResource { public:   // vtable 0x012A8334
    CEngineLandscapeMap* Map;                      // +0x04
    CIndexBuffer*        IndexBuffer;              // +0x08
    CVertexBufferWin32*  VertexBuffer;             // +0x0c
    CIndexBuffer*        BridgeIndexBuffers[4];    // +0x10
    CVertexBufferWin32*  BridgeVertexBuffers[4];   // +0x20
    unsigned char        _pad30[0x14];
    bool                 DeletableBuffers;         // +0x44
    virtual void v00();
    ~CEngineWaterBackgroundSubPatch();
};

CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch()
{
    if (DeletableBuffers) {
        if (IndexBuffer)
            NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(IndexBuffer);
        if (VertexBuffer)
            NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(VertexBuffer);
        for (long i = 0; i < 4; ++i) {
            if (BridgeIndexBuffers[i])
                NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteIndexBuffer(BridgeIndexBuffers[i]);
            if (BridgeVertexBuffers[i])
                NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(BridgeVertexBuffers[i]);
        }
    }
}