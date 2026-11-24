// CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures() -- when the textures are held (+0xc): release both (+0x14 and +0x18) to the engine's texture pool (+0x3190 of the global) and clear the two held flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BD30, length 62 bytes.
// Callees: the pool release @ 0x00F36F00 twice (on the global @ 0x01436E98 + 0x3190, kept in edi; edi is saved only inside the branch). Shape: the first texture is
// loaded before the pool address is formed; the two clears are immediates. The chunk label "FinishWithDynamicTextures" is propagated; named by effect.
// Name (NF378): was the made-up ReleaseTextures_00b5bd30; now the PDB name ?FinishWithDynamicTextures@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ (private):
//   BSim 0.9999 onto that Ego_r symbol (in both the Ego_r.pdb and FableWin.pdb name sets) + Ego_r object membership
//   (Engine_Screen_Effect_Radial_Blur_Renderer.obj, 6 same-object landed neighbours; retail VA order agrees for 7 pairs, 1 does not).
// Leaf: ?FinishWithDynamicTextures@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
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


class CEngineScreenEffectRadialBlurRenderer { public:
    unsigned char _pad[0xc];
    long          m_held;                          // +0x0c
    long          m_heldB;                         // +0x10
    long          m_pTextureA;                     // +0x14 (scratch texture handle)
    long          m_pTextureB;                     // +0x18 (scratch texture handle)
private:
    void FinishWithDynamicTextures();
};

void CEngineScreenEffectRadialBlurRenderer::FinishWithDynamicTextures()
{
    if (m_held) {
        CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
        pool->EndTextureUse(m_pTextureA);
        pool->EndTextureUse(m_pTextureB);
        m_held = 0;
        m_heldB = 0;
    }
}