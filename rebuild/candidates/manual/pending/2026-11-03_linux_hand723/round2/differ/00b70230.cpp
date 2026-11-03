// CEngineWaterRenderer::RenderSeaBackground(float time) -- open a render-state capture block, bind the sea bump map (stage 0) and the environment map (stage 1) with their address modes and the stage-0 mip bias, pick the high-end or low-end sea background shaders, select the sea background vertex / index buffers, upload the sea background shader constants (bump span, sea colour + reflection alpha, bump factor and a camera-centred world matrix on the high end; two static low-end colours on the low end), flush the shader state and draw the sea background mesh
// Grade: DIFFER(2138v2138) -- 3 of the 7 inlined CRenderStateManager::SetRenderState blocks (stage-0 U / V and stage-1 U address modes, the bookmark tests @0x00B70377 / 0x00B703E3 / 0x00B7044F) pick the other commutative operand order for the bookmark test/or (retail `test eax,esi ; or esi,eax`, built `test esi,eax ; or eax,esi`); every other instruction matches (lane gap: no spelling moves it; with a `mask` local 2 blocks differ instead but SetSpecularEnable's load order breaks in 0x00B751F0, see engine/CRenderStateManager.h)
// Leaf: ?RenderSeaBackground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B70230, length 2138 bytes, `ret 4` (time unused).
// Name: Ego_r.pdb ?RenderSeaBackground@CEngineWaterRenderer@@AAEXM@Z (catalog RenderSeaBackground; the sea background buffers
//   +0x68 / +0x7C, counts +0xB0 / +0xB4 and shaders +0xBC / +0xC4 / +0x114 / +0x124 are the Sea* members of the typed class).
// Shape: the three Ego_r.pdb private helpers this pass expands are defined in the TU, as retail's TU holds them:
//   SetSeaBackgroundTexturesAndUVModes (no retail copy) and SetupSeaBackgroundShaderConstantsAndFog (out-of-line copy 0x00B7C510,
//   landed as "SetupSeaMidgroundShaderConstantsAndFog": its body is byte-for-byte the inline block 0x00B70704..0x00B70A59 here;
//   RenderSeaMidground 0x00B6FA40 expands a different block, see NAME_FIXES.tsv); both `__forceinline` (an `inline` definition is
//   not expanded at this size). The capture block is the RAII CRenderStateCaptureBlock (ctor = StartCaptureBlock, dtor =
//   RestoreCaptureBlock 0x00A05840). The reflection alpha is read through a named `const CIDrawEnvironmentParams&` (the inline
//   CArray index on the call result otherwise materialises `add eax,54h` and loads the bump factor first).
// Callees: CRenderStateManager::RestoreCaptureBlock 0x00A05840; CShaderRenderManager::ApplyVertexShader 0x00988020,
//   ApplyPixelShader 0x00988140, SetUserVertexShaderConstant 0x00989B00, SetWorldTransform 0x009881F0 and the PrepareForRender(0x205)
//   callees (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateWorldSpaceFogTransform
//   0x00989640, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform
//   0x00988A50, UpdateCameraPos 0x00989700); CRenderManagerCore::DrawCurrentBuffersIndexed 0x00A0AD40; IDirect3DDevice9 SetTexture /
//   SetStreamSource / SetIndices through the vtable; CIDrawWorld::DrawGetEnvironment / CIDrawEnvironment::PeekInterpolatedParams (slot 1 each).
// Globals: CRenderStateCaptureBlock::StateManager 0x013CB508, g_pRenderManagerCore 0x01436E18, g_pShaderRenderManager 0x01436E14,
//   g_pEngineWorldMap 0x01436E8C, g_pEngineLightingManager 0x01436E9C, g_pEngineCamera 0x01436EA0; the TEXTURE_UV_MODE -> D3D
//   address-mode table 0x01396DC8 (g_textureUVModeToD3D, descriptive: the PDB setters take the enum, entries WRAP / CLAMP read here); the stage-0 mip bias float 0x0139A838 (-4.0f, .data, read once: descriptive name);
//   function statics @ 0x01437874 / 0x01437864 (guard 0x01437884), float pool -0.5f @ 0x01238798.
// Headers: needs the hand-back overlay (CRenderStateManager / CRenderManagerCore / CShaderRenderManager / CEngineWaterRenderer /
//   CEngineWorldMap / CEngineCamera / CEngineLightingManager retail branches, CIDrawWorld / CIDrawEnvironment(Params), C4DVector,
//   CRGBFloatColour, CMatrix3x4, CPreTransposedBoneMatrix).
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"

