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

struct CRGBColour {                                // B,G,R,A bytes over a packed 32-bit colour
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b) { R = r; G = g; B = b; }
};

struct SFloatColour { float R, G, B; };

struct SGlowSettings {
    unsigned char _pad000[0x100];
    SFloatColour  ThresholdColour;                 // +0x100
    unsigned char _pad10c[0x4];
    SFloatColour  IntensityColour;                 // +0x110
    unsigned char _pad11c[0x178 - 0x11c];
    float         ThresholdBlend;                  // +0x178
    float         IntensityScale;                  // +0x17c
    float         MotionBlur;                      // +0x180
};

struct CEnvironmentTheme {
    unsigned char  _pad[0x54];
    SGlowSettings* m_pGlowSettings;                // +0x54
};

struct CEnvironment {
    virtual void v00();
    virtual CEnvironmentTheme* GetTheme();         // slot 1
};

struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};

struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

struct CEngineScreenEffectGlowRenderer_Methods : CEngineScreenEffectGlowRenderer {
    void ComputeGlowColor();
};

void CEngineScreenEffectGlowRenderer_Methods::ComputeGlowColor()
{
    const CEnvironmentTheme* theme = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetTheme();
    const SGlowSettings* settings = theme->m_pGlowSettings;
    CRGBColour threshold((unsigned char)(settings->ThresholdColour.R * 255.0f),
                         (unsigned char)(settings->ThresholdColour.G * 255.0f),
                         (unsigned char)(settings->ThresholdColour.B * 255.0f));
    CRGBColour intensity((unsigned char)(settings->IntensityColour.R * 255.0f),
                         (unsigned char)(settings->IntensityColour.G * 255.0f),
                         (unsigned char)(settings->IntensityColour.B * 255.0f));
    float blend = theme->m_pGlowSettings->ThresholdBlend;
    float scale = theme->m_pGlowSettings->IntensityScale;
    MotionBlur = 1.0f - theme->m_pGlowSettings->MotionBlur;
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