// CEngineSkyRenderer::CEngineSkyRenderer(const CEngineInit& init) -- construct the sky renderer: buffers, shaders, cloud vectors and settings, look up the sky shaders and the SKY def, set the star file paths and build the buffers
// Leaf: ??0CEngineSkyRenderer@@QAE@ABVCEngineInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B625E0, length 2604 bytes, thiscall `ret 4`, no EH frame (no /EHsc).
// Identity: Ego_r.pdb / FableWin.pdb ??0CEngineSkyRenderer@@QAE@ABVCEngineInit@@@Z; the body stores vtable 0x012A293C and touches
//   exactly the engine/CEngineSkyRenderer.h members (0x194 bytes; the FABLE_RETAIL_CENGINESKYRENDERER branch types them).
// Callees: CEngineComponent() 0x00B59710, CBaseClassNonCopyable() 0x0099A2F0 (the three inline CVertexBufferWin32 ctors),
//   CIndexBuffer() 0x00AC1EC0, CVertexShader() 0x00A62FB0, CPixelShader() 0x00A5EC00, CWideString() 0x0099AED0,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CShaderResource::GetVertexShader 0x00A5D5F0 /
//   GetPixelShader 0x00A5D720, CDefinitionManager::GetPDefFromClassIndex 0x009ADE10, CWideString::operator=(const wchar_t*)
//   0x0099B800, BuildBuffers 0x00B620A0; the def's Delete and the shader datas' deleting dtors are virtual (slot 1 / slot 0).
// Shape: members in declaration order (PSkyDef nulled, EnableClouds), then the cloud vectors as a nested loop: the inner
//   history loop unrolls, the outer stays a two-trip loop over two zero C2DVector temporaries on the stack (`mov ebp,[esp+10h]`
//   per store); a flat three-statement body is fully unrolled into register stores instead. Cloud settings come from the
//   engine's environment def (retail CEnvironmentDef = Ego_r - 4: CloudSpeedMultiplier +0x118 .. CloudHeightOffset +0x128; the
//   min speed is 0). Thirteen shader lookups in retail order -- the star-field vertex shader is fetched twice, and the cloud pixel
//   shader is the SIMPLE one when InitFlags bit 0x100 is set -- each a CCharString temporary, a by-value shader temporary, the
//   inline operator= and the temporary's inline dtor. Then StarCount = 0, the inlined GetDef<CSkyDef>("SKY", 1, &PSkyDef)
//   (the template body is in this TU, as in the landed 0x004084B0; its SetDynamicCast re-reads the source after the release:
//   the engine/CDefPointer.h overlay change), the two star paths and BuildBuffers().
// Data: g_pResourceManager 0x01436E98 (bank 5 +0x29E8, bank 6 +0x29F0, bank 16 +0x2A40 of VShaderResources[], PShader +0x2A98);
//   g_pEngineEnvironmentDef 0x01436E24 (established descriptive label; CEngine::Initialise 0x00B26360 stores CEngine +0x118
//   there); g_pEngineDefinitionManager 0x01436E28 (descriptive: CEngine::Initialise stores init.DefinitionManager there at
//   0x00B2637B); L"Data\\Misc\\star_chart.tga" 0x012A29A4, L"Data\\Misc\\stars.dat" 0x012A297C.
// Layout: CEngineInit and CEnvironmentDef have no engine header: local models. Retail CEngineInit has no DebugString (Ego_r +0x0C),
//   so InitFlags is +0x60 (Ego_r +0x64; the landed 0x0042E204 writes it there too).
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
class CEngineInit {                                                      // Ego_r.pdb CEngineInit, retail offsets
public:
    CDefinitionManager* DefinitionManager;                               // +0x00
    unsigned char       _pad04[0x60 - 0x04];                             // EngineDef .. MemoryInit (not read here)
    long                InitFlags;                                       // +0x60
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
class CEnvironmentDef {                                                  // Ego_r.pdb CEnvironmentDef members, retail = Ego_r - 4
public:
    unsigned char _pad000[0x118];
    float         CloudSpeedMultiplier;                                  // +0x118
    float         CloudMaxSpeed;                                         // +0x11c
    float         CloudTextureCoordMultiplier;                           // +0x120
    float         CloudTextureCoordOffset;                               // +0x124
    float         CloudHeightOffset;                                     // +0x128
};
extern const CEnvironmentDef* g_pEngineEnvironmentDef;                   // 0x01436E24 (descriptive)
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
    CloudSpeedMultiplier = g_pEngineEnvironmentDef->CloudSpeedMultiplier;
    CloudMaxSpeed = g_pEngineEnvironmentDef->CloudMaxSpeed;
    CloudMinSpeed = 0.0f;
    CloudTextureOffset = g_pEngineEnvironmentDef->CloudTextureCoordOffset;
    CloudTextureMulitplier = g_pEngineEnvironmentDef->CloudTextureCoordMultiplier;
    CloudHeightOffset = g_pEngineEnvironmentDef->CloudHeightOffset;

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
