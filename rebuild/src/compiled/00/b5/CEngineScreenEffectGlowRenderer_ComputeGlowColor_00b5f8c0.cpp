// CEngineScreenEffectGlowRenderer::ComputeGlowColor() -- read the current environment theme's glow settings, quantise its threshold and intensity colours to bytes and derive the per-channel threshold/intensity scalars and the motion-blur factor
// Leaf: ComputeGlowColor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5F8C0, length 496 bytes, plain `ret`.
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (current environment),
//   virtual slot 1 of that (the theme, whose glow settings pointer sits at +0x54); __ftol2 for the six byte quantisations.
// Layout: glow settings: threshold colour floats R,G,B at +0x100, intensity colour floats at +0x110, threshold blend +0x178,
//   intensity scale +0x17c, motion blur +0x180 (offsets read from the retail body); `this` is engine/CEngineScreenEffectGlowRenderer.h.
// Shape: the colours are CRGBColour locals (B,G,R,A bytes) built from `(unsigned char)(x * 255.0f)`; a negative blend flips
//   the threshold to `1 - c * |blend| / 255`, otherwise `c * blend / 255 + 1`.
#include "engine/CEngineScreenEffectGlowRenderer.h"


// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); the "environment provider at +0x198" is its GameWorld (FableWin.pdb inline
// PeekGameWorld, engine/CEngineWorldMap.h _GAMEWORLD), slot 1 CIDrawWorld::DrawGetEnvironment, then CIDrawEnvironment::PeekInterpolatedParams
// (engine/CIDrawWorld.h / CIDrawEnvironment.h); the "theme +0x54 glow settings pointer" is ThemeChannels[0] (CArray begin at +0x54) and
// the glow fields its Ego_r.pdb members: threshold Colours[16] +0x100, intensity Colours[17] +0x110, Scalers[18..20] +0x178..+0x180.
// Were the local SEngineState_01436e8c / g_pEngineState_01436e8c, CEnvironmentProvider, CEnvironment, CEnvironmentTheme, SGlowSettings models; the colour locals are engine/CRGBColour.h
// (FABLE_RETAIL_CRGBCOLOUR: FableWin.pdb ??0CRGBColour@@QAE@EEE@Z, inline), which replaces a local CRGBColour model.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

struct CEngineScreenEffectGlowRenderer_Methods : CEngineScreenEffectGlowRenderer {
    void ComputeGlowColor();
};

void CEngineScreenEffectGlowRenderer_Methods::ComputeGlowColor()
{
    const CIDrawEnvironmentParams& theme = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
    const CIDrawEnvironmentParams::CThemeChannel* settings = &theme.ThemeChannels[0];
    CRGBColour threshold((unsigned char)(settings->Colours[16].R * 255.0f),
                         (unsigned char)(settings->Colours[16].G * 255.0f),
                         (unsigned char)(settings->Colours[16].B * 255.0f));
    CRGBColour intensity((unsigned char)(settings->Colours[17].R * 255.0f),
                         (unsigned char)(settings->Colours[17].G * 255.0f),
                         (unsigned char)(settings->Colours[17].B * 255.0f));
    float blend = theme.ThemeChannels[0].Scalers[18];
    float scale = theme.ThemeChannels[0].Scalers[19];
    MotionBlur = 1.0f - theme.ThemeChannels[0].Scalers[20];
    if (blend >= 0.0f) {
        ThresholdR = (float)threshold.R * blend * (1.0f / 255.0f) + 1.0f;
        ThresholdG = (float)threshold.G * blend * (1.0f / 255.0f) + 1.0f;
        ThresholdB = (float)threshold.B * blend * (1.0f / 255.0f) + 1.0f;
    } else {
        blend = -blend;
        ThresholdR = 1.0f - (float)threshold.R * blend * (1.0f / 255.0f);
        ThresholdG = 1.0f - (float)threshold.G * blend * (1.0f / 255.0f);
        ThresholdB = 1.0f - (float)threshold.B * blend * (1.0f / 255.0f);
    }
    IntensityR = (float)intensity.R * scale * (1.0f / 255.0f);
    IntensityG = (float)intensity.G * scale * (1.0f / 255.0f);
    IntensityB = (float)intensity.B * scale * (1.0f / 255.0f);
}