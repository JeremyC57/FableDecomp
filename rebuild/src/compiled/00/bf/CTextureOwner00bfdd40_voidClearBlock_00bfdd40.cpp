// CTextureOwner_00bfdd40::void ClearBlock() -- free the landscape block at +0x38 through the landscape memory manager (engine 0x01436EA8 -> +0x6b0, DeleteVertexBuffer 0x00BDA0D0) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDD40, length 48 bytes.
// Leaf: ?ClearBlock@CTextureOwner_00bfdd40@@QAEXXZ
// The chunk label is propagated; named by its effect. Shape: the block pushed before the manager is loaded, `ret`.
// Callee (NF269, 2026-11-22): 0x00BDA0D0 is ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z (Ego_r.pdb
//   009c2640 / FableWin.pdb 02cb7450; retail 0x00BDA0D0 queues on VertexBufferDisposalList +0x1C), was the stand-in FreeBlock(void*) on a
//   row-local model of the manager; now the retail branch of engine/CEngineLandscapeMemoryManager.h with the landed DeleteVertexBuffer opt-in.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
struct CTextureOwner_00bfdd40 {
    unsigned char _pad[0x38];
    CVertexBufferWin32* m_pBlock;                  // +0x38: a vertex buffer (freed through DeleteVertexBuffer 0x00BDA0D0)
    void ClearBlock();
};

void CTextureOwner_00bfdd40::ClearBlock()
{
    if (m_pBlock) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(m_pBlock);
        m_pBlock = 0;
    }
}