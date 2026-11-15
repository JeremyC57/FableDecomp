// CEngineLandscapeRenderer::CEngineLandscapeRenderer(const CEngineMemoryInit& init) -- construct the landscape renderer: members, default landscape settings and fade factors, the LOD Z bands, the themes, every landscape vertex / pixel shader by name (the colour-buffer shadow sets only for ps_1_4+), the shadowed-shader pointers, the blend tables and the landscape memory manager
// Leaf: ??0CEngineLandscapeRenderer@@QAE@ABVCEngineMemoryInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B69000, 8358 bytes, thiscall `ret 4` (two epilogues: the `new` null test), frame 0x10, no EH.
// Identity: the catalog label "CEnginePrimitiveManagerParticleSpriteGroup ctor" is WRONG: the row stores the CEngineLandscapeRenderer vtable
//   0x012A2B54 (rtti-vtables.tsv) and fills that class's members (+0x4B8..+0x6B0, engine/CEngineLandscapeRenderer.h); Ego_r.pdb / FableWin.pdb
//   ??0CEngineLandscapeRenderer@@QAE@ABVCEngineMemoryInit@@@Z, bound from the landed caller CEngine::Initialise 0x00B26360 (reloc_bindings).
// Spellings EVIDENCED by retail bytes: settings assigned in the body around CalcForegroundFadeFactors (stores cannot cross that call,
//   0x00B6926A / 0x00B6929D); the LOD Z bands as a loop over a running `z` doubled each step (the x87 chain scheduled after every settings
//   store, 0x00B6930B; the unrolled element-wise spellings interleave it); each shader fetched through GPResourceManager per statement
//   with an implicit const char* -> CCharString conversion (the name temporary built before the resource load, 0x00B6933E); the two
//   ps_1_4 tests as separate ifs (0x00B6A098 / 0x00B6ACEA); CalcForegroundFadeFactors' body in the TU (no `mov ecx,esi` before
//   BuildThemes, 0x00B69339). FITTED: none. Descriptive: the loop locals i / z.
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_CTOR
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GETSETTINGS
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_BLENDTABLES
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_CTOR
#define FABLE_RETAIL_CLANDSCAPETESSELATORBASE_INLINE_CTOR
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_CTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CPIXELSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngineMemoryInit.h"

namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;   // 0x01436E98
extern CDisplayManager* GPDisplayManager;           // 0x01436E1C
}

// Same-TU callee defined above the row (retail 0x00B671E0, the landed body's text on the shared header; RELOCATION_MATCH at its own
// address): VC7.1 sees it does not touch ecx, so the ctor calls BuildThemes without reloading `this` (0x00B69339).
void CEngineLandscapeRenderer::CalcForegroundFadeFactors()
{
    Settings.BackgroundFadeTransform.Initialise(1.0f / (Settings.ForegroundFadeEnd - Settings.ForegroundFadeStart), 0.0f, 0.0f,
                                                Settings.ForegroundFadeEnd / (Settings.ForegroundFadeStart - Settings.ForegroundFadeEnd) + 1.0f);
    Settings.ForegroundFadeTransform.Initialise(1.0f / (Settings.ForegroundFadeStart - Settings.ForegroundFadeEnd), 0.0f, 0.0f,
                                                Settings.ForegroundFadeStart / (Settings.ForegroundFadeEnd - Settings.ForegroundFadeStart) + 1.0f);
}

