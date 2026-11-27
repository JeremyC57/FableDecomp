// Leaf: ??0CEngineWeatherSettings@@QAE@XZ
// Flags: (none)
// Inline-budget: FableWin calls GFFloatRandom at all four sites. Retail calls it in the wind loop and expands it in the speed loop.
// With plain C++ the leaf is 1157 bytes, /d2inlS1 gives 1157, and /d2inlT10 gives 970. Limiting the wind loop alone gives 1018.
// Retail: 0x00B50FB0 through 0x00B513A9 inclusive. Ego_r 0x009F8450 and FableWin 0x02D32B10 identify the constructor.
// FableWin 0x02D32C9C and 0x02D32CAB both store RainDropMinSize.Y. The native 0x00B50BE0 scalar deletes through the base destructor.
#define FABLE_RETAIL_C3DCOORDI_CTOR_WEATHER_PASS29
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_BOXCENTER
#define FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFLOATRANDOM_INLINE
#include "engine/CEngineWeatherSettings.h"
#include "engine/GFFloatRandom.h"
inline CEngineWeatherSettings::~CEngineWeatherSettings() {}

CEngineWeatherSettings::CEngineWeatherSettings()
{
    Enable = true;
    ForceWeatherEffects = false;
    ParticleBoxLocked = false;
    ParticleBoxZLocked = false;
    RainSpeedMultiplier = 1.0f;
    RainWindSpeedMultiplier = 1.0f;
    RainWeighting = 1.0f;
    RainDropSize.X = 0.5f;
    RainDropSize.Y = 0.5f;
    RainDropMinSize.Y = 0.2f;
    RainDropMinSize.Y = 0.2f;
    RainStretchFactor = 0.0f;
    RainFadeStart = 10.0f;
    RainFadeEnd = 15.0f;
    RainMaxAlpha = 0.2f;
    RainVolumeDepthSelectMultiplier = 0.6f;
    SnowWeighting = -1.0f;
    SnowSpeedMultiplier = 0.2f;
    SnowWindSpeedMultiplier = 0.2f;
    SnowflakeBaseSize = 0.03f;
    SnowflakeScaleOffset = 1.0f;
    SnowflakeScaleDistanceFactor = 1.0f;
    SnowflakeScaleDistanceSqrFactor = 0.0f;
    SnowflakeScaleMax = 64.0f;
    SnowflakeScaleMin = 0.0f;
    SnowFadeStart = 0.0f;
    SnowFadeEnd = 35.0f;
    SnowMaxAlpha = 0.5f;
    ParticleBoxSpan = C3DVector(8.0f, 8.0f, 8.0f);
    ParticleTimeIncrement = 0.1f;
    WindSpeedFactor = 0.05f;
    MinWindSpeed = 0.0f;
    MaxWindSpeed = 0.005f;
    MistMaxAlpha = 0.4f;
    MistTextureSpan.X = 0.1f;
    MistTextureSpan.Y = 0.1f;
    MistTextureSpan.Z = 0.1f;
    MistVertexSpan = C3DVector(5.0f, 5.0f, 0.0f);
    MistVertexSpan2 = C3DVector(7.0f, 7.0f, 0.1f);
    MistVertexSpan3 = C3DVector(8.0f, 8.0f, 0.2f);
    MistVertexSpan4 = C3DVector(9.0f, 9.0f, 0.3f);
    MistSpeedMultiplier = 1.0f;
    MistWindSpeedMultiplier = 0.01f;
    WeatherWindMinSpeed = 0.0f;
    WeatherWindMaxSpeed = 0.0f;
    WeatherWindSpeedMultiplier = 0.15f;
    RandomNumberGeneratorSeed = 0x29;
    ParticleBoxCenter = C3DCoordI(0, 0, 0);
    for (long c_i = 0; c_i < 81; ++c_i)
        ParticleBoxPos[c_i] = C3DVector(0.0f, 0.0f, 0.0f);
    CurrentRainStrength = 0.0f;
    CurrentSnowStrength = 0.0f;
    Raining = false;
    Snowing = false;
    Mist = false;
    RainTimePassed = 0.0f;
    RainTimePassedHistory[1] = 0.0f;
    RainTimePassedHistory[0] = 0.0f;
    SnowTimePassed = 0.0f;
    SnowTimePassedHistory[1] = 0.0f;
    SnowTimePassedHistory[0] = 0.0f;
    MistTimePassed = 0.0f;
    MistTimePassedHistory[1] = 0.0f;
    MistTimePassedHistory[0] = 0.0f;
    WindVector = C3DVector(0.0f, 0.0f, 0.0f);
    WindVectorHistory[1] = C3DVector(0.0f, 0.0f, 0.0f);
    WindVectorHistory[0] = C3DVector(0.0f, 0.0f, 0.0f);

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    {
        for (long c = 0; c < 5; ++c) {
            RainWindFactors[c] = GFFloatRandom(0.5f, &RandomNumberGeneratorSeed) + 0.5f;
            SnowWindFactors[c] = GFFloatRandom(0.9f, &RandomNumberGeneratorSeed) + 0.1f;
        }
    }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    {
        for (long c = 0; c < 5; ++c) {
            RainSpeedFactors[c] = GFFloatRandom(1.0f, &RandomNumberGeneratorSeed) + 1.0f;
            SnowSpeedFactors[c] = GFFloatRandom(0.5f, &RandomNumberGeneratorSeed) + 0.5f;
        }
    }
}

