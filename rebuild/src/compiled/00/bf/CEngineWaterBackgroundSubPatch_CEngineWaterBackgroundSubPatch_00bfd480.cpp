// CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch() -- when the sub-patch owns its buffers, hand every index buffer back to the landscape memory manager's free edge list and free every vertex block (main pair first, then the four bridge pairs); then the CMovableResource base destructor
// Leaf: ??1CEngineWaterBackgroundSubPatch@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD480, length 144 bytes.
// Callees: the landscape memory manager (engine global @ 0x01436EA8 -> +0x6b0, re-read for every call) DeleteIndexBuffer
// @ 0x00BDA090 (index buffers) and DeleteVertexBuffer @ 0x00BDA0D0 (vertex buffers), as in the landed ~CLandscapeLayerMesh @ 0x00BFDE20.
// Callee names (NF269, 2026-11-22; were the stand-ins AppendFreeEdgeNode(void*) / FreeBlock(void*)): Ego_r.pdb 009c25e0 / 009c2640 and
//   FableWin.pdb 02cb7350 / 02cb7450 ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z /
//   ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, the spellings the landed self rows define.
// Identity (NF279, 2026-11-22; nf277 ruling 4): the virtual destructor ??1CEngineWaterBackgroundSubPatch@@UAE@XZ (Ego_r.pdb 00a4bba0 + FableWin.pdb
//   02e05150); its only caller, ??_GCEngineWaterBackgroundSubPatch 0x00BFDC90 (vtable 0x012A8334 slot 0), already binds that spelling. Was
//   ??1CEngineWaterBackgroundSubPatch@@QAE@XZ on a row-local CMovableResource (placeholder virtual v00, non-virtual destructor). Now on the shared
//   retail engine/CMovableResource.h (virtual destructor), engine/CEngineWaterBackgroundSubPatch.h (PDB layout; its VIRTUALS opt-in declares the
//   destructor and the two private overrides) and the retail branch of engine/CEngineLandscapeMemoryManager.h (the landed Delete<X> opt-ins),
//   the 0x00BFDE20 shape.
// TU: the destructor is virtual, so this TU also emits ??_7CEngineWaterBackgroundSubPatch@@6B@ and ??_GCEngineWaterBackgroundSubPatch@@UAEPAXI@Z
//   (pick-any; retail 0x00BFDC90: ??1 called, then the class delete) and the CMovableResource vtable / ??_G. The class delete is FableWin.pdb
//   ??3CEngineWaterBackgroundSubPatch@@SIXPAX@Z (02e05130; static __fastcall; no out-of-line Ego_r copy, so inline in retail): FreeVirtual
//   0x00BD9B90, defined inline below.
// Shape: the bridge loop is a `dec ebx ; jne` count with one pointer (`[esi-10h]` reaches the index array); `this` in edi, `ret`.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER    // DeleteIndexBuffer 0x00BDA090 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22          // the class on its retail base CMovableResource
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS        // the virtual destructor, the two private overrides, the class delete
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

inline void FABLE_FASTCALL CEngineWaterBackgroundSubPatch::operator delete(void* block)
{
    NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(block);
}

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
