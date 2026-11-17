// CEngineScreenEffectDisplacementRenderer::SetEffectQuality(const CEngineVideoQualitySettings&) -- sets the 2D/3D feature flags, initializes textures if either is requested and supported, otherwise frees its scratch page; retail 0x00B86B00 length 0xa3. Uses shared renderer layout and the video-quality twin's four-byte option prefix; callees 0x00B86A70 and 0x00F37030.
// Leaf: ?SetEffectQuality@CEngineScreenEffectDisplacementRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z
// agentNF57 (q10423): defined as the class's own virtual, Ego_r.pdb / FableWin.pdb ?SetEffectQuality@CEngineScreenEffectDisplacementRenderer@@UAE_NABVCEngineVideoOptionsDef@@@Z (retail vtable slot 13, the first after
//   CEngineComponent's thirteen), on the header's retail _MEMBERS branch (new overlay opt-in FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SETEFFECTQUALITY)
//   with engine/CEngineVideoOptionsDef.h (FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS: the quality flags at +0x6C..+0x6F)
//   and CEngineComponent::RenderEnabled (+0x08, FABLE_RETAIL_CENGINECOMPONENT_PROTECTED). It now calls the private
//   InitialiseTextures by its PDB spelling. Was the derived shell CEngineScreenEffectDisplacementRenderer_Quality::SetEffectQuality(const CEngineVideoQualitySettings&).
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_PASS22_FIELDS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SETEFFECTQUALITY
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

bool CEngineScreenEffectDisplacementRenderer::SetEffectQuality(const CEngineVideoOptionsDef& options)
{
    Enable2DDisplacement = EffectSupported && options.Enable2DDisplacement;
    Enable3DDisplacement = EffectSupported && options.Enable3DDisplacement;
    if (!Enable2DDisplacement && !Enable3DDisplacement) {
        if (DynamicTexturePageHandle >= 0)
            NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
        RenderEnabled = false;
        return true;
    }
    if (InitialiseTextures()) {
        RenderEnabled = true;
        return true;
    }
    if (DynamicTexturePageHandle >= 0)
        NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().FreeMemoryPage(DynamicTexturePageHandle);
    RenderEnabled = false;
    return false;
}
