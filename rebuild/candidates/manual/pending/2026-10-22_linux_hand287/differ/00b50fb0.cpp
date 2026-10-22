// Grade: DIFFER(1034v1018) -- same store order, temporaries and loops; retail allocates the zero constant to eax before the first C3DVector temporaries (so MistVertexSpan.Z forwards from eax and 0.2/0.5 use edx/edi), VC7.1 here zeroes eax only after MistVertexSpan2; loop-entry padding then differs
// CEngineWeatherSettings::CEngineWeatherSettings() -- default rain/snow/mist tuning, a zeroed particle box, and per-slot random wind/speed factors from a fixed seed
// Leaf: ??0CEngineWeatherSettings@@QAE@XZ
// Layout: the base at +0 is the 4-byte vptr-only CBase (ctor 0x0099A2F0 stores vtable 0x0129A7C4 only); members from Ego_r.pdb CEngineWeatherSettings (0x558).
extern "C" double __cdecl fmod(double, double);
#pragma intrinsic(fmod)
unsigned long __fastcall GFROR13(unsigned long value);              // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
inline float GFFloatRandomInline(float range, unsigned long* seed)
{
    double scaled = range * 65536.0;
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    if (scaled > 0.0001)
        return (float)(fmod((double)*seed, scaled) * (1.0 / 65536.0));
    return 0.0f;
}
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C3DCoordI {
    long X, Y, Z;
    C3DCoordI() { }
    C3DCoordI(long x, long y, long z) : X(x), Y(y), Z(z) { }
};
struct CBase {
    CBase();                                       // 0x0099A2F0
    virtual ~CBase();
};
struct CEngineWeatherSettings : CBase {
    bool      Enable;                              // +0x04
    bool      ForceWeatherEffects;
    bool      ParticleBoxLocked;
    bool      ParticleBoxZLocked;
    float     RainFadeStart;                       // +0x08
    float     RainFadeEnd;
    float     RainMaxAlpha;                        // +0x10
    float     RainWeighting;
    float     RainSpeedMultiplier;
    float     RainWindSpeedMultiplier;
    float     RainStretchFactor;                   // +0x20
    C2DVector RainDropSize;                        // +0x24
    C2DVector RainDropMinSize;                     // +0x2c
    float     RainVolumeDepthSelectMultiplier;     // +0x34
    float     SnowFadeStart;                       // +0x38
    float     SnowFadeEnd;
    float     SnowMaxAlpha;                        // +0x40
    float     SnowWeighting;
    float     SnowSpeedMultiplier;
    float     SnowWindSpeedMultiplier;
    float     SnowflakeBaseSize;                   // +0x50
    float     SnowflakeScaleOffset;
    float     SnowflakeScaleDistanceFactor;
    float     SnowflakeScaleDistanceSqrFactor;
    float     SnowflakeScaleMax;                   // +0x60
    float     SnowflakeScaleMin;
    float     WindSpeedFactor;
    float     MinWindSpeed;
    float     MaxWindSpeed;                        // +0x70
    float     MistMaxAlpha;
    C3DVector MistTextureSpan;                     // +0x78
    C3DVector MistVertexSpan;                      // +0x84
    C3DVector MistVertexSpan2;                     // +0x90
    C3DVector MistVertexSpan3;                     // +0x9c
    C3DVector MistVertexSpan4;                     // +0xa8
    float     MistSpeedMultiplier;                 // +0xb4
    float     MistWindSpeedMultiplier;
    float     ParticleTimeIncrement;               // +0xbc
    C3DVector ParticleBoxSpan;                     // +0xc0
    float     WeatherWindMaxSpeed;                 // +0xcc
    float     WeatherWindMinSpeed;
    float     WeatherWindSpeedMultiplier;          // +0xd4
    unsigned long RandomNumberGeneratorSeed;       // +0xd8
    C3DCoordI ParticleBoxCenter;                   // +0xdc
    C3DVector ParticleBoxPos[81];                  // +0xe8
    float     CurrentRainStrength;                 // +0x4b4
    float     CurrentSnowStrength;
    bool      Raining;                             // +0x4bc
    bool      Snowing;
    bool      Mist;
    float     RainTimePassed;                      // +0x4c0
    float     SnowTimePassed;
    float     MistTimePassed;
    float     RainTimePassedHistory[2];            // +0x4cc
    float     SnowTimePassedHistory[2];            // +0x4d4
    float     MistTimePassedHistory[2];            // +0x4dc
    C3DVector WindVector;                          // +0x4e4
    C3DVector WindVectorHistory[2];                // +0x4f0
    float     RainWindFactors[5];                  // +0x508
    float     RainSpeedFactors[5];                 // +0x51c
    float     SnowWindFactors[5];                  // +0x530
    float     SnowSpeedFactors[5];                 // +0x544
    CEngineWeatherSettings();
    virtual ~CEngineWeatherSettings();
};

