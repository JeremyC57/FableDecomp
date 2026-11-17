// CEngineScreenEffectRadialBlurRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- enable the radial blur when the quality settings ask for it and the hardware supports it (and its textures could be allocated); otherwise free its dynamic texture page and disable it. Returns false only when the textures could not be allocated
// Leaf: ?SetEffectQuality@CEngineScreenEffectRadialBlurRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5BFD0, length 113 bytes (+ int3 padding).
// Callees: CEngineScreenEffectRadialBlurRenderer::InitialiseTextures @ 0x00B5BF10, CGPUScratchBufferTextureManager::FreeMemoryPage
// @ 0x00F37030 (engine global @ 0x01436E98 + 0x3190).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h -- DynamicTexturePageHandle +0x1c, EffectSupported +0x1c8; the
// enabled flag is the retail-only byte at +0x08 (the header's `_pad_0x08`); the settings' radial-blur byte is at +0x6f.
// The method is attached through a derived shell (the header is a plain layout struct).
// Shape: the page is freed through an inline `if (handle >= 0)` helper (`test ; jl`, the handle not reset); the success
// arm returns the stored 1 (`mov al,1 ; mov [esi+8],al`), the failure arm the stored 0, the disabled arm stores an
// immediate 0 after the free and returns 1.
// agentNF57 (q10421): defined as the class's own virtual, Ego_r.pdb / FableWin.pdb ?SetEffectQuality@CEngineScreenEffectRadialBlurRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z (retail vtable slot 13, the first after
//   CEngineComponent's thirteen), on the header's retail _MEMBERS branch (new overlay opt-in FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_SETEFFECTQUALITY)
//   with engine/CEngineVideoOptionsDef.h (FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS: the quality flags at +0x6C..+0x6F)
//   and CEngineComponent::RenderEnabled (+0x08, FABLE_RETAIL_CENGINECOMPONENT_PROTECTED). It now calls the private
//   InitialiseTextures by its PDB spelling. Was the derived shell CEngineScreenEffectRadialBlurRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings&).
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_SETEFFECTQUALITY
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

bool CEngineScreenEffectRadialBlurRenderer::SetEffectQuality(const CEngineVideoOptionsDef& options)
{
    if (options.EnableRadialBlur && EffectSupported) {
        if (InitialiseTextures()) {
            RenderEnabled = true;
            return true;
        }
        if (DynamicTexturePageHandle >= 0)
            NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
        RenderEnabled = false;
        return false;
    }
    if (DynamicTexturePageHandle >= 0)
        NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
    RenderEnabled = false;
    return true;
}
