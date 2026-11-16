// CEngineScreenEffectGlowRenderer::ReleaseGlowTextures() -- hand the four glow texture handles back to the engine's texture pool, then null the four texture pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5EC90, length 77 bytes.
// Callees: the texture pool's release @ 0x00F36F00 (`__fastcall void (long)`, an empty `ret 4` in the
// PC build; the manifest's "$E2" label is a misattributed initialiser name), four times. Globals:
// g_pEngine @ 0x01436E98 (the CEngine context, "DAT_01436e98" in the auto-names); the pool is the
// sub-object at +0x3190 of it, addressed once into a named local.
// Layout = Ego_r.pdb (engine/CEngineScreenEffectGlowRenderer.h, a flat POD -> `_Retail` model):
// GlowTexture256/64/64Alt/16 pointers +0x10..+0x1c, their handles +0x20..+0x2c. Called last from
// CEngineScreenEffectGlowRenderer::Render @ 0x00B619B0; the manifest's
// "CCombatAbilityData::TransferBinaryOut" label is propagated. The four null stores are sunk after
// the calls (one zero register).
struct CTexture_Calls;

// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngine_Retail / g_pEngine_01436e98 / CTexturePool_Retail and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

struct CEngineScreenEffectGlowRenderer_Retail {
    unsigned char   _pad[0x10];
    CTexture_Calls* GlowTexture256;                         // +0x10
    CTexture_Calls* GlowTexture64;                          // +0x14
    CTexture_Calls* GlowTexture64Alt;                       // +0x18
    CTexture_Calls* GlowTexture16;                          // +0x1c
    long            GlowTexture256Handle;                   // +0x20
    long            GlowTexture64Handle;                    // +0x24
    long            GlowTexture64AltHandle;                 // +0x28
    long            GlowTexture16Handle;                    // +0x2c
    void ReleaseGlowTextures();
};

void CEngineScreenEffectGlowRenderer_Retail::ReleaseGlowTextures()
{
    CGPUScratchBufferTextureManager* pool = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    pool->EndTextureUse(GlowTexture256Handle);
    pool->EndTextureUse(GlowTexture64Handle);
    pool->EndTextureUse(GlowTexture64AltHandle);
    pool->EndTextureUse(GlowTexture16Handle);
    GlowTexture256 = 0;
    GlowTexture64 = 0;
    GlowTexture64Alt = 0;
    GlowTexture16 = 0;
}