#include <stdio.h>
#include <string.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

long FABLE_FASTCALL GFROR13(unsigned long value)
{
    return static_cast<long>((value >> 13) | (value << 19));
}

static bool zero_vector(const C3DVector& vec)
{
    return vec.X == 0 && vec.Y == 0 && vec.Z == 0;
}

static bool close_to(float actual, float expected)
{
    float difference = actual - expected;
    return difference < 1e-6f && difference > -1e-6f;
}
static const float rain_wind[5] = {0.500732422f, 0.944976807f, 0.787612915f, 0.568344116f, 0.590805054f};
static const float snow_wind[5] = {0.315082997f, 0.449755520f, 0.830637097f, 0.287313908f, 0.933166146f};
static const float rain_speed[5] = {1.343734741f, 1.208633423f, 1.081512451f, 1.649536133f, 1.848236084f};
static const float snow_speed[5] = {0.896881104f, 0.615036011f, 0.743942261f, 0.999481201f, 0.952880859f};
static const unsigned long final_seed = 0x6e2ff3f0UL;
int main()
{
    CEngineWeatherSettings s;
    if (!s.Enable || s.ForceWeatherEffects || s.ParticleBoxLocked || s.ParticleBoxZLocked ||
        s.Raining || s.Snowing || s.Mist || s.RandomNumberGeneratorSeed != final_seed)
        return 1;
    if (s.RainSpeedMultiplier != 1 || s.RainWindSpeedMultiplier != 1 || s.RainWeighting != 1 ||
        s.RainDropSize.X != .5f || s.RainDropSize.Y != .5f || s.RainDropMinSize.Y != .2f ||
        s.RainStretchFactor != 0 || s.RainFadeStart != 10 || s.RainFadeEnd != 15 ||
        s.RainMaxAlpha != .2f || s.RainVolumeDepthSelectMultiplier != .6f)
        return 2;
    if (s.SnowWeighting != -1 || s.SnowSpeedMultiplier != .2f || s.SnowWindSpeedMultiplier != .2f ||
        s.SnowflakeBaseSize != .03f || s.SnowflakeScaleOffset != 1 || s.SnowflakeScaleDistanceFactor != 1 ||
        s.SnowflakeScaleDistanceSqrFactor != 0 || s.SnowflakeScaleMax != 64 || s.SnowflakeScaleMin != 0 ||
        s.SnowFadeStart != 0 || s.SnowFadeEnd != 35 || s.SnowMaxAlpha != .5f)
        return 3;
    if (s.ParticleBoxSpan.X != 8 || s.ParticleBoxSpan.Y != 8 || s.ParticleBoxSpan.Z != 8 ||
        s.ParticleTimeIncrement != .1f || s.ParticleBoxCenter.X || s.ParticleBoxCenter.Y || s.ParticleBoxCenter.Z)
        return 4;
    for (int i = 0; i < 81; ++i)
        if (!zero_vector(s.ParticleBoxPos[i])) return 5;
    if (s.WindSpeedFactor != .05f || s.MinWindSpeed != 0 || s.MaxWindSpeed != .005f ||
        s.MistMaxAlpha != .4f || s.MistSpeedMultiplier != 1 || s.MistWindSpeedMultiplier != .01f ||
        s.WeatherWindMinSpeed || s.WeatherWindMaxSpeed || s.WeatherWindSpeedMultiplier != .15f)
        return 6;
    const C3DVector* span[5] = {&s.MistTextureSpan,&s.MistVertexSpan,&s.MistVertexSpan2,&s.MistVertexSpan3,&s.MistVertexSpan4};
    const float xy[5] = {.1f,5,7,8,9}, z[5] = {.1f,0,.1f,.2f,.3f};
    for (int v = 0; v < 5; ++v)
        if (span[v]->X != xy[v] || span[v]->Y != xy[v] || span[v]->Z != z[v]) return 7;
    if (s.CurrentRainStrength || s.CurrentSnowStrength || s.RainTimePassed || s.SnowTimePassed || s.MistTimePassed || !zero_vector(s.WindVector)) return 8;
    for (int h = 0; h < 2; ++h)
        if (s.RainTimePassedHistory[h] || s.SnowTimePassedHistory[h] || s.MistTimePassedHistory[h] || !zero_vector(s.WindVectorHistory[h])) return 9;
    for (int k = 0; k < 5; ++k)
        if (!close_to(s.RainWindFactors[k],rain_wind[k]) || !close_to(s.SnowWindFactors[k],snow_wind[k]) ||
            !close_to(s.RainSpeedFactors[k],rain_speed[k]) || !close_to(s.SnowSpeedFactors[k],snow_speed[k])) return 10;
    puts("00b50fb0_TEST PASS");
    return 0;
}