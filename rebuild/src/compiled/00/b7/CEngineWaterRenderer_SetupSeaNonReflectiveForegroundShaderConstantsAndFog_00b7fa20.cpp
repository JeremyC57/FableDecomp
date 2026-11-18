// CEngineWaterRenderer::SetupSeaNonReflectiveForegroundShaderConstantsAndFog() -- set up the non-reflective sea foreground shader state: user constant 0 = swell (when enabled), the four foreground bone matrices, pixel constant 5 = the clamped fresnel factor, user constant 3 from the bump factor, pixel constant 7 = the water colour with the theme's reflection alpha; then the inlined CShaderRenderManager::PrepareForRender(0x604)
// Leaf: ?SetupSeaNonReflectiveForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7FA20, length 704 bytes (+ int3 padding), plain `ret`, no direct caller (an
//   out-of-line copy of the private inline helper that RenderSeaNonReflectiveForeground 0x00B77E20 expands).
// Identity: FableWin.pdb 0x02D501F0 ?SetupSeaNonReflectiveForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ (private, no
//   argument: this body takes none). The old row emitted it public (QAE) from a local class model.
// Port (NF68, round 7): the local C4DVector / CRGBFloatColour / CMatrix3x4 / CPreTransposedBoneMatrix / CArray / CShaderRenderManager /
//   CEngineWaterSettings / CEngineWaterRenderer models and the made-up environment view (SEngineState / CEnvironmentProvider) are replaced
//   by the shared headers, on the macro block of the landed sibling SetupSeaBackgroundShaderConstantsAndFog 0x00B7C510 (plus the NF68
//   overlay opt-in FABLE_RETAIL_CENGINEWATERRENDERER_SETUPSEANONREFLECTIVEFOREGROUND that declares this private member):
//   0x01436E8C is NEngineInternal::GPEngineWorldMap and the alpha is
//   PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeChannels[0].Scalers[2] (+0x54 / +0x138), as in 0x00B7C510.
// Callees: SetUserVertexShaderConstant(KMMMM) 0x00989B00, the inline SetBoneMatrixCount (BoneMatrices resize 0x00B7EDF0),
//   SetBoneTransform(K, const CMatrix3x4&) 0x0098A7F0, the inline SetPixelShaderConstant and PrepareForRender. Float pool: 0.25f
//   @ 0x0123078C, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8, -0.5f @ 0x01238798.
// Shape: the swell constant is two whole calls (if/else, cross-jumped); the fresnel factor is a ternary over an inline Clamp
//   (`v < lo` / `v > hi` returns; no shared Clamp header exists); the sea-colour constant is a C4DVector local filled member by member.
#define FABLE_RETAIL_CENGINEWATERRENDERER_SETUPSEANONREFLECTIVEFOREGROUND
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

inline float Clamp(float value, float lo, float hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

void CEngineWaterRenderer::SetupSeaNonReflectiveForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SwellFactor, Settings.ShoreSwellFactor, Settings.RefOffset, 0.0f);
    else
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetBoneMatrixCount(4);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, Settings.OtherSeaShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    float factor = WaterBumpFactor * Settings.BumpMatFactor;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
    C4DVector colour;
    colour.X = Settings.Colour.R;
    colour.Y = Settings.Colour.G;
    colour.Z = Settings.Colour.B;
    const CIDrawEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    colour.W = params.ThemeChannels[0].Scalers[2] * Settings.ColourToReflectionFactor;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, colour);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}
