// CEngineSkyRenderer::LoadNewScene -- request theme and flare textures and report whether any still need loading.
// Grade: RELOCATION_MATCH
// Leaf: ?LoadNewScene@CEngineSkyRenderer@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z
// Retail: Fable.exe 0x00B63800, 1023 bytes, virtual thiscall.
// Flags: none
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_LOADNEWSCENE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_FINISH_LOCAL_PASS24
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CSkyDef.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineWorldMap.h"

namespace NEngineInternal {
    extern CEngineWorldMap* GPEngineWorldMap;
    extern CEngineResourceManager* GPResourceManager;
}

bool CEngineSkyRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& load_params)
{
    const CIDrawEnvironmentParams& env_params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    const CIDrawEnvironmentParams::CThemeChannel& theme_channel = env_params.ThemeChannels[0];
    CArray<unsigned long> texture_indices;
    for (unsigned long c_index = 0; c_index < 2; ++c_index) {
        texture_indices.push_back(theme_channel.TextureSets[0].Textures[c_index]);
        texture_indices.push_back(theme_channel.TextureSets[2].Textures[c_index]);
        texture_indices.push_back(theme_channel.TextureSets[1].Textures[c_index]);
    }
    texture_indices.push_back(PSkyDef->SunTexture);
    texture_indices.push_back(PSkyDef->MoonTexture);
    texture_indices.push_back(PSkyDef->StarTexture);
    long no_flare_elements = PSkyDef->FlareElements.size();
    for (long c_flare = 0; c_flare < no_flare_elements; ++c_flare)
        texture_indices.push_back(PSkyDef->FlareElements[c_flare].Texture);
    bool still_loading = false;
    for (unsigned long c_texture = 0; c_texture < texture_indices.size(); ++c_texture) {
        long texture_index = texture_indices[c_texture];
        if (texture_index) {
            const CGraphicInfoFrame* graphic_info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(texture_index, 0);
            if (graphic_info) {
                const CTexture* texture;
                if (!graphic_info->GetTexture(&texture, 0, 0))
                    still_loading = true;
            }
        }
    }
    return still_loading;
}
