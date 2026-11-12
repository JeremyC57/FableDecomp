// CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase(CArray<CEngineScene*>&) -- construct the static mesh
//   primitive manager: base ctor, member shader ctors, point-light shader index table, fetch every static / DP3-bump vertex shader
//   by name (colour-buffer shadow shaders only on ps_1_4+), the stipple-alpha pixel shader, then the shadow alias pointers.
// Leaf: ??0CEnginePrimitiveManagerStaticMeshesBase@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (fg.py, FABLE_HDR_OVERLAY=hdr/, strict headers, cl 13.10.3077, no flags)
// Retail: Fable.exe 0x00B9ECE0, 8921 bytes, __thiscall, ret 4, returns this.
// Callees: 0x00BEBC30 CEnginePrimitiveManagerMeshBase ctor, 0x00A62FB0 CVertexShader ctor, 0x00A5EC00 CPixelShader ctor,
//   0x0099EBF0 CCharString(const char*,long), 0x0099EAE0 ~CCharString, 0x00A5D5F0 CShaderResource::GetVertexShader,
//   0x00A5D720 CShaderResource::GetPixelShader, 0x009BEE00 CDisplayManager::GetPixelShaderVersion.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

namespace NEngineInternal
{
    extern CEngineResourceManager* GPResourceManager;   // 0x01436E98
    extern CDisplayManager* GPDisplayManager;           // 0x01436E1C
}

CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManagerMeshBase(scenes)
{
    PointLightShaderIndex[0] = 0;
    PointLightShaderIndex[1] = 1;
    PointLightShaderIndex[2] = 1;
    PointLightShaderIndex[3] = 2;
    PointLightShaderIndex[4] = 2;
    PointLightShaderIndex[5] = 3;

    const CShaderResource* static_shaders = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)1);
    const CShaderResource* bump_shaders = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)2);

    VertexShadersStatic[0]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG");
    VertexShadersStatic[1]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG");
    VertexShadersStatic[2]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG");
    VertexShadersStatic[3]                           = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShadersStaticEnv[0]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG");
    VertexShadersStaticEnv[1]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG");
    VertexShadersStaticEnv[2]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG");
    VertexShadersStaticEnv[3]                        = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG");
    VertexShaders2DStatic                            = static_shaders->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT");
    VertexShaders2DStaticEnv                         = static_shaders->GetVertexShader("VSHADER_2D_STATIC_ENV_DIRLIGHT");
    VertexShaderOutline                              = static_shaders->GetVertexShader("VSHADER_STATIC_OUTLINE");
    VertexShaderOutlineSmoothFill                    = static_shaders->GetVertexShader("VSHADER_STATIC_OUTLINE_SMOOTH_FILL");
    VertexShaderDepthShadowBufferRender              = static_shaders->GetVertexShader("VSHADER_STATIC_SHADOW_BUFFER_RENDER");
    VertexShadersStaticDepthBufferShadow[0]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SHADOW");
    VertexShadersStaticDepthBufferShadow[1]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticDepthBufferShadow[2]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticDepthBufferShadow[3]          = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticSpotDepthBufferShadow[0]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_SHADOW");
    VertexShadersStaticSpotDepthBufferShadow[1]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticSpotDepthBufferShadow[2]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticSpotDepthBufferShadow[3]      = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticEnvDepthBufferShadow[0]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SHADOW");
    VertexShadersStaticEnvDepthBufferShadow[1]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticEnvDepthBufferShadow[2]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticEnvDepthBufferShadow[3]       = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticEnvSpotDepthBufferShadow[0]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SPOT_SHADOW");
    VertexShadersStaticEnvSpotDepthBufferShadow[1]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticEnvSpotDepthBufferShadow[2]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticEnvSpotDepthBufferShadow[3]   = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_SHADOW");
    VertexShadersStaticDP3BumpDepthBufferShadow[0]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_SHADOW");
    VertexShadersStaticDP3BumpDepthBufferShadow[1]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticDP3BumpDepthBufferShadow[2]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_SHADOW");
    VertexShadersStaticDP3BumpDepthBufferShadow[3]   = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_SHADOW");
    VertexShaderDP3BumpOutline                       = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_OUTLINE");
    VertexShaders2DStaticDP3Bump                     = bump_shaders->GetVertexShader("VSHADER_2D_STATIC_DP3_BUMP_DIRLIGHT");
    VertexShaderDP3BumpOutlineSmoothFill             = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_OUTLINE_SMOOTH_FILL");
    VertexShadersStaticStippleAlpha[0]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_STIPPLE_ALPHA");
    VertexShadersStaticStippleAlpha[1]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_STIPPLE_ALPHA");
    VertexShadersStaticStippleAlpha[2]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_STIPPLE_ALPHA");
    VertexShadersStaticStippleAlpha[3]               = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_STIPPLE_ALPHA");
    VertexShadersStaticSpotlight[0]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_SPOT_LIGHT");
    VertexShadersStaticSpotlight[1]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_SPOT_LIGHT");
    VertexShadersStaticSpotlight[2]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_SPOT_LIGHT");
    VertexShadersStaticSpotlight[3]                  = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_SPOT_LIGHT");
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400)
    {
        VertexShadersStaticColourBufferShadow[0]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_COLBUFF_SHADOW");
        VertexShadersStaticColourBufferShadow[1]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticColourBufferShadow[2]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticColourBufferShadow[3]         = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticEnvColourBufferShadow[0]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_COLBUFF_SHADOW");
        VertexShadersStaticEnvColourBufferShadow[1]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticEnvColourBufferShadow[2]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticEnvColourBufferShadow[3]      = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticDP3BumpColourBufferShadow[0]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_FOG_COLBUFF_SHADOW");
        VertexShadersStaticDP3BumpColourBufferShadow[1]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_2POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticDP3BumpColourBufferShadow[2]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_4POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticDP3BumpColourBufferShadow[3]  = bump_shaders->GetVertexShader("VSHADER_STATIC_DP3_BUMP_DIRLIGHT_5POINTLIGHTS_FOG_COLBUFF_SHADOW");
        VertexShadersStaticSpotColourBufferShadow[0]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticSpotColourBufferShadow[1]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticSpotColourBufferShadow[2]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticSpotColourBufferShadow[3]     = static_shaders->GetVertexShader("VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticEnvSpotColourBufferShadow[0]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticEnvSpotColourBufferShadow[1]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_2POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticEnvSpotColourBufferShadow[2]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShadersStaticEnvSpotColourBufferShadow[3]  = static_shaders->GetVertexShader("VSHADER_STATIC_ENV_DIRLIGHT_5POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW");
        VertexShaderColourShadowBufferRender             = static_shaders->GetVertexShader("VSHADER_STATIC_COLBUFF_SHADOW_BUFFER_RENDER");
    }
    PixelShaderDiffuseFogStippleAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_STIPPLE_ALPHA");

    VertexShaderShadowBufferRender = &VertexShaderDepthShadowBufferRender;
    VertexShadersStaticShadow = VertexShadersStaticDepthBufferShadow;
    VertexShadersStaticSpotShadow = VertexShadersStaticSpotDepthBufferShadow;
    OverridePrimitive = 0;
    RenderOnlyNMeshes = -1;
}
