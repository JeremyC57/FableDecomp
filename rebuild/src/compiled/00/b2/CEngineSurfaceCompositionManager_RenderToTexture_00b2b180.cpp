// CEngineSurfaceCompositionManager::RenderToTexture(CTexture& texture, const C2DExtentsI& extents, bool alpha, bool unused_flag, const CEngineTextureMorph& morph, const CArray<CGraphicFrame>& frames, long scratch_texture) -- with vertex shaders on, pixel shaders off, constant layout 3, the screen-copy vertex shader and fog / specular off, composite the morph's blend layers (one quad per blend over the matching frame's texture, the blend's diffuse colour) into level 0 of the scratch render texture once per mip level, copy each result into the texture's level and halve the box until it is smaller than MinRenderTargetSize; then fog back on, generate the remaining mipmaps and end the scratch texture use
// Leaf: ?RenderToTexture@CEngineSurfaceCompositionManager@@AAEXAAVCTexture@@ABVC2DExtentsI@@_N2ABVCEngineTextureMorph@@ABV?$CArray@VCGraphicFrame@@@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B180, 1539 bytes, thiscall, `ret 1Ch` (seven stack words), no EH frame. Sole caller
//   CEngineSurfaceCompositionManager::UpdateDiskCacheTexture 0x00B2D330 (call at 0x00B2D911).
// Identity (two sources): Ego_r.pdb ?RenderToTexture@CEngineSurfaceCompositionManager@@AAEXAAVCTexture@@ABVC2DExtentsI@@_N2ABVCEngineTextureMorph@@ABV?$CArray@VCGraphicFrame@@@@J@Z
//   (Ego_r 0x009CC490, private): the only seven-parameter member of the class in both name sets (`ret 1Ch`), and every parameter type is
//   used as the body uses it -- CTexture& (GetSurfaceLevel / GenerateMipmaps), const C2DExtentsI& (+0 / +4 read as longs), bool (passed to
//   StartComposition), const CEngineTextureMorph& (Blends at +0x08, CCompositeBlendDesc stride 0x10: BlendType +0x04, DiffuseColour +0x08),
//   const CArray<CGraphicFrame>& (frames[i].FrameData->PTexture), long (the scratch texture handle for StartTextureUse / EndTextureUse);
//   the sole caller is UpdateDiskCacheTexture 0x00B2D330 (0x00B2D911). The 4th parameter (a
//   bool from UpdateDiskCacheTexture's third argument) is never read (descriptive name).
// Layout: engine/CEngineSurfaceCompositionManager.h MEMBERS opt-in (ScreenCopyVertexShader +0xFB8, MinRenderTargetSize +0x10CC);
//   engine/CSurfaceCompositor.h (new, 0x30 bytes, nested CVertexType 0x18); CEngineTextureMorph Blends +0x08.
// Callees: CGPUScratchBufferTextureManager::StartTextureUse 0x00F36EE0 / EndTextureUse 0x00F36F00, CTexture::GetSurfaceLevel 0x009F9E00 /
//   GenerateMipmaps 0x009F9E60, CShaderRenderManager Enable/DisableVertexShaders 0x00987FE0 / 0x009880E0, Enable/DisablePixelShaders
//   0x00988110 / 0x00988190, SetVertexShaderConstantLayout 0x0098B5E0 (on the singleton 0x013BC470: CScopedShadersEnable /
//   CScopedVertexShaderConstantLayout), ApplyVertexShader 0x00988020 and the PrepareForRender(0) updates SetVSConstants 0x0098B930 /
//   UpdatePixelShader 0x00988A20 / UpdatePixelShaderConstants 0x00989BF0 / UpdateAmbient 0x00989760 / UpdateFogColour 0x009897C0 /
//   UpdateLightGlobals 0x0098A760 / ResetWorldTransform 0x00988290 / UpdateCombinedProjectionTransform 0x00988A50 (on
//   NEngineInternal::GPShaderRenderManager 0x01436E14), CSurfaceCompositor::CSurfaceCompositor(CSurface&) 0x00F38990 / StartComposition
//   0x00F39730 / CompositeQuad 0x00F39A50 / EndComposition 0x00F38930, CSurface::CopyFromSurface 0x009F37E0 / Uninitialise 0x009F2E20,
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840 (CRenderStateCaptureBlock::StateManager 0x013CB508), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC.
// Inline (expanded): the two scoped shader objects, CRenderStateManager::SetFogEnable / SetSpecularEnable (SetRenderState on
//   NEngineInternal::GPRenderManager 0x01436E18, DeviceState.FogEnable +0x2944 / SpecularEnable +0x29E4), PrepareForRender, the
//   compositor's implicit destructor (counted render target, capture block, ~CBaseClass), ~CSurface x2, CVertexType::SetPosition /
//   SetU1V1, CRGBColour::GetD3DCOLOR, CGraphicFrame::GetTexture.
// Shape: (1) the scratch texture is named before GetSurfaceLevel so StartTextureUse is called before the hidden result is pushed
//   (0x00B2B1A8); (2) the clip-space quad is a C2DBoxF filled field by field (TLY first: retail computes 1 - 2*BRY/Y before 2*BRX/X - 1,
//   0x00B2B34D..0x00B2B389) whose constant corners TLX = -1 and BRY = 1 fold into the vertex stores and leave their frame words unused
//   ([esp+3Ch] / [esp+48h]; frame 0xEC); (3) the blend reference is named before the layer texture, so the FrameData temporary takes ecx in
//   c2's expression-temporary rotation (eax still holds the blend; 0x00B2B47D); (4) each vertex is SetPosition, SetU1V1, then the colour;
//   (5) the blend count is read once into a local before the layer loop (`edi`, 0x00B2B458).
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER
#define FABLE_RETAIL_CENGINESURFACECOMPOSITIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CTEXTURE_GENERATEMIPMAPS
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_DTOR
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#include "engine/Direct3D9Types.h"
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/C3DVector.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CArray.h"
#include "engine/CGraphicFrame.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CSurfaceCompositor.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                // 0x01436E18

