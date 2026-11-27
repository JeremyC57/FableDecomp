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