extern CRenderManagerCore* g_pRenderManagerCore;         // 0x01436E18
extern CShaderRenderManager* g_pShaderRenderManager;     // 0x01436E14
extern CEngineWorldMap* g_pEngineWorldMap;               // 0x01436E8C
extern CEngineLightingManager* g_pEngineLightingManager;       // 0x01436E9C
extern CEngineCamera* g_pEngineCamera;                   // 0x01436EA0
extern float g_seaBackgroundMipmapLODBias;               // 0x0139A838 (-4.0f)

__forceinline bool CEngineWaterRenderer::SetSeaBackgroundTexturesAndUVModes()
{
    g_pRenderManagerCore->AttachTextureToStage(0, SeaBumpMap);
    g_pRenderManagerCore->AttachTextureToStage(1, EnvironmentMap);
    g_pRenderManagerCore->AttachTextureToStage(1, EnvironmentMap);
    g_pRenderManagerCore->SetTextureUVModeU(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeV(0, TEXTURE_UV_MODE_WRAP);
    g_pRenderManagerCore->SetTextureUVModeU(1, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeV(1, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeU(2, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureUVModeV(2, TEXTURE_UV_MODE_CLAMP);
    g_pRenderManagerCore->SetTextureMipmapLODBias(0, g_seaBackgroundMipmapLODBias);
    return true;
}

__forceinline void CEngineWaterRenderer::SetupSeaBackgroundShaderConstantsAndFog()
{
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SeaBumpmapSpan, 0.0f, 0.0f, 0.0f);
    if (HighendWaterEnabled) {
        C4DVector colour;
        colour.X = Settings.Colour.R;
        colour.Y = Settings.Colour.G;
        colour.Z = Settings.Colour.B;
        const CIDrawEnvironmentParams& params = g_pEngineWorldMap->GameWorld->DrawGetEnvironment()->PeekInterpolatedParams();
        colour.W = params.ThemeChannels[0].Scalers[2] * Settings.ColourToReflectionFactor;
        g_pShaderRenderManager->SetPixelShaderConstant(7, colour);
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
        CMatrix3x4 world;
        world.SetIdentity();
        world.SetTranslateOnly(g_pEngineCamera->CameraDesc.WorldPosition);
        g_pShaderRenderManager->SetWorldTransform(world);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * g_pEngineLightingManager->ThemeChannels[0].DiffuseColour;
        g_pShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    g_pShaderRenderManager->PrepareForRender(0x205);
}

void CEngineWaterRenderer::RenderSeaBackground(float time)
{
    CRenderStateCaptureBlock captureBlock;
    SetSeaBackgroundTexturesAndUVModes();
    if (HighendWaterEnabled) {
        g_pShaderRenderManager->ApplyVertexShader(SeaBackgroundVertexShader, false);
        g_pShaderRenderManager->ApplyPixelShader(SeaBackgroundPixelShader);
    } else {
        g_pShaderRenderManager->ApplyVertexShader(SeaBackgroundLowEndVertexShader, false);
        g_pShaderRenderManager->ApplyPixelShader(WaterLowEndPixelShader);
    }
    g_pRenderManagerCore->SelectVertexBuffer(0, &SeaBackgroundVertexBuffer);
    g_pRenderManagerCore->SelectIndexBuffer(&SeaBackgroundIndexBuffer, 0);
    SetupSeaBackgroundShaderConstantsAndFog();
    g_pRenderManagerCore->DrawCurrentBuffersIndexed(0, SeaBackgroundVertexCount, 0, SeaBackgroundPolyCount);
}
