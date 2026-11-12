#ifndef FABLE_ENGINE_CENGINEWEATHERSETTINGS_H
#define FABLE_ENGINE_CENGINEWEATHERSETTINGS_H
// New header (chunk 2005, row 0x00B51570 CEngineWeatherRenderer::BuildBuffers). RTTI .?AVCEngineWeatherSettings@@ : CBaseClassNonCopyable
// : CBaseClass (ghidra_out/rtti_bases.tsv; vtable 0x012A2424 slot 0 = 0x00B50BE0). Ego_r.pdb members (ghidra_out/struct_layouts_egor.tsv,
// sizeof 0x558, no containers so retail = Ego_r): the leading scalars through RandomNumberGeneratorSeed (+0xD8; BuildBuffers steps it at
// CEngineWeatherRenderer +0x1EC = Settings +0xD8) are typed, the rest stays opaque.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
class CEngineWeatherSettings : public CBaseClassNonCopyable {
public:
    CEngineWeatherSettings();                            // ??0CEngineWeatherSettings@@QAE@XZ
    virtual ~CEngineWeatherSettings();                   // ??1CEngineWeatherSettings@@UAE@XZ (slot 0 ??_G 0x00B50BE0)
    void AddToConsole();                                 // ?AddToConsole@CEngineWeatherSettings@@QAEXXZ
    bool  Enable;                                        // +0x04
    bool  ForceWeatherEffects;                           // +0x05
    bool  ParticleBoxLocked;                             // +0x06
    bool  ParticleBoxZLocked;                            // +0x07
    float RainFadeStart;                                 // +0x08
    float RainFadeEnd;                                   // +0x0c
    float RainMaxAlpha;                                  // +0x10
    float RainWeighting;                                 // +0x14
    float RainSpeedMultiplier;                           // +0x18
    float RainWindSpeedMultiplier;                       // +0x1c
    float RainStretchFactor;                             // +0x20
    C2DVector RainDropSize;                              // +0x24
    C2DVector RainDropMinSize;                           // +0x2c
    float RainVolumeDepthSelectMultiplier;               // +0x34
    float SnowFadeStart;                                 // +0x38
    float SnowFadeEnd;                                   // +0x3c
    float SnowMaxAlpha;                                  // +0x40
    float SnowWeighting;                                 // +0x44
    float SnowSpeedMultiplier;                           // +0x48
    float SnowWindSpeedMultiplier;                       // +0x4c
    float SnowflakeBaseSize;                             // +0x50
    float SnowflakeScaleOffset;                          // +0x54
    float SnowflakeScaleDistanceFactor;                  // +0x58
    float SnowflakeScaleDistanceSqrFactor;               // +0x5c
    float SnowflakeScaleMax;                             // +0x60
    float SnowflakeScaleMin;                             // +0x64
    float WindSpeedFactor;                               // +0x68
    float MinWindSpeed;                                  // +0x6c
    float MaxWindSpeed;                                  // +0x70
    float MistMaxAlpha;                                  // +0x74
    C3DVector MistTextureSpan;                           // +0x78
    C3DVector MistVertexSpan;                            // +0x84
    C3DVector MistVertexSpan2;                           // +0x90
    C3DVector MistVertexSpan3;                           // +0x9c
    C3DVector MistVertexSpan4;                           // +0xa8
    float MistSpeedMultiplier;                           // +0xb4
    float MistWindSpeedMultiplier;                       // +0xb8
    float ParticleTimeIncrement;                         // +0xbc
    C3DVector ParticleBoxSpan;                           // +0xc0
    float WeatherWindMaxSpeed;                           // +0xcc
    float WeatherWindMinSpeed;                           // +0xd0
    float WeatherWindSpeedMultiplier;                    // +0xd4
    unsigned long RandomNumberGeneratorSeed;             // +0xd8
    unsigned char ParticleBoxCenter[0xc];                // +0xdc C3DCoordI
#ifdef FABLE_RETAIL_CENGINEWEATHERSETTINGS_TAIL
    // Nested opt-in (chunk 2008, row 0x00B54890 CEngineWeatherRenderer::RenderRain): the Ego_r.pdb tail typed (no containers: retail =
    // Ego_r). RenderRain reads ParticleBoxPos (+0xE8), CurrentRainStrength (+0x4B4), RainTimePassed (+0x4C0), WindVector (+0x4E4),
    // RainWindFactors (+0x508) and RainSpeedFactors (+0x51C).
    C3DVector ParticleBoxPos[81];                        // +0xe8
    float CurrentRainStrength;                           // +0x4b4
    float CurrentSnowStrength;                           // +0x4b8
    bool  Raining;                                       // +0x4bc
    bool  Snowing;                                       // +0x4bd
    bool  Mist;                                          // +0x4be
    float RainTimePassed;                                // +0x4c0
    float SnowTimePassed;                                // +0x4c4
    float MistTimePassed;                                // +0x4c8
    float RainTimePassedHistory[2];                      // +0x4cc
    float SnowTimePassedHistory[2];                      // +0x4d4
    float MistTimePassedHistory[2];                      // +0x4dc
    C3DVector WindVector;                                // +0x4e4
    C3DVector WindVectorHistory[2];                      // +0x4f0
    float RainWindFactors[5];                            // +0x508
    float RainSpeedFactors[5];                           // +0x51c
    float SnowWindFactors[5];                            // +0x530
    float SnowSpeedFactors[5];                           // +0x544
#else
    unsigned char _rest_0xe8[0x558 - 0xe8];              // +0xe8 ParticleBoxPos[81] .. SnowSpeedFactors[5] (not modelled)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineWeatherSettings) == 0x558);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherSettings, RandomNumberGeneratorSeed) == 0xd8);
#endif
