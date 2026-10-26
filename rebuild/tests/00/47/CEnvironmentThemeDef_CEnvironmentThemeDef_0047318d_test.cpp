#include <cstdio>
static int g_baseCalls = 0;
class CParentDefClassBase { public:
    virtual ~CParentDefClassBase() {}
    long m_baseTag;
    CParentDefClassBase() : m_baseTag(0) {}
    CParentDefClassBase(const CParentDefClassBase& o) : m_baseTag(o.m_baseTag) { ++g_baseCalls; }
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { m_baseTag = o.m_baseTag; ++g_baseCalls; return *this; }
};
class CEnvironmentThemeDef : public CParentDefClassBase { public:
    float  TimeOfDay;                       // +0x3c
    bool   MoonLit;                         // +0x40
    float  FogStartZ;                       // +0x44
    float  FogEndZ;                         // +0x48
    long   SkyTexture0;                     // +0x4c
    long   SkyTexture1;                     // +0x50
    float  SkyTexture1Blend;                // +0x54
    long   CloudLowerLayerTexture0;         // +0x58
    long   CloudLowerLayerTexture1;         // +0x5c
    float  CloudLowerLayerTexture1Blend;    // +0x60
    float  CloudLowerLayerSpeedMultiplier;  // +0x64
    long   CloudUpperLayerTexture0;         // +0x68
    long   CloudUpperLayerTexture1;         // +0x6c
    float  CloudUpperLayerTexture1Blend;    // +0x70
    float  CloudUpperLayerSpeedMultiplier;  // +0x74
    float  WaterColourToReflectionBlend;    // +0x78
    float  WaterAlphaFactor;                // +0x7c
    float  WaterSpecularHightlightFactor;   // +0x80
    float  RainStrength;                    // +0x84
    float  SnowStrength;                    // +0x88
    float  MistMaxAlpha;                    // +0x8c
    float  LightningFrequency;              // +0x90
    float  GlowThresholdScale;              // +0x94
    float  GlowBloomScale;                  // +0x98
    float  GlowMotionBlur;                  // +0x9c
    float  ShadowFactor;                    // +0xa0
    float  FadedShadowFactor;               // +0xa4
    float  WaterRefractionBlendStart;       // +0xa8
    float  WaterRefractionBlendEnd;         // +0xac
    float  WaterFlowSpeedFactor;            // +0xb0
    float  WaterOscilationSpeed;            // +0xb4
    float  WaterSwellFactor;                // +0xb8
    float  WaterShoreSwellFactor;           // +0xbc
    float  WaterReflectionOffset;           // +0xc0
    CEnvironmentThemeDef() {}
    CEnvironmentThemeDef(const CEnvironmentThemeDef& other);
};
CEnvironmentThemeDef::CEnvironmentThemeDef(const CEnvironmentThemeDef& other)
    : CParentDefClassBase(other)
    , TimeOfDay(other.TimeOfDay)
    , MoonLit(other.MoonLit)
    , FogStartZ(other.FogStartZ)
    , FogEndZ(other.FogEndZ)
    , SkyTexture0(other.SkyTexture0)
    , SkyTexture1(other.SkyTexture1)
    , SkyTexture1Blend(other.SkyTexture1Blend)
    , CloudLowerLayerTexture0(other.CloudLowerLayerTexture0)
    , CloudLowerLayerTexture1(other.CloudLowerLayerTexture1)
    , CloudLowerLayerTexture1Blend(other.CloudLowerLayerTexture1Blend)
    , CloudLowerLayerSpeedMultiplier(other.CloudLowerLayerSpeedMultiplier)
    , CloudUpperLayerTexture0(other.CloudUpperLayerTexture0)
    , CloudUpperLayerTexture1(other.CloudUpperLayerTexture1)
    , CloudUpperLayerTexture1Blend(other.CloudUpperLayerTexture1Blend)
    , CloudUpperLayerSpeedMultiplier(other.CloudUpperLayerSpeedMultiplier)
    , WaterColourToReflectionBlend(other.WaterColourToReflectionBlend)
    , WaterAlphaFactor(other.WaterAlphaFactor)
    , WaterSpecularHightlightFactor(other.WaterSpecularHightlightFactor)
    , RainStrength(other.RainStrength)
    , SnowStrength(other.SnowStrength)
    , MistMaxAlpha(other.MistMaxAlpha)
    , LightningFrequency(other.LightningFrequency)
    , GlowThresholdScale(other.GlowThresholdScale)
    , GlowBloomScale(other.GlowBloomScale)
    , GlowMotionBlur(other.GlowMotionBlur)
    , ShadowFactor(other.ShadowFactor)
    , FadedShadowFactor(other.FadedShadowFactor)
    , WaterRefractionBlendStart(other.WaterRefractionBlendStart)
    , WaterRefractionBlendEnd(other.WaterRefractionBlendEnd)
    , WaterFlowSpeedFactor(other.WaterFlowSpeedFactor)
    , WaterOscilationSpeed(other.WaterOscilationSpeed)
    , WaterSwellFactor(other.WaterSwellFactor)
    , WaterShoreSwellFactor(other.WaterShoreSwellFactor)
    , WaterReflectionOffset(other.WaterReflectionOffset)
{
}
int main()
{
    CEnvironmentThemeDef a;
    a.m_baseTag = 77;
    a.TimeOfDay = 0.25f; a.MoonLit = true; a.FogStartZ = 10.0f; a.FogEndZ = 90.0f;
    a.SkyTexture0 = 3; a.SkyTexture1 = 4; a.CloudUpperLayerTexture1 = 12;
    a.GlowBloomScale = 1.5f; a.WaterReflectionOffset = -2.5f; a.ShadowFactor = 0.75f;
    CEnvironmentThemeDef b(a);
    bool ok = true;
    ok = ok && g_baseCalls == 1 && b.m_baseTag == 77 && b.TimeOfDay == 0.25f && b.MoonLit
         && b.FogStartZ == 10.0f && b.FogEndZ == 90.0f && b.SkyTexture0 == 3 && b.SkyTexture1 == 4
         && b.CloudUpperLayerTexture1 == 12 && b.GlowBloomScale == 1.5f
         && b.WaterReflectionOffset == -2.5f && b.ShadowFactor == 0.75f;
    if (!ok) { std::printf("0047318d_TEST FAIL\n"); return 1; }
    std::printf("0047318d_TEST PASS\n");
    return 0;
}