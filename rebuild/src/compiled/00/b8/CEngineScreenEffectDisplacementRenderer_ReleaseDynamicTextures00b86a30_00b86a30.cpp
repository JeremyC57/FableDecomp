// CEngineScreenEffectDisplacementRenderer::ReleaseDynamicTextures_00b86a30() -- clear the handle at +0xc when held; when the texture state at +0x10 is set release the texture at +0x14 to the engine's texture pool (+0x3190 of the global) and clear the state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86A30, length 54 bytes.
// Callees: the pool release @ 0x00F36F00 (on the global @ 0x01436E98 + 0x3190; an empty `ret 4` in retail). Shape: the pool address is formed first (`add ecx,3190h`)
// and kept in ecx across both tests; the two clears are immediates. The chunk label "FinishWithDynamicTextures" is propagated; named by effect.
// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngineGlobal_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98


class CEngineScreenEffectDisplacementRenderer { public:
    unsigned char _pad[0xc];
    void*         m_pHandle;                       // +0x0c
    long          m_textureState;                  // +0x10
    long           m_pTexture;                      // +0x14 (scratch texture handle)
    void ReleaseDynamicTextures_00b86a30();
};

void CEngineScreenEffectDisplacementRenderer::ReleaseDynamicTextures_00b86a30()
{
    CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    if (m_pHandle)
        m_pHandle = 0;
    if (m_textureState) {
        pool->EndTextureUse(m_pTexture);
        m_textureState = 0;
    }
}