// CEnvironmentDef::operator=(const CEnvironmentDef&) -- memberwise copy of the def: CParentDefClassBase part, then every CEnvironmentDef member in declaration order; returns *this
// Leaf: ??4CEnvironmentDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045BE2B.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, CCharString::operator= @ 0x0099EFB0.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CEnvironmentDef's
// vtable 0x012335A4 is the 5-byte `jmp` at 0x0045BE26 right before this body (rtti-vtables.tsv).
// Layout: PDB CEnvironmentDef members from +0x40 sit at retail +0x3c (retail CParentDefClassBase 0x3c bytes; retail
// STLport vectors/maps are 12 bytes where the PDB has 16). Names and order are the PDB's.
// Shape: compiler-generated memberwise assignment written out; scalars and small structs are copied inline,
// strings/containers through their out-of-line operator=.
#pragma optimize("s", on)

#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x38];      // +0x04 CDefClassBase part, InstantiationName, SubDefInfoMap
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CCharString {                    // 4-byte string handle
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CEnvironmentDef : CParentDefClassBase {// vtable 0x012335A4
    CCharString ColourLookupTexture;                                 // +0x3c
    long        DiffuseLookupRow;                                    // +0x40
    long        AmbientLookupRow;                                    // +0x44
    long        BacklightLookupRow;                                  // +0x48
    long        ReflectionLookupRow;                                 // +0x4c
    long        MistEffectColourLookupRow;                           // +0x50
    long        FogColourLookupRow;                                  // +0x54
    long        FogAlphaLookupRow;                                   // +0x58
    long        SunColourLookupRow;                                  // +0x5c
    long        CloudColourLookupRow;                                // +0x60
    long        MoonColourLookupRow;                                 // +0x64
    long        StarsColourLookupRow;                                // +0x68
    long        SunFlareColourLookupRow;                             // +0x6c
    long        LensFlareColourLookupRow;                            // +0x70
    long        SkyGradientTopLookupRow;                             // +0x74
    long        SkyGradientTopAlphaLookupRow;                        // +0x78
    long        SkyGradientBottomLookupRow;                          // +0x7c
    long        SkyGradientBottomAlphaLookupRow;                     // +0x80
    long        WaterColourLookupRow;                                // +0x84
    long        SeaColourLookupRow;                                  // +0x88
    long        GlowThresholdColourLookupRow;                        // +0x8c
    long        GlowBloomColourLookupRow;                            // +0x90
    long        SunlightAttenuatorColourLookupRow;                   // +0x94
    float       DiffuseClampAngle;                                   // +0x98
    float       SunlightAttenuatorAngleFadeStart;                    // +0x9c
    float       SunlightAttenuatorAngleFadeEnd;                      // +0xa0
    long        SeaTexture;                                          // +0xa4
    long        SeaRadius;                                           // +0xa8
    long        SeaFlatSectionStart;                                 // +0xac
    long        SeaFlatSectionEnd;                                   // +0xb0
    float       DayStartTime;                                        // +0xb4
    float       DaySpeed;                                            // +0xb8
    long        WaterBumpMapPC;                                      // +0xbc
    long        SeaBumpMapPC;                                        // +0xc0
    long        IceBumpMapPC;                                        // +0xc4
    long        IceBumpMap;                                          // +0xc8
    long        IceTexture;                                          // +0xcc
    long        WaterEdgeAlphaMap;                                   // +0xd0
    long        WaterSurfMap;                                        // +0xd4
    long        WaterBumpMap;                                        // +0xd8
    long        WaterBumpMap2;                                       // +0xdc
    long        SeaBumpMap;                                          // +0xe0
    long        SeaBumpMap2;                                         // +0xe4
    long        WaterEnvMapOverlayTexture;                           // +0xe8
    long        MistAlphaGraphic;                                    // +0xec
    long        MistAlphaGraphicPC;                                  // +0xf0
    long        RainTexture;                                         // +0xf4
    long        RainTexturePC;                                       // +0xf8
    long        SnowTexture;                                         // +0xfc
    long        RainSplashParticle;                                  // +0x100
    float       LightningFadeInDuration;                             // +0x104
    float       LightningFadeOutDuration;                            // +0x108
    float       LightningFlashDuration;                              // +0x10c
    float       LightningRainThreshold;                              // +0x110
    long        LightningTheme;                                      // +0x114
    float       CloudSpeedMultiplier;                                // +0x118
    float       CloudMaxSpeed;                                       // +0x11c
    float       CloudTextureCoordMultiplier;                         // +0x120
    float       CloudTextureCoordOffset;                             // +0x124
    float       CloudHeightOffset;                                   // +0x128
    float       WaterLakeMinimumFlowSpeed;                           // +0x12c
    float       WaterLakeMaximumFlowSpeed;                           // +0x130
    CEnvironmentDef& operator=(const CEnvironmentDef& other);
};

CEnvironmentDef& CEnvironmentDef::operator=(const CEnvironmentDef& other)
{
    CParentDefClassBase::operator=(other);
    ColourLookupTexture = other.ColourLookupTexture;
    DiffuseLookupRow = other.DiffuseLookupRow;
    AmbientLookupRow = other.AmbientLookupRow;
    BacklightLookupRow = other.BacklightLookupRow;
    ReflectionLookupRow = other.ReflectionLookupRow;
    MistEffectColourLookupRow = other.MistEffectColourLookupRow;
    FogColourLookupRow = other.FogColourLookupRow;
    FogAlphaLookupRow = other.FogAlphaLookupRow;
    SunColourLookupRow = other.SunColourLookupRow;
    CloudColourLookupRow = other.CloudColourLookupRow;
    MoonColourLookupRow = other.MoonColourLookupRow;
    StarsColourLookupRow = other.StarsColourLookupRow;
    SunFlareColourLookupRow = other.SunFlareColourLookupRow;
    LensFlareColourLookupRow = other.LensFlareColourLookupRow;
    SkyGradientTopLookupRow = other.SkyGradientTopLookupRow;
    SkyGradientTopAlphaLookupRow = other.SkyGradientTopAlphaLookupRow;
    SkyGradientBottomLookupRow = other.SkyGradientBottomLookupRow;
    SkyGradientBottomAlphaLookupRow = other.SkyGradientBottomAlphaLookupRow;
    WaterColourLookupRow = other.WaterColourLookupRow;
    SeaColourLookupRow = other.SeaColourLookupRow;
    GlowThresholdColourLookupRow = other.GlowThresholdColourLookupRow;
    GlowBloomColourLookupRow = other.GlowBloomColourLookupRow;
    SunlightAttenuatorColourLookupRow = other.SunlightAttenuatorColourLookupRow;
    DiffuseClampAngle = other.DiffuseClampAngle;
    SunlightAttenuatorAngleFadeStart = other.SunlightAttenuatorAngleFadeStart;
    SunlightAttenuatorAngleFadeEnd = other.SunlightAttenuatorAngleFadeEnd;
    SeaTexture = other.SeaTexture;
    SeaRadius = other.SeaRadius;
    SeaFlatSectionStart = other.SeaFlatSectionStart;
    SeaFlatSectionEnd = other.SeaFlatSectionEnd;
    DayStartTime = other.DayStartTime;
    DaySpeed = other.DaySpeed;
    WaterBumpMapPC = other.WaterBumpMapPC;
    SeaBumpMapPC = other.SeaBumpMapPC;
    IceBumpMapPC = other.IceBumpMapPC;
    IceBumpMap = other.IceBumpMap;
    IceTexture = other.IceTexture;
    WaterEdgeAlphaMap = other.WaterEdgeAlphaMap;
    WaterSurfMap = other.WaterSurfMap;
    WaterBumpMap = other.WaterBumpMap;
    WaterBumpMap2 = other.WaterBumpMap2;
    SeaBumpMap = other.SeaBumpMap;
    SeaBumpMap2 = other.SeaBumpMap2;
    WaterEnvMapOverlayTexture = other.WaterEnvMapOverlayTexture;
    MistAlphaGraphic = other.MistAlphaGraphic;
    MistAlphaGraphicPC = other.MistAlphaGraphicPC;
    RainTexture = other.RainTexture;
    RainTexturePC = other.RainTexturePC;
    SnowTexture = other.SnowTexture;
    RainSplashParticle = other.RainSplashParticle;
    LightningFadeInDuration = other.LightningFadeInDuration;
    LightningFadeOutDuration = other.LightningFadeOutDuration;
    LightningFlashDuration = other.LightningFlashDuration;
    LightningRainThreshold = other.LightningRainThreshold;
    LightningTheme = other.LightningTheme;
    CloudSpeedMultiplier = other.CloudSpeedMultiplier;
    CloudMaxSpeed = other.CloudMaxSpeed;
    CloudTextureCoordMultiplier = other.CloudTextureCoordMultiplier;
    CloudTextureCoordOffset = other.CloudTextureCoordOffset;
    CloudHeightOffset = other.CloudHeightOffset;
    WaterLakeMinimumFlowSpeed = other.WaterLakeMinimumFlowSpeed;
    WaterLakeMaximumFlowSpeed = other.WaterLakeMaximumFlowSpeed;
    return *this;
}