void CEngineSurfaceCompositionManager::RenderToTexture(CTexture& texture, const C2DExtentsI& extents, bool alpha, bool unused_flag,
                                                       const CEngineTextureMorph& morph, const CArray<CGraphicFrame>& frames,
                                                       long scratch_texture)
{
    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    CTexture* render_texture = scratch.StartTextureUse(scratch_texture);
    CSurface render_surface = render_texture->GetSurfaceLevel(0);
    CScopedShadersEnable shaders_enable(true, false);
    CScopedVertexShaderConstantLayout constant_layout(3);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(ScreenCopyVertexShader, false);
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPRenderManager->SetSpecularEnable(false);
    C2DBoxI box(0, 0, extents.X, extents.Y);
    unsigned long size = extents.X < extents.Y ? extents.X : extents.Y;
    long level = 0;
    while (size >= MinRenderTargetSize) {
        CSurface level_surface = texture.GetSurfaceLevel(level);
        C2DBoxF quad;
        quad.TLX = -1.0f;
        quad.TLY = 1.0f - (float)(box.BRY * 2) / extents.Y;
        quad.BRX = (float)(box.BRX * 2) / extents.X - 1.0f;
        quad.BRY = 1.0f;
        CSurfaceCompositor compositor(render_surface);
        compositor.StartComposition(alpha);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
        unsigned long count = morph.Blends.size();
        for (unsigned long i = 0; i < count; ++i) {
            const CCompositeBlendDesc& blend = morph.Blends[i];
            const CTexture* layer_texture = frames[i].GetTexture();
            float u = 0.25f / box.BRX;
            float v = 0.25f / box.BRY;
            CSurfaceCompositor::CVertexType vertices[4];
            vertices[0].SetPosition(C3DVector(quad.TLX, quad.BRY, 0.5f));
            vertices[0].SetU1V1(u, v);
            vertices[0].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[1].SetPosition(C3DVector(quad.BRX, quad.BRY, 0.5f));
            vertices[1].SetU1V1(u + 1.0f, v);
            vertices[1].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[2].SetPosition(C3DVector(quad.BRX, quad.TLY, 0.5f));
            vertices[2].SetU1V1(u + 1.0f, v + 1.0f);
            vertices[2].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            vertices[3].SetPosition(C3DVector(quad.TLX, quad.TLY, 0.5f));
            vertices[3].SetU1V1(u, v + 1.0f);
            vertices[3].DiffuseColour = blend.DiffuseColour.GetD3DCOLOR();
            compositor.CompositeQuad(*layer_texture, vertices, blend.BlendType);
        }
        compositor.EndComposition();
        level_surface.CopyFromSurface(&box, render_surface, &box, (unsigned long)-1);
        box.BRX /= 2;
        box.BRY /= 2;
        size >>= 1;
        ++level;
    }
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    texture.GenerateMipmaps(5, level - 1);
    scratch.EndTextureUse(scratch_texture);
}