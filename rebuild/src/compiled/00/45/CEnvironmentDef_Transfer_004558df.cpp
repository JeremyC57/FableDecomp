// CEnvironmentDef::Transfer(CPersistContext& context) -- persist the environment definition: the colour lookup texture name, the lookup rows, sea/water/weather texture ids and the lightning/cloud/lake floats, each through the context's typed transfer helper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004558DF, length 830 bytes (virtual, `?Transfer@CEnvironmentDef@@UAEXAAVCPersistContext@@@Z`).
// Callees (typed CPersistContext transfers of the definition module; the catalog's "CEngineLightingManager::UpdateShadowScene"
// labels on 0x00431102/0x00431061 are propagated): CCharString 0x00431143, long 0x00431102, float 0x00431061.
// Layout: Ego_r.pdb CEnvironmentDef (0x138) less 4 in retail (the definition base is 4 bytes smaller, as for the landed
// CSoundThemeDef 0x0046329E): ColourLookupTexture +0x3c ... WaterLakeMaximumFlowSpeed +0x130.
// TU: the unaligned 0x43xxxx-0x46xxxx definition band is size-optimised (`optimize("s", on)`).
// Shape: the members are transferred in source order of the def file, not layout order (the Sea*/Day*/BumpMap groups move).
// Leaf: Transfer
#pragma optimize("s", on)
struct CCharString { void* Data; };

struct CPersistContext {
    void Transfer(CCharString& value);               // 0x00431143
    void TransferBinary(long* value);                // 0x00431102
    void TransferBinary(float* value);               // 0x00431061
};

struct CEnvironmentDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x38];                      // definition base (retail 4 bytes below the PDB)
    CCharString ColourLookupTexture;                 // +0x3c
    long  DiffuseLookupRow;                          // +0x40
    long  AmbientLookupRow;                          // +0x44
    long  BacklightLookupRow;                        // +0x48
    long  ReflectionLookupRow;                       // +0x4c
    long  MistEffectColourLookupRow;                 // +0x50
    long  FogColourLookupRow;                        // +0x54
    long  FogAlphaLookupRow;                         // +0x58
    long  SunColourLookupRow;                        // +0x5c
    long  CloudColourLookupRow;                      // +0x60
    long  MoonColourLookupRow;                       // +0x64
    long  StarsColourLookupRow;                      // +0x68
    long  SunFlareColourLookupRow;                   // +0x6c
    long  LensFlareColourLookupRow;                  // +0x70
    long  SkyGradientTopLookupRow;                   // +0x74
    long  SkyGradientTopAlphaLookupRow;              // +0x78
    long  SkyGradientBottomLookupRow;                // +0x7c
    long  SkyGradientBottomAlphaLookupRow;           // +0x80
    long  WaterColourLookupRow;                      // +0x84
    long  SeaColourLookupRow;                        // +0x88
    long  GlowThresholdColourLookupRow;              // +0x8c
    long  GlowBloomColourLookupRow;                  // +0x90
    long  SunlightAttenuatorColourLookupRow;         // +0x94
    float DiffuseClampAngle;                         // +0x98
    float SunlightAttenuatorAngleFadeStart;          // +0x9c
    float SunlightAttenuatorAngleFadeEnd;            // +0xa0
    long  SeaTexture;                                // +0xa4
    long  SeaRadius;                                 // +0xa8
    long  SeaFlatSectionStart;                       // +0xac
    long  SeaFlatSectionEnd;                         // +0xb0
    float DayStartTime;                              // +0xb4
    float DaySpeed;                                  // +0xb8
    long  WaterBumpMapPC;                            // +0xbc
    long  SeaBumpMapPC;                              // +0xc0
    long  IceBumpMapPC;                              // +0xc4
    long  IceBumpMap;                                // +0xc8
    long  IceTexture;                                // +0xcc
    long  WaterEdgeAlphaMap;                         // +0xd0
    long  WaterSurfMap;                              // +0xd4
    long  WaterBumpMap;                              // +0xd8
    long  WaterBumpMap2;                             // +0xdc
    long  SeaBumpMap;                                // +0xe0
    long  SeaBumpMap2;                               // +0xe4
    long  WaterEnvMapOverlayTexture;                 // +0xe8
    long  MistAlphaGraphic;                          // +0xec
    long  MistAlphaGraphicPC;                        // +0xf0
    long  RainTexture;                               // +0xf4
    long  RainTexturePC;                             // +0xf8
    long  SnowTexture;                               // +0xfc
    long  RainSplashParticle;                        // +0x100
    float LightningFadeInDuration;                   // +0x104
    float LightningFadeOutDuration;                  // +0x108
    float LightningFlashDuration;                    // +0x10c
    float LightningRainThreshold;                    // +0x110
    long  LightningTheme;                            // +0x114
    float CloudSpeedMultiplier;                      // +0x118
    float CloudMaxSpeed;                             // +0x11c
    float CloudTextureCoordMultiplier;               // +0x120
    float CloudTextureCoordOffset;                   // +0x124
    float CloudHeightOffset;                         // +0x128
    float WaterLakeMinimumFlowSpeed;                 // +0x12c
    float WaterLakeMaximumFlowSpeed;                 // +0x130
};

