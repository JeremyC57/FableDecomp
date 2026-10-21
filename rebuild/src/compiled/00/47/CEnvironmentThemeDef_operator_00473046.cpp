// CEnvironmentThemeDef::operator=(const CEnvironmentThemeDef&) -- memberwise assignment: base CParentDefClassBase::operator=, then the 34 theme fields (time of day, moon flag, fog, sky/cloud textures, water, weather and glow factors); returns *this
// Leaf: ??4CEnvironmentThemeDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00473046, length 327 bytes.
// Pragma: #pragma optimize("s", on) (unaligned entry, the def module is size-optimised).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0.
// Identity: vtable 0x01236244 = CEnvironmentThemeDef (rtti-vtables.tsv, slot 20 GetSizeofClass 0x004732DA returns 0xC4);
//   the copy constructor @ 0x0047318D is the byte twin of this body after its base call.
// Layout: Ego_r.pdb CEnvironmentThemeDef (200 bytes) with TimeOfDay at +0x40; the retail CParentDefClassBase is
//   0x3c bytes (PDB 0x40), so every member sits 4 lower (TimeOfDay +0x3c, MoonLit +0x40 .. WaterReflectionOffset
//   +0xc0, retail sizeof 0xC4). No generated engine header exists for the class, so it is modelled here.
// Shape: compiler-generated-style memberwise assignment; floats are copied as integer load/store pairs through eax,
//   the bool through al.
#pragma optimize("s", on)

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x3c - 4];
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
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
    CEnvironmentThemeDef& operator=(const CEnvironmentThemeDef& other);
};

CEnvironmentThemeDef& CEnvironmentThemeDef::operator=(const CEnvironmentThemeDef& other)
{
    CParentDefClassBase::operator=(other);
    TimeOfDay = other.TimeOfDay;
    MoonLit = other.MoonLit;
    FogStartZ = other.FogStartZ;
    FogEndZ = other.FogEndZ;
    SkyTexture0 = other.SkyTexture0;
    SkyTexture1 = other.SkyTexture1;
    SkyTexture1Blend = other.SkyTexture1Blend;
    CloudLowerLayerTexture0 = other.CloudLowerLayerTexture0;
    CloudLowerLayerTexture1 = other.CloudLowerLayerTexture1;
    CloudLowerLayerTexture1Blend = other.CloudLowerLayerTexture1Blend;
    CloudLowerLayerSpeedMultiplier = other.CloudLowerLayerSpeedMultiplier;
    CloudUpperLayerTexture0 = other.CloudUpperLayerTexture0;
    CloudUpperLayerTexture1 = other.CloudUpperLayerTexture1;
    CloudUpperLayerTexture1Blend = other.CloudUpperLayerTexture1Blend;
    CloudUpperLayerSpeedMultiplier = other.CloudUpperLayerSpeedMultiplier;
    WaterColourToReflectionBlend = other.WaterColourToReflectionBlend;
    WaterAlphaFactor = other.WaterAlphaFactor;
    WaterSpecularHightlightFactor = other.WaterSpecularHightlightFactor;
    RainStrength = other.RainStrength;
    SnowStrength = other.SnowStrength;
    MistMaxAlpha = other.MistMaxAlpha;
    LightningFrequency = other.LightningFrequency;
    GlowThresholdScale = other.GlowThresholdScale;
    GlowBloomScale = other.GlowBloomScale;
    GlowMotionBlur = other.GlowMotionBlur;
    ShadowFactor = other.ShadowFactor;
    FadedShadowFactor = other.FadedShadowFactor;
    WaterRefractionBlendStart = other.WaterRefractionBlendStart;
    WaterRefractionBlendEnd = other.WaterRefractionBlendEnd;
    WaterFlowSpeedFactor = other.WaterFlowSpeedFactor;
    WaterOscilationSpeed = other.WaterOscilationSpeed;
    WaterSwellFactor = other.WaterSwellFactor;
    WaterShoreSwellFactor = other.WaterShoreSwellFactor;
    WaterReflectionOffset = other.WaterReflectionOffset;
    return *this;
}