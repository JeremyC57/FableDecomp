// CEngineSkyRenderer::RenderClouds(float interpolate) -- draw the two cloud layers: inside a render-state capture block read the interpolated environment's first theme channel, fetch frame 0 of the four cloud textures (texture sets 2 and 1, textures 0 / 1; a missing id or a frame that is not loaded yet gives the engine's zero-RGBA texture and counts as missing), return when all four are missing, apply the cloud pixel shader, upload (1 - blend 0) of sets 2 / 1 as pixel constants 5 / 6 and colour 7 with w 1 as constant 7, wrap UV modes on stages 0..3, premultiplied-alpha blending (ONE, INV_SRC_ALPHA, blending on, alpha test off), apply the cloud vertex shader, interpolate the two cloud scroll vectors from their history and upload them as user vertex constant 0, flush the shader state and draw the cloud mesh indexed with the four textures
// Grade: RELOCATION_MATCH (2688v2688, 734/734; session 4 grade 149, cand/c74.cpp). The twelve SetRenderState expansions' bookmark test / or
//   operand order (retail BBAAABBBBBBB) is a c2 record-id tie decided by the function's named-symbol / IL amount: closed with the pixel
//   constants 5 / 6 spelled as the matched sibling RenderOuterSky spells constant 5 (C4DVector(b, b, b, b)), the FableWin-count releases
//   (FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL + CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT) and SetRenderState's named desired_state
//   (FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED, FITTED). RenderSunFlare / RenderOuterSky stay RELOCATION_MATCH with these.
// Leaf: ?RenderClouds@CEngineSkyRenderer@@AAEXM@Z
// Inline-budget: FABLE_FORCEINLINE Lionhead header helpers as in the matched sibling 0x00B65A20 RenderOuterSky (PeekGraphicInfoFrame, GetFrame,
//   TouchTexture, TouchResource, ~CGraphicFrame; owner 2026-11-01 rule, retail copies uncalled), the same overlay headers.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B64FA0, 2688 bytes, thiscall, `ret 4`, 0x38-byte frame, no EH frame. Start: 16-byte aligned after the
//   previous function; RenderSky 0x00B66190 calls it. End: the out-of-line tails (0x00B659BA.. missing / create-and-load paths, last `jmp 0x00B659C4`
//   at 0x00B65A1E) belong to it; RenderOuterSky starts at 0x00B65A20. Catalog length right.
// Identity (two sources): FableWin.pdb ?RenderClouds@CEngineSkyRenderer@@AAEXM@Z (private, one float: `ret 4`) and Ego_r.pdb 0x00A15F60 right
//   before RenderOuterSky 0x00A169E0 (retail 0x00B64FA0 -> 0x00B65A20, same order); FableWin lists GetZeroRGBATexture as its only new COMDAT.
// Callees: CIDrawWorld slot 1 / CIDrawEnvironment slot 1 (as RenderOuterSky), CFrameData::BeginLoading 0x009FDE80, CFrameData(const
//   CGraphicInfoFrame*) 0x009FE380, CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CGraphicFrameData::Uninitialise 0x009FCBA0,
//   operator new 0x00BFEA1A / delete 0x00BFE9BC, CShaderRenderManager::ApplyPixelShader 0x00988140 / ApplyVertexShader 0x00988020 /
//   SetUserVertexShaderConstant(unsigned long, float, float, float, float) 0x00989B00, the inlined PrepareForRender(0) (SetVSConstants 0x0098B930,
//   UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0,
//   UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290, UpdateCombinedProjectionTransform 0x00988A50),
//   CRenderManagerCore::DrawCurrentBuffersIndexed(const CTexture**, 5 x unsigned long) 0x00A0ACA0, CRenderStateManager::RestoreCaptureBlock 0x00A05840.
// Globals: g_pEngineWorldMap 0x01436E8C, NEngineInternal::GPResourceManager 0x01436E98 (GetZeroRGBATexture = +0x2AD4), GPRenderManager 0x01436E18,
//   GPShaderRenderManager 0x01436E14, CRenderStateManager::D3DTextureUVModeTable[TEXTURE_UV_WRAP] 0x01396DC8, D3DAlphaBlendModeTable[ONE]
//   0x01396F6C / [INV_SRC_ALPHA] 0x01396F7C, the capture-block state manager 0x013CB508. Members: CloudPixelShader +0x120, CloudVertexShader
//   +0xE8, CloudVector[2] +0x15C, CloudVectorHistory[2][2] +0x16C (Ego_r layout = retail).
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB
#define FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_MUL
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_RENDERCLOUDS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWINDEXED_TEXTURES
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/CEngineWorldMap.h"
#include "engine/GFGetInterpolatedValue.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
extern CEngineWorldMap* g_pEngineWorldMap;                                            // 0x01436E8C

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

void CEngineSkyRenderer::RenderClouds(float interpolate)
{
    CRenderStateScopedCaptureBlock capture;
    const CIDrawEnvironmentParams& params = g_pEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    long texture_ids[4];
    texture_ids[0] = params.ThemeChannels[0].TextureSets[2].Textures[0];
    texture_ids[1] = params.ThemeChannels[0].TextureSets[2].Textures[1];
    texture_ids[2] = params.ThemeChannels[0].TextureSets[1].Textures[0];
    texture_ids[3] = params.ThemeChannels[0].TextureSets[1].Textures[1];
    const CTexture* textures[4];
    long num_missing = 0;
    for (long i = 0; i < 4; ++i) {
        long texture_id = texture_ids[i];
        const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0);
        CGraphicFrame frame;
        if (texture_id != 0 && bank->PeekGraphicInfoFrame(texture_id, 0)->GetFrame(frame, 0, 0)) {
            textures[i] = frame.GetTexture();
        } else {
            textures[i] = NEngineInternal::GPResourceManager->GetZeroRGBATexture();
            ++num_missing;
        }
    }
    if (num_missing == 4)
        return;

    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(CloudPixelShader);
    float blend0 = 1.0f - params.ThemeChannels[0].TextureSets[2].Blends[0];
    float blend1 = 1.0f - params.ThemeChannels[0].TextureSets[1].Blends[0];
    CRGBFloatColour colour = params.ThemeChannels[0].Colours[7];
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(blend0, blend0, blend0, blend0));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(blend1, blend1, blend1, blend1));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, C4DVector(C3DVector(colour.GetR(), colour.GetG(), colour.GetB()), 1.0f));
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(3, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(3, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(CloudVertexShader, false);
    CloudVector[0] = (CloudVectorHistory[0][0] - CloudVectorHistory[0][1]) * interpolate + CloudVectorHistory[0][1];
    CloudVector[1] = (CloudVectorHistory[1][0] - CloudVectorHistory[1][1]) * interpolate + CloudVectorHistory[1][1];
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, CloudVector[0].X, CloudVector[0].Y, CloudVector[1].X, CloudVector[1].Y);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersIndexed(textures, 4, 0x49, 0x14d, 0x144, 0x240);
}
