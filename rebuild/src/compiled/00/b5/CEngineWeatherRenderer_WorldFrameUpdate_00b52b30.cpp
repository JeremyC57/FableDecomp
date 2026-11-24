// CEngineWeatherRenderer::WorldFrameUpdate(bool) -- per-frame weather update: unless forced, take the rain/snow/mist strengths from the current environment (honouring overrides), then on a full frame advance the particle time histories and the wind vector history
// Leaf: ?WorldFrameUpdate@CEngineWeatherRenderer@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B52B30, 665 bytes, `ret 4`. Callees: StartRaining 0x00B51EA0, StartSnowing 0x00B51F00, FadeRainToTarget 0x00B52870, FadeSnowToTarget 0x00B52900;
//   environment provider slot 1 / environment slot 1 through the engine state 0x01436E8C; engine globals 0x01436E48 (wind direction +0x628, speed +0x644, full-frame flag +0x650).
// Layout: CEngineWeatherSettings embedded at +0x114 (Ego_r.pdb field offsets + 0x114); the PDB header lists it as an opaque Settings blob, so the members are modelled flat.
// Shape: C3DVector needs a user copy ctor and operator* with a named result (no NRVO in VC7.1): retail keeps the ctor temporary and copies it.
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); the "environment provider at +0x198" is its GameWorld (FableWin.pdb inline
// PeekGameWorld, engine/CEngineWorldMap.h _GAMEWORLD), slot 1 CIDrawWorld::DrawGetEnvironment, the environment's slot 1
// CIDrawEnvironment::PeekInterpolatedParams; the "+0x54 weather record" is ThemeChannels[0] (CArray begin) and the strengths its Ego_r.pdb
// Scalers[14] / [15] / [16] (+0x168 snow, +0x16C rain, +0x170 mist). Were local SEngineState_01436e8c / CEnvironmentProvider /
// CEnvironment / SWindRecord / SWeatherStrengths stand-ins; the local C3DVector model is engine/C3DVector.h (its user copy ctor; the
// scaled copy built in a named local is the header's FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT opt-in, the same text).
#define FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_GAMEWORLD
#include "engine/C3DVector.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CIDrawWorld.h"
#include "engine/CIDrawEnvironment.h"
namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C
struct SEngineGlobals_01436e48 {
    unsigned char _pad[0x628];
    C3DVector     WindDirection;                   // +0x628
    unsigned char _pad634[0x10];
    float         WindSpeed;                       // +0x644
    unsigned char _pad648[8];
    bool          FullFrame;                       // +0x650
};
extern SEngineGlobals_01436e48* g_pEngineGlobals_01436e48;   // 0x01436E48

static inline float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

class CEngineWeatherRenderer { public:
    unsigned char _pad04[4];
    bool          Initialised;                     // +0x08
    unsigned char _pad09[0x110];
    bool          ForceWeatherEffects;             // +0x119
    unsigned char _pad11a[0x12];
    float         RainSpeedMultiplier;             // +0x12c
    unsigned char _pad130[0x2c];
    float         SnowSpeedMultiplier;             // +0x15c
    unsigned char _pad160[0x28];
    float         MistMaxAlpha;                    // +0x188
    unsigned char _pad18c[0x3c];
    float         MistSpeedMultiplier;             // +0x1c8
    float         MistWindSpeedMultiplier;         // +0x1cc
    float         ParticleTimeIncrement;           // +0x1d0
    unsigned char _pad1d4[0xc];
    float         WeatherWindMaxSpeed;             // +0x1e0
    float         WeatherWindMinSpeed;             // +0x1e4
    float         WeatherWindSpeedMultiplier;      // +0x1e8
    unsigned char _pad1ec[0x3dc];
    float         CurrentRainStrength;             // +0x5c8
    float         CurrentSnowStrength;             // +0x5cc
    bool          Raining;                         // +0x5d0
    bool          Snowing;                         // +0x5d1
    bool          Mist;                            // +0x5d2
    unsigned char _pad5d3[0xd];
    float         RainTimePassedHistory[2];        // +0x5e0
    float         SnowTimePassedHistory[2];        // +0x5e8
    float         MistTimePassedHistory[2];        // +0x5f0
    unsigned char _pad5f8[0xc];
    C3DVector     WindVectorHistory[2];            // +0x604
    unsigned char _pad61c[0x12e8 - 0x61c];
    float         OverridenRainStrength;           // +0x12e8
    bool          RainOverriden;                   // +0x12ec
    float         OverridenSnowStrength;           // +0x12f0
    bool          SnowOverriden;                   // +0x12f4
    void StartRaining(float strength);             // 0x00B51EA0
    void StartSnowing(float strength);             // 0x00B51F00
private:
    void FadeRainToTarget(float a0);         // 0x00B52870
void FadeSnowToTarget(float a0);         // 0x00B52900
    public:
    virtual void WorldFrameUpdate(bool full);
    private:
};

void CEngineWeatherRenderer::WorldFrameUpdate(bool full)
{
    if (!Initialised)
        return;
    if (!ForceWeatherEffects) {
        const CIDrawEnvironmentParams& env = NEngineInternal::GPEngineWorldMap->PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams();
        if (!RainOverriden) {
            float rain = env.ThemeChannels[0].Scalers[15];
            if (OverridenRainStrength < 0.0f) {
                StartRaining(rain);
            } else {
                FadeRainToTarget(rain);
                if ((float)fabs(CurrentRainStrength - rain) < 0.01f)
                    OverridenRainStrength = -1.0f;
            }
        } else {
            FadeRainToTarget(OverridenRainStrength);
        }
        if (!SnowOverriden) {
            float snow = env.ThemeChannels[0].Scalers[14];
            if (OverridenSnowStrength < 0.0f) {
                StartSnowing(snow);
            } else {
                FadeSnowToTarget(snow);
                if ((float)fabs(CurrentSnowStrength - snow) < 0.01f)
                    OverridenSnowStrength = -1.0f;
            }
        } else {
            FadeSnowToTarget(OverridenSnowStrength);
        }
        float mist = env.ThemeChannels[0].Scalers[16];
        if (mist <= 0.0f) {
            Mist = false;
        } else {
            MistMaxAlpha = mist;
            Mist = true;
        }
    }
    if (full || g_pEngineGlobals_01436e48->FullFrame) {
        RainTimePassedHistory[0] = RainTimePassedHistory[1];
        SnowTimePassedHistory[0] = SnowTimePassedHistory[1];
        MistTimePassedHistory[0] = MistTimePassedHistory[1];
        RainTimePassedHistory[1] += RainSpeedMultiplier * ParticleTimeIncrement;
        WindVectorHistory[0] = WindVectorHistory[1];
        SnowTimePassedHistory[1] += SnowSpeedMultiplier * ParticleTimeIncrement;
        MistTimePassedHistory[1] += MistWindSpeedMultiplier * MistSpeedMultiplier * ParticleTimeIncrement;
        WindVectorHistory[1] = g_pEngineGlobals_01436e48->WindDirection * Clamp(g_pEngineGlobals_01436e48->WindSpeed * WeatherWindSpeedMultiplier, WeatherWindMinSpeed, WeatherWindMaxSpeed);
    }
}