// CEngineSkyRenderer::CEngineSkyRenderer(const CEngineInit& init) -- construct the sky renderer: buffers, shaders, cloud vectors and settings, look up the sky shaders and the SKY def, set the star file paths and build the buffers
// Leaf: ??0CEngineSkyRenderer@@QAE@ABVCEngineInit@@@Z
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "engine/CEngineSkyRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CDefinitionManager.h"

enum EShaderBankNo;
// CEngineInit (Ego_r.pdb, 0x68 bytes there): DefinitionManager +0x00; retail reads a flag word at +0x60 (InitFlags; Ego_r has it
// at +0x64 after a ten-long CEngineMemoryInit, so one Ego_r memory field is absent from retail).
class CEngineInit {
public:
    class CDefinitionManager* DefinitionManager;                         // +0x00
    unsigned char             _pad04[0x60 - 0x04];
    unsigned long             InitFlags;                                 // +0x60
};
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, long index, CDefPointer<const T>* def) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> base = GetPDefFromClassIndex(name, index);
    if (!base.IsNull()) {
        def->SetDynamicCast(base);
        return true;
    }
    return false;
}
// CEngineDef: the engine def the engine publishes at 0x01436E24 (CEngine::Initialise 0x00B268AF); the cloud fields are descriptive.
class CEngineDef {
public:
    unsigned char _pad000[0x118];
    float         CloudSpeedMultiplier;                                  // +0x118
    float         CloudMaxSpeed;                                         // +0x11c
    float         CloudTextureMulitplier;                                // +0x120
    float         CloudTextureOffset;                                    // +0x124
    float         CloudHeightOffset;                                     // +0x128
};
extern const CEngineDef* g_pEngineDef;                                   // 0x01436E24 (descriptive)
extern CDefinitionManager* g_pEngineDefinitionManager;                   // 0x01436E28 (descriptive)
extern CEngineResourceManager* g_pResourceManager;                       // 0x01436E98

CEngineSkyRenderer::CEngineSkyRenderer(const CEngineInit& init)
{
    EnableClouds = true;
    for (long i = 0; i < 2; ++i) {
        for (long j = 0; j < 2; ++j)
            CloudVectorHistory[i][j] = C2DVector(0.0f, 0.0f);
        CloudVector[i] = C2DVector(0.0f, 0.0f);
    }
    CloudSpeedMultiplier = g_pEngineDef->CloudSpeedMultiplier;
    CloudMaxSpeed = g_pEngineDef->CloudMaxSpeed;
    CloudMinSpeed = 0.0f;
    CloudTextureOffset = g_pEngineDef->CloudTextureOffset;
    CloudTextureMulitplier = g_pEngineDef->CloudTextureMulitplier;
    CloudHeightOffset = g_pEngineDef->CloudHeightOffset;

    CloudVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)5)->GetVertexShader("VSHADER_INNER_SKY");
    SkyVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)5)->GetVertexShader("VSHADER_OUTER_SKY");
    StarFieldVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)16)->GetVertexShader("VSHADER_SKY_STAR_FIELD");
    StarFieldVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)16)->GetVertexShader("VSHADER_SKY_STAR_FIELD");
    StarFieldPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SKY_STAR_FIELD");
    SkySpriteVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)5)->GetVertexShader("VSHADER_SKY_SPRITE");
    SkyPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_OUTER_SKY");
    if (init.InitFlags & 0x100)
        CloudPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_INNER_SKY_SIMPLE");
    else
        CloudPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_INNER_SKY");
    SkySpritePixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SKY_SPRITE");
    BaseBandVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)5)->GetVertexShader("VSHADER_SKY_BASE_BAND");
    BaseBandPixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DIFFUSE_ONLY");
    ScreenSpaceSpriteVertexShader = g_pResourceManager->GetVShaderResource((EShaderBankNo)6)->GetVertexShader("VSHADER_SKY_SCREEN_SPACE_SPRITE");
    ScreenSpaceSpritePixelShader = g_pResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SKY_SPRITE");

    StarCount = 0;
    g_pEngineDefinitionManager->GetDef("SKY", 1, &PSkyDef);
    StarBitmapFilePath = L"Data\\Misc\\star_chart.tga";
    StarVBFilePath = L"Data\\Misc\\stars.dat";
    BuildBuffers();
}
