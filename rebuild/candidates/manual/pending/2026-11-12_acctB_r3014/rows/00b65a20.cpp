// CEngineSkyRenderer::RenderOuterSky(float interpolate) -- draw the outer sky dome: inside a render-state capture block, read the interpolated environment's first theme channel (sky texture set 0: textures 0 / 1 and blend 0; colours 12 / 13), return when there is no sky texture, fetch frame 0 of the first texture (requesting the load and drawing nothing while it is not loaded) and of the second one (falling back to the first texture), apply the sky vertex / pixel shaders, upload the blend (1 - blend 0) as pixel constant 5 and the two colours as user vertex constants 0 / 1, set additive blending (source and destination ONE, blending on), flush the shader state and draw the current buffers indexed with the two textures
// Leaf: ?RenderOuterSky@CEngineSkyRenderer@@AAEXM@Z
// Inline-budget: FABLE_FORCEINLINE Lionhead header helpers (owner 2026-11-01 rule: retail copies uncalled or absent), measured with the c2
//   inline trace (tools/inl.sh, default threshold 74), the same set as RenderSnow 0x00B54E30: CGraphicDataBank::PeekGraphicInfoFrame (c1 102;
//   uncalled copy 0x0041C1F0), CGraphicInfoFrame::GetFrame (uncalled copy 0x0041C260), CManagedTexture::TouchTexture (uncalled 0x0041BB70),
//   CResource::TouchResource (uncalled 0x0041B9C0), ~CGraphicFrame (c1 88; ~CFrameData 0x009FE6C0 calls UnInitialise but expands the
//   member destructor). With them every inline decision of the trace is r=1.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B65A20, 1893 bytes, thiscall, `ret 4`, 0x44-byte frame, no EH frame. Start confirmed: RenderSky
//   0x00B66190 calls it (0x00B66284, `call 0x00B65A20`); the preceding function's out-of-line tail ends with the 2-byte `jmp 0x00B659C4`
//   at 0x00B65A1E, so 0x00B65A20 follows it with no padding (16-byte aligned). End: the out-of-line create-and-load tail of the second
//   frame fetch (0x00B66143..0x00B66184, `jmp 0x00B65DF6`) belongs to it; int3 padding from 0x00B66185. The catalog length is right.
// Identity (two sources): Ego_r.pdb 0x00A169E0 / FableWin.pdb ?RenderOuterSky@CEngineSkyRenderer@@AAEXM@Z (private, one float argument:
//   `ret 4`; the argument is not read), Ego_r donor order RenderClouds 0x00A15F60 -> RenderOuterSky 0x00A169E0 = retail 0x00B64FA0 ->
//   0x00B65A20, and its only caller RenderSky (Ego_r ?RenderSky@CEngineSkyRenderer@@AAEXMABVC3DVector@@_N@Z) passes its interpolation.
// Callees: CIDrawWorld slot 1 DrawGetEnvironment (CWorld 0x0049E350), CIDrawEnvironment slot 1 PeekInterpolatedParams (CEnvironment
//   0x006B7350), CFrameData::BeginLoading 0x009FDE80 / CFrameData(const CGraphicInfoFrame*) 0x009FE380, CResource::ListRemove 0x009FBFB0 /
//   ListInsert 0x009FBF90, CGraphicFrameData::Uninitialise 0x009FCBA0, operator new 0x00BFEA1A / delete 0x00BFE9BC, CShaderRenderManager::
//   ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetUserVertexShaderConstant(unsigned long, const CRGBFloatColour&)
//   0x00989B50, the inlined PrepareForRender(0) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants
//   0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, ResetWorldTransform 0x00988290,
//   UpdateCombinedProjectionTransform 0x00988A50), CRenderManagerCore::DrawCurrentBuffersIndexed(const CTexture**, 5 x unsigned long)
//   0x00A0ACA0, CRenderStateManager::RestoreCaptureBlock 0x00A05840.
// Globals: g_pEngineWorldMap 0x01436E8C (established bound spelling; GameWorld +0x198), NEngineInternal::GPResourceManager 0x01436E98,
//   GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14 (established spellings, no PDB data symbol), the capture-block state
//   manager 0x013CB508, CRenderStateManager::D3DAlphaBlendModeTable[ALPHA_BLEND_ONE] 0x01396F6C.
// Shape: the frame fetches are the shared GetFrame of RenderSnow (overlay engine/CGraphicInfo.h); one CGraphicFrame shared by both fetches
//   (the second assignment releases the first frame with no null test: its texture pointer was read). The bank is a named local (kept in
//   edi for the second fetch). The two colours are copied straight from `params.ThemeChannels[0].Colours[12] / [13]` into named
//   locals with the existing FABLE_RETAIL_CARRAY opt-in (CArray::operator[] through begin(), as STLport vector::operator[]): the
//   inline begin() result is an allocator candidate (a named value, not an expression temporary), so each colour address is a base + 0xC0 / 0xD0 address form (c2 opcode 18, emitted
//   `add eax,0C0h|0D0h`) rather than a two-address `add`: the scheduler keeps colour 13's add ahead of its first load (0x00B65E97),
//   the blend vector's Y dead store follows the GPShaderRenderManager load (0x00B65E42) and the per-region round-robin that splits
//   `UpdateFlags |= 2` (c2.dll 0x10785179, skipping opcode-18 tuples) lands on ebx (0x00B65E5A). Named const references or the plain
//   `_M_start[i]` operator[] make the addresses visited `add` tuples (esi / load-first / Y-first, 597/602).
#include "fable_compiler.h"
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_RENDEROUTERSKY
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

void CEngineSkyRenderer::RenderOuterSky(float interpolate)
{
    CRenderStateScopedCaptureBlock capture;
    const CIDrawEnvironmentParams& params = g_pEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    long texture0 = params.ThemeChannels[0].TextureSets[0].Textures[0];
    long texture1 = params.ThemeChannels[0].TextureSets[0].Textures[1];
    if (texture0 == 0)
        return;
    float blend = 1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0];
    const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0);
    CGraphicFrame frame;
    if (!bank->PeekGraphicInfoFrame(texture0, 0)->GetFrame(frame, 0, 0))
        return;
    const CTexture* textures[2];
    textures[0] = frame.GetTexture();
    if (texture1 != 0 && bank->PeekGraphicInfoFrame(texture1, 0)->GetFrame(frame, 0, 0))
        textures[1] = frame.GetTexture();
    else
        textures[1] = textures[0];

    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SkyVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SkyPixelShader);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(blend, blend, blend, blend));
    CRGBFloatColour colour0 = params.ThemeChannels[0].Colours[12];
    CRGBFloatColour colour1 = params.ThemeChannels[0].Colours[13];
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, colour0);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, colour1);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersIndexed(textures, 2, 0, 0x49, 0, 0x6c);
}
