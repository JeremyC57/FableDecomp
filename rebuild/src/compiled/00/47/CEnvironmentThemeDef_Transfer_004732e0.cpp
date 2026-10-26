// CEnvironmentThemeDef::Transfer(CPersistContext& context) -- persist every environment-theme member in declaration order: time of day, the moon-lit flag, fog range, the sky and cloud-layer textures/blends/speeds, the water, weather, glow and shadow factors
// Leaf: ?Transfer@CEnvironmentThemeDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004732E0, length 438 bytes, `ret 4` (vtable slot of CEnvironmentThemeDef).
// Callees: CPersistContext::Transfer(float&) @ 0x00431061, Transfer(bool&) @ 0x0043314A, Transfer(long&) @ 0x00431102
// (the overload of each call follows the member type; the parent def's Transfer is NOT called).
// Layout: Ego_r.pdb CEnvironmentThemeDef (200 bytes) less 4: the TLC def root ends at +0x3c (PDB 0x40), so TimeOfDay
// sits at +0x3c and WaterReflectionOffset at +0xc0 (0xc4 bytes).
// Shape: the context reference lives in edi, every member address is formed with `lea eax ; push eax ; mov ecx,edi`;
// the last member's address is formed in place (`add esi,0C0h`).
#pragma optimize("s", on)
class CPersistContext { public:
    void Transfer(float& value);                // 0x00431061
    void Transfer(bool& value);                 // 0x0043314A
    void Transfer(long& value);                 // 0x00431102
};

class CParentDefClassBase { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
};

class CEnvironmentThemeDef : public CParentDefClassBase { public:
    float TimeOfDay;                            // +0x3c
    bool  MoonLit;                              // +0x40
    float FogStartZ;                            // +0x44
    float FogEndZ;                              // +0x48
    long  SkyTexture0;                          // +0x4c
    long  SkyTexture1;                          // +0x50
    float SkyTexture1Blend;                     // +0x54
    long  CloudLowerLayerTexture0;              // +0x58
    long  CloudLowerLayerTexture1;              // +0x5c
    float CloudLowerLayerTexture1Blend;         // +0x60
    float CloudLowerLayerSpeedMultiplier;       // +0x64
    long  CloudUpperLayerTexture0;              // +0x68
    long  CloudUpperLayerTexture1;              // +0x6c
    float CloudUpperLayerTexture1Blend;         // +0x70
    float CloudUpperLayerSpeedMultiplier;       // +0x74
    float WaterColourToReflectionBlend;         // +0x78
    float WaterAlphaFactor;                     // +0x7c
    float WaterSpecularHightlightFactor;        // +0x80
    float RainStrength;                         // +0x84
    float SnowStrength;                         // +0x88
    float MistMaxAlpha;                         // +0x8c
    float LightningFrequency;                   // +0x90
    float GlowThresholdScale;                   // +0x94
    float GlowBloomScale;                       // +0x98
    float GlowMotionBlur;                       // +0x9c
    float ShadowFactor;                         // +0xa0
    float FadedShadowFactor;                    // +0xa4
    float WaterRefractionBlendStart;            // +0xa8
    float WaterRefractionBlendEnd;              // +0xac
    float WaterFlowSpeedFactor;                 // +0xb0
    float WaterOscilationSpeed;                 // +0xb4
    float WaterSwellFactor;                     // +0xb8
    float WaterShoreSwellFactor;                // +0xbc
    float WaterReflectionOffset;                // +0xc0
    virtual void Transfer(CPersistContext& context);
};

void CEnvironmentThemeDef::Transfer(CPersistContext& context)
{
    context.Transfer(TimeOfDay);
    context.Transfer(MoonLit);
    context.Transfer(FogStartZ);
    context.Transfer(FogEndZ);
    context.Transfer(SkyTexture0);
    context.Transfer(SkyTexture1);
    context.Transfer(SkyTexture1Blend);
    context.Transfer(CloudLowerLayerTexture0);
    context.Transfer(CloudLowerLayerTexture1);
    context.Transfer(CloudLowerLayerTexture1Blend);
    context.Transfer(CloudLowerLayerSpeedMultiplier);
    context.Transfer(CloudUpperLayerTexture0);
    context.Transfer(CloudUpperLayerTexture1);
    context.Transfer(CloudUpperLayerTexture1Blend);
    context.Transfer(CloudUpperLayerSpeedMultiplier);
    context.Transfer(WaterColourToReflectionBlend);
    context.Transfer(WaterAlphaFactor);
    context.Transfer(WaterSpecularHightlightFactor);
    context.Transfer(RainStrength);
    context.Transfer(SnowStrength);
    context.Transfer(MistMaxAlpha);
    context.Transfer(LightningFrequency);
    context.Transfer(GlowThresholdScale);
    context.Transfer(GlowBloomScale);
    context.Transfer(GlowMotionBlur);
    context.Transfer(ShadowFactor);
    context.Transfer(FadedShadowFactor);
    context.Transfer(WaterRefractionBlendStart);
    context.Transfer(WaterRefractionBlendEnd);
    context.Transfer(WaterFlowSpeedFactor);
    context.Transfer(WaterOscilationSpeed);
    context.Transfer(WaterSwellFactor);
    context.Transfer(WaterShoreSwellFactor);
    context.Transfer(WaterReflectionOffset);
}