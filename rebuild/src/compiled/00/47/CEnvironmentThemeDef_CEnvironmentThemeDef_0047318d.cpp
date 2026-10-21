// CEnvironmentThemeDef::CEnvironmentThemeDef(const CEnvironmentThemeDef&) -- copy constructor: the CParentDefClassBase copy, own vtable, then the 34 theme fields copied memberwise
// Leaf: ??0CEnvironmentThemeDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047318D, length 333 bytes.
// Pragma: #pragma optimize("s", on) (unaligned entry, the def module is size-optimised).
// Callees: the CParentDefClassBase copy constructor @ 0x0046EE98.
// Identity: stores vtable 0x01236244 = CEnvironmentThemeDef (rtti-vtables.tsv); called by the vector helper
//   _Uninit_fill_n<CEnvironmentThemeDef> @ 0x0047725F (landed) on 0xC4-byte elements.
// Layout: Ego_r.pdb CEnvironmentThemeDef minus 4 (retail CParentDefClassBase is 0x3c bytes): TimeOfDay +0x3c,
//   MoonLit +0x40 .. WaterReflectionOffset +0xc0 (no generated engine header for the class).
// Shape: base copy, vtable store, then every dword copied through eax in declaration order; returns `this`.
#pragma optimize("s", on)

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x3c - 4];
    CParentDefClassBase(const CParentDefClassBase& other);              // 0x0046EE98
};

struct CEnvironmentThemeDef : CParentDefClassBase {                       // vtable 0x01236244, retail sizeof 0xC4
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
    virtual void v00();
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