CEngineWeatherSettings::CEngineWeatherSettings()
{
    RainDropMinSize.Y = 0.2f;
    RainMaxAlpha = 0.2f;
    SnowSpeedMultiplier = 0.2f;
    SnowWindSpeedMultiplier = 0.2f;
    RainDropSize.X = 0.5f;
    RainDropSize.Y = 0.5f;
    SnowMaxAlpha = 0.5f;
    ParticleBoxSpan = C3DVector(8.0f, 8.0f, 8.0f);
    ParticleTimeIncrement = 0.1f;
    MistTextureSpan.X = 0.1f;
    MistTextureSpan.Y = 0.1f;
    MistTextureSpan.Z = 0.1f;
    MistVertexSpan = C3DVector(5.0f, 5.0f, 0.0f);
    MistVertexSpan2 = C3DVector(7.0f, 7.0f, 0.1f);
    Enable = true;
    ForceWeatherEffects = false;
    ParticleBoxLocked = false;
    ParticleBoxZLocked = false;
    RainSpeedMultiplier = 1.0f;
    RainWindSpeedMultiplier = 1.0f;
    RainWeighting = 1.0f;
    RainStretchFactor = 0.0f;
    RainFadeStart = 10.0f;
    RainFadeEnd = 15.0f;
    RainVolumeDepthSelectMultiplier = 0.6f;
    SnowWeighting = -1.0f;
    SnowflakeBaseSize = 0.03f;
    SnowflakeScaleOffset = 1.0f;
    SnowflakeScaleDistanceFactor = 1.0f;
    SnowflakeScaleDistanceSqrFactor = 0.0f;
    SnowflakeScaleMax = 64.0f;
    SnowflakeScaleMin = 0.0f;
    SnowFadeStart = 0.0f;
    SnowFadeEnd = 35.0f;
    WindSpeedFactor = 0.05f;
    MinWindSpeed = 0.0f;
    MaxWindSpeed = 0.005f;
    MistMaxAlpha = 0.4f;
    MistVertexSpan3 = C3DVector(8.0f, 8.0f, 0.2f);
    MistVertexSpan4 = C3DVector(9.0f, 9.0f, 0.3f);
    MistSpeedMultiplier = 1.0f;
    ParticleBoxCenter = C3DCoordI(0, 0, 0);
    MistWindSpeedMultiplier = 0.01f;
    WeatherWindMinSpeed = 0.0f;
    WeatherWindMaxSpeed = 0.0f;
    WeatherWindSpeedMultiplier = 0.15f;
    RandomNumberGeneratorSeed = 0x29;
    for (long i = 0; i < 81; ++i)
        ParticleBoxPos[i] = C3DVector(0.0f, 0.0f, 0.0f);
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
    for (long j = 0; j < 5; ++j) {
        RainWindFactors[j] = GFFloatRandom(0.5f, &RandomNumberGeneratorSeed) + 0.5f;
        SnowWindFactors[j] = GFFloatRandom(0.9f, &RandomNumberGeneratorSeed) + 0.1f;
    }
    for (long k = 0; k < 5; ++k) {
        RainSpeedFactors[k] = GFFloatRandomInline(1.0f, &RandomNumberGeneratorSeed) + 1.0f;
        SnowSpeedFactors[k] = GFFloatRandomInline(0.5f, &RandomNumberGeneratorSeed) + 0.5f;
    }
}