CEngineLandscapeRenderer::CEngineLandscapeRenderer(const CEngineMemoryInit& init)
{
    Settings.MeshDetailLevel = 2.0f;
    Settings.ProceduralTextureDetail = 3.0f;
    Settings.ForegroundFadeStart = 48.0f;
    Settings.ForegroundFadeEnd = 64.0f;
    CalcForegroundFadeFactors();
    Settings.DrawForeground = true;
    Settings.DrawBackground = true;
    Settings.DrawBackgroundWater = true;
    Settings.EnableUpdate = true;
    Settings.FogEnable = true;
    Settings.DrawDebugDisplay = false;
    Settings.MinimumFileBlockSize = 0x8000;
    Settings.LODUpdateMaxTime = 0.004f;
    Settings.ForegroundMaxPixelsPerTexel = 1.5f;
    Settings.EnableForegroundWithoutGameMap = true;
    Settings.BumpMappingEnabled = true;
    Settings.UpdateTesselation = true;
    Settings.StaticMapOcclusionEnable = false;
    LODZBandsZ[0] = 0.0f;
    LODZBandsZ[1] = 0.0f;
    float z = Settings.ForegroundFadeEnd;
    for (long i = 2; i < 8; ++i) {
        LODZBandsZ[i] = z;
        z *= 2.0f;
    }
    BuildThemes();
    ForegroundBlackoutVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_BLACKOUT_PASS");
    ForegroundVertexShaders.resize(6);
    ForegroundVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND");
    ForegroundVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS");
    ForegroundVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS");
    ForegroundVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS");
    ForegroundVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS");
    ForegroundVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS");
    ForegroundDepthBufferShadowedVertexShaders.resize(6);
    ForegroundDepthBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_SHADOWED");
    ForegroundDepthBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SHADOWED");
    ForegroundDepthBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SHADOWED");
    ForegroundDepthBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SHADOWED");
    ForegroundDepthBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SHADOWED");
    ForegroundDepthBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders.resize(6);
    ForegroundBumpDepthBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_BUMP_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_BUMP_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_BUMP_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_BUMP_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_BUMP_SHADOWED");
    ForegroundBumpDepthBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_BUMP_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders.resize(6);
    ForegroundSpotDepthBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_SPOT_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SPOT_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SPOT_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SPOT_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SPOT_SHADOWED");
    ForegroundSpotDepthBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_SPOT_SHADOWED");
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
        ForegroundColourBufferShadowedVertexShaders.resize(6);
        ForegroundColourBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_COLBUFF_SHADOWED");
        ForegroundColourBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_COLBUFF_SHADOWED");
        ForegroundColourBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_COLBUFF_SHADOWED");
        ForegroundColourBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_COLBUFF_SHADOWED");
        ForegroundColourBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_COLBUFF_SHADOWED");
        ForegroundColourBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders.resize(6);
        ForegroundBumpColourBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_BUMP_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_BUMP_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_BUMP_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_BUMP_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_BUMP_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_BUMP_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders.resize(6);
        ForegroundSpotColourBufferShadowedVertexShaders[0] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_SPOT_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders[1] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SPOT_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders[2] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_2LIGHTS_SPOT_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders[3] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SPOT_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders[4] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_4LIGHTS_SPOT_COLBUFF_SHADOWED");
        ForegroundSpotColourBufferShadowedVertexShaders[5] = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)12)->GetVertexShader("VSHADER_LANDSCAPE_FOREGROUND_5LIGHTS_SPOT_COLBUFF_SHADOWED");
    }
    BackgroundVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)11)->GetVertexShader("VSHADER_LANDSCAPE_BACKGROUND");
    BackgroundAlphaVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)11)->GetVertexShader("VSHADER_LANDSCAPE_BACKGROUND_ALPHA");
    ProcTextureVertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader("VSHADER_LANDSCAPE_PROC_TEXTURE");
    ForegroundDepthBufferShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_FOREGROUND_SHADOWED");
    ForegroundBumpDepthBufferShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_FOREGROUND_BUMP_SHADOWED");
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
        ForegroundColourBufferShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_FOREGROUND_COLBUFF_SHADOWED");
        ForegroundBumpColourBufferShadowedPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_FOREGROUND_BUMP_COLBUFF_SHADOWED");
    }
    ForegroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_FOREGROUND");
    ForegroundBlackoutPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DIFFUSE_ONLY");
    BackgroundPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_BACKGROUND");
    ProcTexturePixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_LANDSCAPE_PROC_TEXTURE");
    ForegroundShadowedPixelShader = &ForegroundDepthBufferShadowedPixelShader;
    ForegroundBumpShadowedPixelShader = &ForegroundBumpDepthBufferShadowedPixelShader;
    ForegroundShadowedVertexShaders = &ForegroundDepthBufferShadowedVertexShaders;
    ForegroundBumpShadowedVertexShaders = &ForegroundBumpDepthBufferShadowedVertexShaders;
    ForegroundSpotShadowedVertexShaders = &ForegroundSpotDepthBufferShadowedVertexShaders;
    BuildBlendingTables();
    MemoryManager = new CEngineLandscapeMemoryManager(init);
}