void CEnvironmentDef::Transfer(CPersistContext& context)
{
    context.Transfer(ColourLookupTexture);
    context.TransferBinary(&DiffuseLookupRow);
    context.TransferBinary(&AmbientLookupRow);
    context.TransferBinary(&BacklightLookupRow);
    context.TransferBinary(&ReflectionLookupRow);
    context.TransferBinary(&MistEffectColourLookupRow);
    context.TransferBinary(&FogColourLookupRow);
    context.TransferBinary(&FogAlphaLookupRow);
    context.TransferBinary(&SunColourLookupRow);
    context.TransferBinary(&CloudColourLookupRow);
    context.TransferBinary(&MoonColourLookupRow);
    context.TransferBinary(&StarsColourLookupRow);
    context.TransferBinary(&SunFlareColourLookupRow);
    context.TransferBinary(&LensFlareColourLookupRow);
    context.TransferBinary(&SkyGradientTopLookupRow);
    context.TransferBinary(&SkyGradientTopAlphaLookupRow);
    context.TransferBinary(&SkyGradientBottomLookupRow);
    context.TransferBinary(&SkyGradientBottomAlphaLookupRow);
    context.TransferBinary(&SunlightAttenuatorColourLookupRow);
    context.TransferBinary(&DiffuseClampAngle);
    context.TransferBinary(&SunlightAttenuatorAngleFadeStart);
    context.TransferBinary(&SunlightAttenuatorAngleFadeEnd);
    context.TransferBinary(&WaterColourLookupRow);
    context.TransferBinary(&SeaColourLookupRow);
    context.TransferBinary(&GlowThresholdColourLookupRow);
    context.TransferBinary(&GlowBloomColourLookupRow);
    context.TransferBinary(&SeaTexture);
    context.TransferBinary(&SeaRadius);
    context.TransferBinary(&SeaFlatSectionStart);
    context.TransferBinary(&SeaFlatSectionEnd);
    context.TransferBinary(&DayStartTime);
    context.TransferBinary(&DaySpeed);
    context.TransferBinary(&MistAlphaGraphic);
    context.TransferBinary(&MistAlphaGraphicPC);
    context.TransferBinary(&IceBumpMap);
    context.TransferBinary(&IceBumpMapPC);
    context.TransferBinary(&IceTexture);
    context.TransferBinary(&WaterEdgeAlphaMap);
    context.TransferBinary(&WaterSurfMap);
    context.TransferBinary(&WaterBumpMapPC);
    context.TransferBinary(&WaterBumpMap);
    context.TransferBinary(&WaterBumpMap2);
    context.TransferBinary(&SeaBumpMapPC);
    context.TransferBinary(&SeaBumpMap);
    context.TransferBinary(&SeaBumpMap2);
    context.TransferBinary(&WaterEnvMapOverlayTexture);
    context.TransferBinary(&RainTexture);
    context.TransferBinary(&RainTexturePC);
    context.TransferBinary(&SnowTexture);
    context.TransferBinary(&RainSplashParticle);
    context.TransferBinary(&LightningFadeInDuration);
    context.TransferBinary(&LightningFadeOutDuration);
    context.TransferBinary(&LightningFlashDuration);
    context.TransferBinary(&LightningRainThreshold);
    context.TransferBinary(&LightningTheme);
    context.TransferBinary(&CloudSpeedMultiplier);
    context.TransferBinary(&CloudMaxSpeed);
    context.TransferBinary(&CloudTextureCoordMultiplier);
    context.TransferBinary(&CloudTextureCoordOffset);
    context.TransferBinary(&CloudHeightOffset);
    context.TransferBinary(&WaterLakeMinimumFlowSpeed);
    context.TransferBinary(&WaterLakeMaximumFlowSpeed);
}