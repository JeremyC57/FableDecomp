// CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>&) -- construct the
//   animated-mesh primitive manager: base MeshBase ctor, default-construct the CVertexShader members, fill PointLightShaderIndex
//   {0,1,1,2,2,3}, fetch 61 palette-skinned vertex shaders by name from vertex-shader banks 3 and 4 (the DP3-bump ones from bank 4;
//   the spot-light / colour-buffer-shadow set only when the pixel-shader version is >= 1400 = ps_1_4), point the shadow selectors at
//   the depth-buffer sets and set TextureOverlayFadeDistandeFactor = 2.0.
// Grade: RELOCATION_MATCH (fg.py, FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077, no flags)
// Retail: Fable.exe @ 0x00BCFF10, length 8140 bytes, __fastcall, `ret 4`, returns this, no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z; vtable 0x012A78DC.
// Callees: CEnginePrimitiveManagerMeshBase::CEnginePrimitiveManagerMeshBase(CArray<CEngineScene*>&) 0x00BEBC30,
//   CVertexShader::CVertexShader() 0x00A62FB0, CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CShaderResource::GetVertexShader(const CCharString&) const 0x00A5D5F0, CDisplayManager::GetPixelShaderVersion() const 0x009BEE00.
//   Inline: CVertexShader::operator= / ~CVertexShader (CVertexShaderData::Release), CEngineResourceManager::GetVShaderResource.
// Globals: NEngineInternal::GPResourceManager 0x01436E98, NEngineInternal::GPDisplayManager 0x01436E1C.
// Shape: the two bank pointers are source locals (loaded once, kept in ebx / the dead arg slot across all 61 calls).
// Headers: overlay hdr/engine/CEnginePrimitiveManagerAnimatedMeshesBase.h (NEW, agent4010) and CDisplayManager.h
//   (FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION).
// Leaf: ??0CEnginePrimitiveManagerAnimatedMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Flags: none
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/Direct3D9Types.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CDisplayManager.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98 (labels_landing_2026-11-12ef.tsv; landed 0x00BBCC70 / 0x00BADB40 / 0x00B2D9F0)
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C

CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerMeshBase(scenes)
{
    PointLightShaderIndex[0] = 0;
    PointLightShaderIndex[1] = 1;
    PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 2;
    PointLightShaderIndex[4] = 2;
    PointLightShaderIndex[5] = 3;

    const CShaderResource* shader_resource     = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)3);
    const CShaderResource* dp3_shader_resource = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)4);

    VertexShadersPalSkin[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG");   // 00bd01b2 +0x038
    VertexShadersPalSkin[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG");   // 00bd022c +0x040
    VertexShadersPalSkin[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG");   // 00bd02a2 +0x048
    VertexShadersPalSkin[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG");   // 00bd0318 +0x050
    VertexShadersPalSkinEnv[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG");   // 00bd038e +0x058
    VertexShadersPalSkinEnv[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG");   // 00bd0404 +0x060
    VertexShadersPalSkinEnv[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG");   // 00bd047a +0x068
    VertexShadersPalSkinEnv[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG");   // 00bd04f0 +0x070
    VertexShaderPalSkinOutline = shader_resource->GetVertexShader("VSHADER_PALSKIN_OUTLINE");   // 00bd0566 +0x2a8
    VertexShaderPalSkinOutlineSmoothFill = shader_resource->GetVertexShader("VSHADER_PALSKIN_OUTLINE_SMOOTH_FILL");   // 00bd05dd +0x2b8
    VertexShadersPalSkinDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SHADOW");   // 00bd0655 +0x098
    VertexShadersPalSkinDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd06cd +0x0a0
    VertexShadersPalSkinDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0745 +0x0a8
    VertexShadersPalSkinDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd07bd +0x0b0
    VertexShadersPalSkinSpotDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SPOT_SHADOW");   // 00bd0835 +0x118
    VertexShadersPalSkinSpotDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd08ad +0x120
    VertexShadersPalSkinSpotDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0925 +0x128
    VertexShadersPalSkinSpotDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd099d +0x130
    VertexShadersPalSkinDepthShadowBufferRender = shader_resource->GetVertexShader("VSHADER_PALSKIN_SHADOW_BUFFER_RENDER");   // 00bd0a15 +0x198
    VertexShadersPalSkinEnvDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SHADOW");   // 00bd0a8d +0x0b8
    VertexShadersPalSkinEnvDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd0b05 +0x0c0
    VertexShadersPalSkinEnvDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0b7d +0x0c8
    VertexShadersPalSkinEnvDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd0bf5 +0x0d0
    VertexShadersPalSkinEnvSpotDepthBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SPOT_SHADOW");   // 00bd0c6d +0x138
    VertexShadersPalSkinEnvSpotDepthBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0ce5 +0x140
    VertexShadersPalSkinEnvSpotDepthBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0d5d +0x148
    VertexShadersPalSkinEnvSpotDepthBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");   // 00bd0dd5 +0x150
    VertexShadersPalSkinDP3BumpDepthBufferShadow[0] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_FOG_SHADOW");   // 00bd0e4d +0x0f8
    VertexShadersPalSkinDP3BumpDepthBufferShadow[1] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");   // 00bd0ec7 +0x100
    VertexShadersPalSkinDP3BumpDepthBufferShadow[2] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");   // 00bd0f41 +0x108
    VertexShadersPalSkinDP3BumpDepthBufferShadow[3] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");   // 00bd0fbb +0x110
    VertexShaderPalSkinDP3BumpOutline = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_OUTLINE");   // 00bd1035 +0x2b0
    VertexShaderPalSkinDP3BumpOutlineSmoothFill = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_OUTLINE_SMOOTH_FILL");   // 00bd10af +0x2c0
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {   // 0x00BD1129: mov ecx,[0x01436E1C]; call 0x009BEE00; cmp eax,578h; jb 0x00BD1CFF
        VertexShadersPalSkinSpotlight[0] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_SPOT_LIGHT");   // 00bd113f +0x30c
        VertexShadersPalSkinSpotlight[1] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_2POINTLIGHTS_SPOT_LIGHT");   // 00bd11b7 +0x314
        VertexShadersPalSkinSpotlight[2] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_4POINTLIGHTS_SPOT_LIGHT");   // 00bd122f +0x31c
        VertexShadersPalSkinSpotlight[3] = shader_resource->GetVertexShader("VSHADER_RIGIDSKIN_DIRLIGHT_5POINTLIGHTS_SPOT_LIGHT");   // 00bd12a7 +0x324
        VertexShadersPalSkinColourShadowBufferRender = shader_resource->GetVertexShader("VSHADER_PALSKIN_COLBUFF_SHADOW_BUFFER_RENDER");   // 00bd131f +0x1a0
        VertexShadersPalSkinColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1397 +0x1a8
        VertexShadersPalSkinColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd140f +0x1b0
        VertexShadersPalSkinColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1487 +0x1b8
        VertexShadersPalSkinColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd14ff +0x1c0
        VertexShadersPalSkinSpotColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1577 +0x228
        VertexShadersPalSkinSpotColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd15ef +0x230
        VertexShadersPalSkinSpotColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1667 +0x238
        VertexShadersPalSkinSpotColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd16df +0x240
        VertexShadersPalSkinEnvColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1757 +0x1c8
        VertexShadersPalSkinEnvColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd17cf +0x1d0
        VertexShadersPalSkinEnvColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1847 +0x1d8
        VertexShadersPalSkinEnvColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd18bf +0x1e0
        VertexShadersPalSkinEnvSpotColourBufferShadow[0] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1937 +0x248
        VertexShadersPalSkinEnvSpotColourBufferShadow[1] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd19af +0x250
        VertexShadersPalSkinEnvSpotColourBufferShadow[2] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1a27 +0x258
        VertexShadersPalSkinEnvSpotColourBufferShadow[3] = shader_resource->GetVertexShader("VSHADER_PALSKIN_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");   // 00bd1a9f +0x260
        VertexShadersPalSkinDP3BumpColourBufferShadow[0] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_FOG_COLBUFF_SHADOW");   // 00bd1b17 +0x208
        VertexShadersPalSkinDP3BumpColourBufferShadow[1] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1b91 +0x210
        VertexShadersPalSkinDP3BumpColourBufferShadow[2] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1c0b +0x218
        VertexShadersPalSkinDP3BumpColourBufferShadow[3] = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");   // 00bd1c85 +0x220
    }   // 0x00BD1CFF = jb target
    VertexShaders2DPalSkin = shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_DIRLIGHT");   // 00bd1cff +0x2c8
    VertexShaders2DPalSkinEnv = shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_ENV_DIRLIGHT");   // 00bd1d77 +0x2d0
    VertexShaders2DPalSkinDP3Bump = dp3_shader_resource->GetVertexShader("VSHADER_PALSKIN_2D_DP3_BUMP_DIRLIGHT");   // 00bd1df0 +0x2e0
    VertexShadersPalSkinShadow             = VertexShadersPalSkinDepthBufferShadow;
    VertexShadersPalSkinSpotShadow         = VertexShadersPalSkinSpotDepthBufferShadow;
    VertexShadersPalSkinShadowBufferRender = &VertexShadersPalSkinDepthShadowBufferRender;
    VertexShadersPalSkinEnvShadow          = VertexShadersPalSkinEnvDepthBufferShadow;
    VertexShadersPalSkinEnvSpotShadow      = VertexShadersPalSkinEnvSpotDepthBufferShadow;
    VertexShadersPalSkinEnvBumpShadow      = VertexShadersPalSkinEnvBumpDepthBufferShadow;
    VertexShadersPalSkinEnvBumpSpotShadow  = VertexShadersPalSkinEnvBumpSpotDepthBufferShadow;
    VertexShadersPalSkinDP3BumpShadow      = VertexShadersPalSkinDP3BumpDepthBufferShadow;
    TextureOverlayFadeDistandeFactor = 2.0f;
}