// CEngineWaterRenderer::RenderSeaMidground(float time) -- bind the sea bump map and the environment map on stages 0-2 with their texture address modes, pick the high-end or low-end sea background shaders, upload the fog constant and either the bump/sea-colour (high end) or low-end colour constants, then draw the alphaed and the opaque sea midground lists
// Residue: DIFFER(2019v2019) md 14 -- the CRenderStateManager bookmark test/or operand order: retail keeps `test eax,esi / or esi,eax` for the first five UV setters and switches at the 6th (+0x2FAC), ours switches at the 2nd (+0x2F84); plus the AlphaBlendEnable(false) block loads CurrentBookmarkMask after the state mask (retail first). c2 canonicalises the commutative pair (source order `ms` inert) by a key that moves with the IL TUPLE count before the site (symbols inert: 60 unused locals; a same-symbol `dsum +=` chain moves it like dead locals), function-local (dummy functions before/after inert), periodic (~280 dead-load units). Probe: removing one AttachTextureToStage -> A A A B B B, two -> A A A A B B, the capture block -> A A A B B B; +20 dead loads -> B x6, +60 -> B B B B A A, +90..215 -> A x6, +240 -> A A A A B B. 52 grades (shared-header rewrite, constant/stage-loop/local spellings, one-level / plain-inline / if-else / GetD3DTexture attach, mask local (B x6), swapped source operands, copy-init capture, unused locals k=1,2,60, TU dummies).
// Next: retail has fewer IL tuples before the first setter than our capture block + three two-level AttachTextureToStage expansions (about two and a half attaches' worth) or ~250 dead-load units more; find the c2 key (likely a value-number hash compared mod a table size) with a c2.dll trace at the commutative canonicalisation, then a natural spelling with that count -- the same lane gap as 0x00B70230 / 0x00B761D0 / 0x00B751F0 (CRenderStateManager.h).
// Leaf: ?RenderSeaMidground@CEngineWaterRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6FA40, length 2019 bytes (+ int3 padding), `ret 4`.
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
#define FABLE_RETAIL_CENGINEWATERSETTINGS_COLOUR
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineWaterRenderer.h"
#include "engine/CEngineLightingManager.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C

struct CEngineFogView {                            // @ 0x01436EA8
    unsigned char _pad0[0x62c];
    float FogStart;                                // +0x62C
    unsigned char _pad630[0x638 - 0x630];
    float FogEnd;                                  // +0x638
};
extern CEngineFogView* g_pEngineFog;               // 0x01436EA8
struct SThemeSettings {
    unsigned char _pad000[0x138];
    float         SeaReflectionAlpha;              // +0x138
};
struct CEnvironmentTheme {
    unsigned char   _pad[0x54];
    SThemeSettings* m_pSettings;                   // +0x54
};
class CEnvironment { public:
    virtual void v00();
    virtual CEnvironmentTheme* GetTheme();         // slot 1
};
struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};
struct SEngineState {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState* g_pEngineState;               // 0x01436E8C

void CEngineWaterRenderer::RenderSeaMidground(float time)
{
    CRenderStateScopedCaptureBlock capture;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, SeaBumpMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(2, EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
    if (HighendWaterEnabled) {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(SeaBackgroundPixelShader);
    } else {
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(SeaBackgroundLowEndVertexShader, false);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(WaterLowEndPixelShader);
    }
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SeaBumpmapSpan, g_pEngineFog->FogStart, g_pEngineFog->FogEnd, 1.0f);
    if (HighendWaterEnabled) {
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
        C4DVector colour;
        colour.X = Settings.Colour.R;
        colour.Y = Settings.Colour.G;
        colour.Z = Settings.Colour.B;
        colour.W = g_pEngineState->m_pEnvironmentProvider->GetCurrentEnvironment()->GetTheme()->m_pSettings->SeaReflectionAlpha
                 * Settings.ColourToReflectionFactor;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, colour);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * g_pEngineLightingManager->ThemeChannels[0].DiffuseColour;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x204);
    RenderBackgroundList(SeaAlphaedMidgroundRenderList, time);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    RenderBackgroundList(SeaMidgroundRenderList, time);
}
