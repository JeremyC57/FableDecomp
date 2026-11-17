// CEngineWaterRenderer::SetupSeaBackgroundShaderConstantsAndFog() -- set up the sea background shader state: user constant 0 = the sea bump-map span; high-end: pixel constant 7 = sea colour with the theme reflection alpha, user constant 3 from the bump factor and a camera-centred world transform; low end: the static light/colour constants (5 / 6); then PrepareForRender
// Leaf: ?SetupSeaBackgroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7C510, length 864 bytes (+ int3 padding), plain `ret`, no direct caller (an out-of-line
//   copy of the private inline helper that RenderSeaBackground 0x00B70230 expands).
// Identity (agentNF57 name fix, settles NF46's debt Background vs Midground): FableWin.pdb 0x02D51100
//   ?SetupSeaBackgroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ. This body uses the function statics 0x01437864 (colour),
//   0x01437874 (light_dot) and their guard 0x01437884, and the same call sequence (SetUserVertexShaderConstant, SetWorldTransform,
//   the PrepareForRender updates), as the copy inlined in the landed RenderSeaBackground 0x00B70230, which binds those statics as
//   SetupSeaBackgroundShaderConstantsAndFog's; RenderSeaMidground 0x00B6FA40 inlines the Midground helper with its own statics
//   0x01437840 / 0x01437850 / 0x01437860. The old row spelled it SetupSeaMidgroundShaderConstantsAndFog (public, local model).
// Shape: 0x00B70230's helper body verbatim, on its shared headers and opt-ins (engine/CEngineWaterRenderer.h
//   FABLE_RETAIL_CENGINEWATERRENDERER_SEABACKGROUND_BODIES declares the private member), compiled out of line. The world matrix is
//   built by CMatrix3x4(MATRIX34_INIT_IDENTITY) + SetTranslateOnly; 0x01436E8C is NEngineInternal::GPEngineWorldMap.
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CENGINEWATERSETTINGS_TYPED
#define FABLE_RETAIL_C4DVECTOR_ORIGINAL_COLOUR_CHAIN
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_FLOAT_CTORS
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SEAMIDGROUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
#define FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEABACKGROUND_BODIES
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GETSETTINGS
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_SEAMIDGROUND
#include "fable_assert.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "fable_string.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
namespace NEngineInternal {
 extern CEngineResourceManager* GPResourceManager;
 extern const CEnvironmentDef* GPEnvironmentDef;
 extern CRenderManager* GPRenderManager;
 extern CShaderRenderManager* GPShaderRenderManager;
 extern CEngineLightingManager* GPLightingManager;
 extern CEngineLandscapeRenderer* GPLandscapeRenderer;
 extern CEngineWorldMap* GPEngineWorldMap;
}
#include "engine/CEngineCamera.h"
namespace NEngineInternal { extern CEngineCamera* GPCamera; }

void CEngineWaterRenderer::SetupSeaBackgroundShaderConstantsAndFog()
{
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0,Settings.SeaBumpmapSpan,0.0f,0.0f,0.0f);
    if(HighendWaterEnabled) {
        C4DVector colour(Settings.Colour);
        const CIDrawEnvironmentParams& params=NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        colour.W=params.ThemeChannels[0].Scalers[2]*Settings.ColourToReflectionFactor;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7,colour);
        float bump_factor=Settings.BumpMatFactor*WaterBumpFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3,bump_factor,0.0f,-bump_factor/2.0f,1.0f);
        CMatrix3x4 world_matt(MATRIX34_INIT_IDENTITY);
        world_matt.SetTranslateOnly(NEngineInternal::GPCamera->PeekCameraPos());
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(world_matt);
    } else {
        static C4DVector light_dot(0.8f,0.8f,0.0f,0.0f);
        static CRGBFloatColour colour(0.0f,0.4f,0.8f);
        CRGBFloatColour col=colour*NEngineInternal::GPLightingManager->PeekGlobalDiffuseColour();
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5,light_dot);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6,C4DVector(col));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x205);
}
