// Behaviour test for CEnvironmentDef::Transfer (0x004558DF): every member reaches the right typed helper, in retail's order.
#include <cstdio>
#include <cstddef>
class CCharString { public: int Data; };
class CPersistContext { public:
    int n; char kinds[80]; const void* addrs[80];
    void Transfer(CCharString& value) { kinds[n] = 's'; addrs[n++] = &value; }
    void TransferBinary(int* value) { kinds[n] = 'l'; addrs[n++] = value; }
    void TransferBinary(float* value) { kinds[n] = 'f'; addrs[n++] = value; }
};
class CEnvironmentDef { public:
    void Transfer(CPersistContext& context);
    unsigned char _pad04[0x3c];                      // definition base (retail 4 bytes below the PDB)
    CCharString ColourLookupTexture;                 // +0x3c
    int  DiffuseLookupRow;                          // +0x40
    int  AmbientLookupRow;                          // +0x44
    int  BacklightLookupRow;                        // +0x48
    int  ReflectionLookupRow;                       // +0x4c
    int  MistEffectColourLookupRow;                 // +0x50
    int  FogColourLookupRow;                        // +0x54
    int  FogAlphaLookupRow;                         // +0x58
    int  SunColourLookupRow;                        // +0x5c
    int  CloudColourLookupRow;                      // +0x60
    int  MoonColourLookupRow;                       // +0x64
    int  StarsColourLookupRow;                      // +0x68
    int  SunFlareColourLookupRow;                   // +0x6c
    int  LensFlareColourLookupRow;                  // +0x70
    int  SkyGradientTopLookupRow;                   // +0x74
    int  SkyGradientTopAlphaLookupRow;              // +0x78
    int  SkyGradientBottomLookupRow;                // +0x7c
    int  SkyGradientBottomAlphaLookupRow;           // +0x80
    int  WaterColourLookupRow;                      // +0x84
    int  SeaColourLookupRow;                        // +0x88
    int  GlowThresholdColourLookupRow;              // +0x8c
    int  GlowBloomColourLookupRow;                  // +0x90
    int  SunlightAttenuatorColourLookupRow;         // +0x94
    float DiffuseClampAngle;                         // +0x98
    float SunlightAttenuatorAngleFadeStart;          // +0x9c
    float SunlightAttenuatorAngleFadeEnd;            // +0xa0
    int  SeaTexture;                                // +0xa4
    int  SeaRadius;                                 // +0xa8
    int  SeaFlatSectionStart;                       // +0xac
    int  SeaFlatSectionEnd;                         // +0xb0
    float DayStartTime;                              // +0xb4
    float DaySpeed;                                  // +0xb8
    int  WaterBumpMapPC;                            // +0xbc
    int  SeaBumpMapPC;                              // +0xc0
    int  IceBumpMapPC;                              // +0xc4
    int  IceBumpMap;                                // +0xc8
    int  IceTexture;                                // +0xcc
    int  WaterEdgeAlphaMap;                         // +0xd0
    int  WaterSurfMap;                              // +0xd4
    int  WaterBumpMap;                              // +0xd8
    int  WaterBumpMap2;                             // +0xdc
    int  SeaBumpMap;                                // +0xe0
    int  SeaBumpMap2;                               // +0xe4
    int  WaterEnvMapOverlayTexture;                 // +0xe8
    int  MistAlphaGraphic;                          // +0xec
    int  MistAlphaGraphicPC;                        // +0xf0
    int  RainTexture;                               // +0xf4
    int  RainTexturePC;                             // +0xf8
    int  SnowTexture;                               // +0xfc
    int  RainSplashParticle;                        // +0x100
    float LightningFadeInDuration;                   // +0x104
    float LightningFadeOutDuration;                  // +0x108
    float LightningFlashDuration;                    // +0x10c
    float LightningRainThreshold;                    // +0x110
    int  LightningTheme;                            // +0x114
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

static const char kExpectKinds[] = "sllllllllllllllllllfffllllllllffllllllllllllllllllfffflfffffff";
int main()
{
    CEnvironmentDef def;
    CPersistContext c; c.n = 0;
    def.Transfer(c);
    const unsigned char* base = (const unsigned char*)&def;
    // retail offsets with a 4-byte vptr; the test model keeps 0x3c bytes of padding so the offsets agree on any host.
    static const int kOffsets[] = { 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x50, 0x54, 0x58, 0x5c, 0x60, 0x64, 0x68, 0x6c, 0x70, 0x74, 0x78, 0x7c, 0x80, 0x94, 0x98, 0x9c, 0xa0, 0x84, 0x88, 0x8c, 0x90, 0xa4, 0xa8, 0xac, 0xb0, 0xb4, 0xb8, 0xec, 0xf0, 0xc8, 0xc4, 0xcc, 0xd0, 0xd4, 0xbc, 0xd8, 0xdc, 0xc0, 0xe0, 0xe4, 0xe8, 0xf4, 0xf8, 0xfc, 0x100, 0x104, 0x108, 0x10c, 0x110, 0x114, 0x118, 0x11c, 0x120, 0x124, 0x128, 0x12c, 0x130 };
    int ok = c.n == 62;
    for (int i = 0; ok && i < c.n; ++i) {
        if (c.kinds[i] != kExpectKinds[i]) ok = 0;
        if ((const unsigned char*)c.addrs[i] - base != kOffsets[i]) ok = 0;
    }
    if (!ok) { std::printf("004558df_TEST FAIL\n"); return 1; }
    std::printf("004558df_TEST PASS\n");
    return 0;
}