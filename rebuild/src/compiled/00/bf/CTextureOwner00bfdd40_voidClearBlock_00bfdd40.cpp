// CTextureOwner_00bfdd40::void ClearBlock() -- free the landscape block at +0x38 through the landscape memory manager (engine 0x01436EA8 -> +0x6b0, 0x00BDA0D0) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDD40, length 48 bytes.
// The chunk label is propagated; named by its effect. Shape: the block pushed before the manager is loaded, `ret`.
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
    void FreeBlock(void* block);                   // 0x00BDA0D0 (labelled "LandscapeLayerList::InsertNode")
};
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8 (descriptive; NF28: was a stand-in struct and the address-named pointer)
struct CTextureOwner_00bfdd40 {
    unsigned char _pad[0x38];
    void*         m_pBlock;                        // +0x38
    void ClearBlock();
};

void CTextureOwner_00bfdd40::ClearBlock()
{
    if (m_pBlock) {
        g_pLandscapeRenderer->MemoryManager->FreeBlock(m_pBlock);
        m_pBlock = 0;
    }
}