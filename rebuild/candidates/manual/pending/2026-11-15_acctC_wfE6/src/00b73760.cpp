// CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit& init) -- construct the water renderer: member defaults, high-end water from the init flags, look up every water / ice / sea vertex and pixel shader, clear the texture handles, then create the dynamic textures and the constant buffers
// Leaf: ??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B73760, 6799 bytes, thiscall, `ret 4`, no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z (vtable 0x012A3364 stored after CEngineComponent()).
// Callees: CEngineComponent() 0x00B59710, CIndexBuffer() 0x00AC1EC0, CBaseClassNonCopyable() 0x0099A2F0 (inline CVertexBufferWin32()), CVertexShader()
//   0x00A62FB0, CPixelShader() 0x00A5EC00, CCharString() 0x0099E4B0 / (const char*, -1) 0x0099EBF0 / ~ 0x0099EAE0, CEngineWaterSettings() 0x00BE9920,
//   CShaderResource::GetVertexShader / GetPixelShader 0x00A5D5F0 / 0x00A5D720, vector<CVertexShader>::_M_insert_overflow 0x00B6C120,
//   CDisplayManager::GetPixelShaderVersion 0x009BEE00, InitialiseDynamicTextures 0x00B6D510, BuildConstantBuffers 0x00B6EDF0.
// Member construction is the implicit one of the shared retail class (engine/CEngineWaterRenderer.h, declaration order; the two tesselators'
//   ctors and CVertexShader's copy ctor inline through new overlay opt-ins, see hdr/README.md). Statement order and bank numbers follow the
//   retail call sequence 0x00B73AFA..0x00B751DE; EnvMapOverlay* shaders are not looked up in retail. No flags, no fitted spelling.
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_CTOR
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CVERTEXSHADER_COPYCTOR
#define FABLE_RETAIL_CLANDSCAPETESSELATORBASE_INLINE_CTOR
#define FABLE_RETAIL_CWATERPATCHTESSELATORBASE_INLINE_CTOR
#define FABLE_RETAIL_CENGINEWATERSETTINGS_CTOR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineInit.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }            // 0x01436E1C

CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit& init)
{
    if (init.InitFlags & 0x80) {
        HighendWaterSupported = true;
        HighendWaterEnabled = true;
    } else {
        HighendWaterSupported = false;
        HighendWaterEnabled = false;
    }
    ForegroundVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_WATER_FOREGROUND");
    BackgroundVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)8)->GetVertexShader("VSHADER_WATER_BACKGROUND");
    IceForegroundVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND"));
    IceForegroundVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_1_LIGHTS"));
    IceForegroundVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_2_LIGHTS"));
    IceForegroundVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_3_LIGHTS"));
    IceForegroundShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_SHADOWED"));
    IceForegroundShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_SHADOWED_1_LIGHTS"));
    IceForegroundShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_SHADOWED_2_LIGHTS"));
    IceForegroundShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_SHADOWED_3_LIGHTS"));
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
        IceForegroundColbuffShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED"));
        IceForegroundColbuffShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_1_LIGHTS"));
        IceForegroundColbuffShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_2_LIGHTS"));
        IceForegroundColbuffShadowedVertexShader.push_back(NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED_3_LIGHTS"));
    }
    IceBackgroundVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)8)->GetVertexShader("VSHADER_ICE_BACKGROUND");
    SeaBackgroundVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)9)->GetVertexShader("VSHADER_SEA_BACKGROUND");
    SeaForegroundReflectiveVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_SEA_FOREGROUND_REFLECTIVE");
    SeaForegroundNonReflectiveVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)7)->GetVertexShader("VSHADER_SEA_FOREGROUND_NON_REFLECTIVE");
    BumpmapVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_WATER_BUMPMAP");
    IceBumpmapVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_ICE_BUMPMAP");
    BumpmapRainVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_WATER_BUMPMAP_RAIN");
    EnvironmentMapVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_WATER_ENVIRONMENT_MAP");
    SkyMapVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_WATER_SKY_MAP");
    ForegroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_FOREGROUND");
    BackgroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_BACKGROUND");
    IceForegroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_ICE_FOREGROUND");
    IceForegroundShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_ICE_FOREGROUND_SHADOWED");
    RefractionForegroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_REFRACTION_FOREGROUND");
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400)
        IceForegroundColbuffShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_ICE_FOREGROUND_COLBUFF_SHADOWED");
    SeaBackgroundLowEndVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)9)->GetVertexShader("VSHADER_SEA_BACKGROUND_LOW_END");
    WaterBackgroundLowEndVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)8)->GetVertexShader("VSHADER_WATER_BACKGROUND_LOW_END");
    WaterLowEndPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_LOW_END");
    IceBackgroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_ICE_BACKGROUND");
    SeaBackgroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SEA_BACKGROUND");
    SeaForegroundReflectivePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SEA_FOREGROUND_REFLECTIVE");
    SeaForegroundNonReflectivePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SEA_FOREGROUND_NON_REFLECTIVE");
    BumpmapPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_BUMPMAP");
    IceBumpmapPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_ICE_BUMPMAP");
    BumpmapRainPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_BUMPMAP_RAIN");
    EnvironmentMapPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_ENVIRONMENT_MAP");
    SkyMapPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_WATER_SKY_MAP");
    DynamicTexturePageHandle = 0;
    EnvironmentMap = 0;
    SeaBumpMap = 0;
    WaterBumpMap = 0;
    IceBumpMap = 0;
    HaveBackgroundSea = false;
    WaterGenerator = 0;
    StaticMapBankFile = 0;
    WaterBumpFactor = 1.5f;
    InitialiseDynamicTextures();
    BuildConstantBuffers();
}
