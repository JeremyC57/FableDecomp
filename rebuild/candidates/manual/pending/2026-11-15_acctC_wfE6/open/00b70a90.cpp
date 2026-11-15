// CEngineWaterRenderer::RenderEnvironmentMap() -- draw the water's environment map: the sky dome (theme channel 0's sky and
// Grade: DIFFER(3567v3567) -- 4 fg hunks, 2 real: the last two bookmark blocks (SetTextureUVModeU/V(0, CLAMP) of the back-buffer pass, 0x00B716B7 / 0x00B71724) take the flag / mask registers swapped; two identical-text hunks at 0x00B70C61 / 0x00B70EFD are display artefacts of the masked diff.
//   sky-blend textures, top / bottom colours, the water colour premultiplied by its reflection factor, the camera's heading and
//   field of view mapped onto the sky strip), then the alpha-blended back-buffer copy for the reflection
// Leaf: ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B70A90, length 3567 bytes, __thiscall, plain `ret`.
// Identity: Ego_r.pdb and FableWin.pdb ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ (private).
// Callees: CIDrawWorld::DrawGetEnvironment (virtual on the world map's GameWorld) and CIDrawEnvironment::PeekInterpolatedParams
//   (virtual); inside the two inlined GetFrame calls CFrameData::CFrameData 0x009FE380 / BeginLoading 0x009FDE80, CResource::
//   ListRemove 0x009FBFB0 / ListInsert 0x009FBF90 and the resource's Release (vtable slot 1) (flags 0: the forced-loading branch
//   folds away); operator new 0x00BFEA1A / delete 0x00BFE9BC, CGraphicFrameData::Uninitialise 0x009FCBA0; CShaderRenderManager::
//   ApplyPixelShader 0x00988140 / ApplyVertexShader 0x00988020 / SetUserVertexShaderConstant 0x00989B50 (K, const CRGBFloatColour&)
//   and 0x00989B00 (KMMMM); C2DVector::AccurateNormalise 0x00A14510; the CRT _CIacos 0x00BFEBA2 (twice; tan is the inline fptan
//   intrinsic); CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60; CDisplayManager::CopyBackBufferToTexture 0x009BF5E0;
//   CTexture::GetDimensions 0x009F9DB0; IDirect3DDevice9::SetTexture (+0x104) through the inlined AttachTextureToStage; and
//   PrepareForRender(0)'s update calls (ResetWorldTransform 0x00988290 among them).
// Inline (both PDBs name them, no retail copy): CEngineCamera::PeekLookVector / PeekHFOV / PeekVFOV (`fld 1 ; fdiv Aspect ; fmul
//   HFOV`), CShaderRenderManager::SetPixelShaderConstant / PrepareForRender, CRenderManagerCore::AttachTextureToStage,
//   CRenderStateManager::SetTextureUVModeU/V / SetSourceAlphaBlendMode / SetDestAlphaBlendMode / SetAlphaBlendEnable,
//   CEngineResourceManager::PeekGraphicBank, CGraphicDataBank::GetGraphicInfo, CGraphicInfoFrame::GetFrame, the CGraphicFrame release.
#include "fable_compiler.h"
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_BOOKMARK
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ADDSTATE_DIRTY
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#define FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_LOOKFOV
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#include <math.h>
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineCamera.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }            // 0x01436E8C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                      // 0x01436EA0
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C

FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

inline float GFClamp01(float a)
{
    if (a < 0.0f)
        a = 0.0f;
    else if (a > 1.0f)
        a = 1.0f;
    return a;
}

// Globals: NEngineInternal::GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, GPResourceManager 0x01436E98, GPEngineWorldMap 0x01436E8C,
//   GPCamera 0x01436EA0, GPDisplayManager 0x01436E1C (anniversary PDB NEngineInternal globals + reloc_bindings).
// FITTED: CRenderStateManager SETRENDERSTATE_DESIRED + _BOOKMARK + ADDSTATE_DIRTY (existing shared opt-ins) and one unused named local (re-keys the
//   0x00B7152E block); neither closes the last two blocks (k = 1..8 locals at the top / before the second pass, 1..8 dead asserts: no change).
void CEngineWaterRenderer::RenderEnvironmentMap()
{
    long unused0 = 0;   // Fidelity: FITTED unused named local, re-keys the bookmark operand order of the 0x00B7152E block
    const CIDrawEnvironmentParams& params = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    unsigned long skyTexture = params.ThemeChannels[0].TextureSets[0].Textures[0];
    unsigned long skyBlendTexture = params.ThemeChannels[0].TextureSets[0].Textures[1];
    const CGraphicDataBank* bank = NEngineInternal::GPResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0));
    if (!skyTexture)
        return;
    CGraphicFrame frame;
    if (!bank->PeekGraphicInfoFrame(skyTexture, 0)->GetFrame(frame, 0, 0))
        return;
    CTexture* texture = frame.GetTexture();
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, texture);
    if (skyBlendTexture) {
        if (bank->PeekGraphicInfoFrame(skyBlendTexture, 0)->GetFrame(frame, 0, 0))
            texture = frame.GetTexture();
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, texture);

    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SkyMapPixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SkyMapVertexShader, false);
    CRGBFloatColour topColour = params.ThemeChannels[0].Colours[12];
    CRGBFloatColour bottomColour = params.ThemeChannels[0].Colours[13];
    float alpha = (1.0f - params.ThemeChannels[0].TextureSets[0].Blends[0]) * 255.0f;
    float blend = GFClamp01(alpha / 255.0f);
    C4DVector colour;
    colour.X = Settings.Colour.R;
    colour.Y = Settings.Colour.G;
    colour.Z = Settings.Colour.B;
    colour.W = params.ThemeChannels[0].Scalers[2] * Settings.ColourToReflectionFactor;
    C4DVector premultiplied = colour;
    premultiplied.X *= premultiplied.W;
    premultiplied.Y *= premultiplied.W;
    premultiplied.Z *= premultiplied.W;
    premultiplied.W = 1.0f - premultiplied.W;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, premultiplied);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(blend, blend, blend, blend));

    const C3DVector& look = NEngineInternal::GPCamera->PeekLookVector();
    float lookZ = look.Z;
    C2DVector direction(look.X, look.Y);
    direction.AccurateNormalise();
    float fovFraction = NEngineInternal::GPCamera->PeekHFOV() / 6.2831855f;
    float angle = acos(direction.Y) / 6.2831855f;
    if (direction.X < 0.0f)
        angle = 1.0f - angle;
    float halfFOV = NEngineInternal::GPCamera->PeekHFOV() * 0.5f;
    float start = angle - halfFOV / 6.2831855f;
    float vfov = NEngineInternal::GPCamera->PeekVFOV();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, topColour);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, bottomColour);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, start, fovFraction,
        tan(1.5707964f - acos(lookZ) + vfov * 0.5f) * 0.9285714f + 0.071428575f, 0.0f);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 14, 18, 2);

    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(EnvironmentMapPixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(EnvironmentMapVertexShader, false);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    CTexture& backBuffer = NEngineInternal::GPDisplayManager->CopyBackBufferToTexture();
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, &backBuffer);
    unsigned long width = backBuffer.GetDimensions(0).X;
    unsigned long height = backBuffer.GetDimensions(0).Y;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, (float)width, (float)height, 0.0f, 0.0f);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 14, 6, 4);
}
