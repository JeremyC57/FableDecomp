// CEngineSkyRenderer::RenderMoon(float, const C3DVector&)
// Leaf: ?RenderMoon@CEngineSkyRenderer@@AAEXMABVC3DVector@@@Z
// Flags: /GS
#define FABLE_RETAIL_CENGINESKYRENDERER_MOON_PASS30
#define FABLE_RETAIL_CENGINESKYRENDERER_MOON_SPRITE_BODY_PASS30
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME3
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINESTATEBLOCKS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION

#include <math.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/EGraphicBankNo.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineStateBlocks.h"
#include "engine/CSkyDef.h"

namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;
extern CEngineLightingManager* GPLightingManager;
extern CEngineWorldMap* GPEngineWorldMap;
extern CEngineStateBlocks* GPEngineStateBlocks;
}

void CEngineSkyRenderer::RenderMoon(float interpolate,const C3DVector& moon_origin)
{
    CRenderStateScopedCaptureBlock capture_block;
    C3DVector moon_vector = NEngineInternal::GPLightingManager->PeekMoonVector();
    const unsigned long moon_index = PSkyDef->MoonTexture;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
          ->PeekGraphicInfoFrame(moon_index,0)->IsLoaded()) {
        NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
            ->PeekGraphicInfoFrame(moon_index,0)->BeginLoading(0,0);
        return;
    }

    CGraphicFrame moon_graphic;
    if (NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
        ->GetGraphicFrameMipmap(moon_graphic,moon_index,0,0,0)) {
        CTexture* moon_texture = moon_graphic.GetTexture();
        NEngineInternal::GPEngineStateBlocks->ApplyAlphaStateBlock();
        const CIDrawEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()
            ->DrawGetEnvironment()->PeekInterpolatedParams();
        CRGBColour colour = params.ThemeChannels[0].Colours[8];
        colour.SetA(255);
        C3DVector moon_pos = moon_origin + moon_vector * 2000.0f;
        C3DVector right = moon_vector ^ C3DVector(0.0f,0.0f,1.0f);
        if (right.IsNonZero())
            right.Normalise();
        else
            right = C3DVector(1.0f,0.0f,0.0f);
        C3DVector up = right ^ moon_vector;
        right *= PSkyDef->MoonRadius * 0.001f * 2000.0f;
        up *= PSkyDef->MoonRadius * 0.001f * 2000.0f;
        RenderSprite(moon_texture,moon_pos,right,up,colour);
    }
}