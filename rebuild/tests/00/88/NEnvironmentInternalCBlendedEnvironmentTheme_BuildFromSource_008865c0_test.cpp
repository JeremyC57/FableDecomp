// 008865c0 behaviour test: CBlendedEnvironmentTheme::BuildFromSource (match.cpp body verbatim, host stand-ins)
#include <stdio.h>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CRGBFloatColour { float R, G, B, A; };
template <class T> struct CArray {
    std::vector<T> V;
    void push_back(const T& x) { V.push_back(x); }
};
struct CIDrawEnvironmentTextureSet { CArray<int> Textures; CArray<float> Blends; };
struct CEnvironmentLookupTexture {
    CRGBFloatColour GetColour(long column, long row) const
    { CRGBFloatColour c; c.R = (float)(column * 100 + row); c.G = c.R + 0.25f; c.B = c.R + 0.5f; c.A = c.R + 0.75f; return c; }
};
struct CEnvironmentDef {
    char _pad00[0x40];
    long DiffuseLookupRow;                     // +0x40
    long AmbientLookupRow;
    long BacklightLookupRow;
    long ReflectionLookupRow;
    long MistEffectColourLookupRow;            // +0x50
    long FogColourLookupRow;
    long FogAlphaLookupRow;
    long SunColourLookupRow;
    long CloudColourLookupRow;                 // +0x60
    long MoonColourLookupRow;
    long StarsColourLookupRow;
    long SunFlareColourLookupRow;
    long LensFlareColourLookupRow;             // +0x70
    long SkyGradientTopLookupRow;
    long SkyGradientTopAlphaLookupRow;
    long SkyGradientBottomLookupRow;
    long SkyGradientBottomAlphaLookupRow;      // +0x80
    long WaterColourLookupRow;
    long SeaColourLookupRow;
    long GlowThresholdColourLookupRow;
    long GlowBloomColourLookupRow;             // +0x90
    long SunlightAttenuatorColourLookupRow;
};
struct CEnvironmentThemeDaySetDef {
    char _pad00[0x48];
    float SunTilt;                             // +0x48
    float SunRotate;
    float SunHeight;
    float MoonTilt;
    float MoonRotate;
    float MoonHeight;
};
struct CEnvironmentThemeDef {
    char _pad00[0x3c];
    float TimeOfDay;                           // +0x3c
    bool  MoonLit;                             // +0x40
    float FogStartZ;                           // +0x44
    float FogEndZ;
    int   SkyTexture0;                         // +0x4c
    int   SkyTexture1;
    float SkyTexture1Blend;
    int   CloudLowerLayerTexture0;             // +0x58
    int   CloudLowerLayerTexture1;
    float CloudLowerLayerTexture1Blend;
    float CloudLowerLayerSpeedMultiplier;      // +0x64
    int   CloudUpperLayerTexture0;
    int   CloudUpperLayerTexture1;
    float CloudUpperLayerTexture1Blend;        // +0x70
    float CloudUpperLayerSpeedMultiplier;
    float WaterColourToReflectionBlend;
    float WaterAlphaFactor;
    float WaterSpecularHightlightFactor;       // +0x80
    float RainStrength;
    float SnowStrength;
    float MistMaxAlpha;
    float LightningFrequency;                  // +0x90
    float GlowThresholdScale;
    float GlowBloomScale;
    float GlowMotionBlur;
    float ShadowFactor;                        // +0xa0
    float FadedShadowFactor;
    float WaterRefractionBlendStart;
    float WaterRefractionBlendEnd;
    float WaterFlowSpeedFactor;                // +0xb0
    float WaterOscilationSpeed;
    float WaterSwellFactor;
    float WaterShoreSwellFactor;
    float WaterReflectionOffset;               // +0xc0
};
namespace NEnvironmentInternal {
class CBlendedEnvironmentTheme {
public:
    float TimeOfDay; CRGBFloatColour Colours[19]; float Scalers[30]; CIDrawEnvironmentTextureSet TextureSets[3];
    void BuildFromSource(const CEnvironmentDef& environment, const CEnvironmentLookupTexture& lookup,
                         const CEnvironmentThemeDaySetDef& daySet, const CEnvironmentThemeDef& theme, long column);
};
}
void NEnvironmentInternal::CBlendedEnvironmentTheme::BuildFromSource(const CEnvironmentDef& environment,
                                                                      const CEnvironmentLookupTexture& lookup,
                                                                      const CEnvironmentThemeDaySetDef& daySet,
                                                                      const CEnvironmentThemeDef& theme, long column)
{
    TimeOfDay = theme.TimeOfDay;
    Colours[0] = lookup.GetColour(column, environment.DiffuseLookupRow);
    Colours[1] = lookup.GetColour(column, environment.AmbientLookupRow);
    Colours[2] = lookup.GetColour(column, environment.BacklightLookupRow);
    Colours[3] = lookup.GetColour(column, environment.ReflectionLookupRow);
    Colours[4] = lookup.GetColour(column, environment.MistEffectColourLookupRow);
    Colours[5] = lookup.GetColour(column, environment.FogColourLookupRow);
    Colours[6] = lookup.GetColour(column, environment.SunColourLookupRow);
    Colours[7] = lookup.GetColour(column, environment.CloudColourLookupRow);
    Colours[10] = lookup.GetColour(column, environment.SunFlareColourLookupRow);
    Colours[11] = lookup.GetColour(column, environment.LensFlareColourLookupRow);
    Colours[8] = lookup.GetColour(column, environment.MoonColourLookupRow);
    Colours[9] = lookup.GetColour(column, environment.StarsColourLookupRow);
    Colours[12] = lookup.GetColour(column, environment.SkyGradientTopLookupRow);
    Colours[13] = lookup.GetColour(column, environment.SkyGradientBottomLookupRow);
    Colours[14] = lookup.GetColour(column, environment.WaterColourLookupRow);
    Colours[15] = lookup.GetColour(column, environment.SeaColourLookupRow);
    Colours[5].A = lookup.GetColour(column, environment.FogAlphaLookupRow).R;
    Colours[12].A = lookup.GetColour(column, environment.SkyGradientTopAlphaLookupRow).R;
    Colours[13].A = lookup.GetColour(column, environment.SkyGradientBottomAlphaLookupRow).R;
    Colours[16] = lookup.GetColour(column, environment.GlowThresholdColourLookupRow);
    Colours[17] = lookup.GetColour(column, environment.GlowBloomColourLookupRow);
    Colours[18] = lookup.GetColour(column, environment.SunlightAttenuatorColourLookupRow);
    Scalers[2] = theme.WaterColourToReflectionBlend;
    Scalers[3] = theme.WaterAlphaFactor;
    Scalers[4] = theme.WaterSpecularHightlightFactor;
    Scalers[0] = theme.CloudLowerLayerSpeedMultiplier;
    Scalers[1] = theme.CloudUpperLayerSpeedMultiplier;
    Scalers[5] = (float)(theme.MoonLit ? 1.0 : 0.0);
    Scalers[6] = theme.FogStartZ;
    Scalers[7] = theme.FogEndZ;
    Scalers[15] = theme.RainStrength;
    Scalers[17] = theme.LightningFrequency;
    Scalers[14] = theme.SnowStrength;
    Scalers[16] = theme.MistMaxAlpha;
    Scalers[18] = theme.GlowThresholdScale;
    Scalers[19] = theme.GlowBloomScale;
    Scalers[20] = theme.GlowMotionBlur;
    Scalers[8] = daySet.SunTilt;
    Scalers[9] = daySet.SunRotate;
    Scalers[10] = daySet.SunHeight;
    Scalers[11] = daySet.MoonTilt;
    Scalers[12] = daySet.MoonRotate;
    Scalers[13] = daySet.MoonHeight;
    Scalers[21] = theme.ShadowFactor;
    Scalers[22] = theme.FadedShadowFactor;
    Scalers[23] = theme.WaterRefractionBlendStart;
    Scalers[24] = theme.WaterRefractionBlendEnd;
    Scalers[25] = theme.WaterFlowSpeedFactor;
    Scalers[26] = theme.WaterOscilationSpeed;
    Scalers[27] = theme.WaterSwellFactor;
    Scalers[28] = theme.WaterShoreSwellFactor;
    Scalers[29] = theme.WaterReflectionOffset;
    TextureSets[0].Textures.push_back(theme.SkyTexture0);
    TextureSets[0].Textures.push_back(theme.SkyTexture1);
    TextureSets[0].Blends.push_back(1.0f - theme.SkyTexture1Blend);
    TextureSets[0].Blends.push_back(theme.SkyTexture1Blend);
    TextureSets[2].Textures.push_back(theme.CloudLowerLayerTexture0);
    TextureSets[2].Textures.push_back(theme.CloudLowerLayerTexture1);
    TextureSets[2].Blends.push_back(1.0f - theme.CloudLowerLayerTexture1Blend);
    TextureSets[2].Blends.push_back(theme.CloudLowerLayerTexture1Blend);
    TextureSets[1].Textures.push_back(theme.CloudUpperLayerTexture0);
    TextureSets[1].Textures.push_back(theme.CloudUpperLayerTexture0);
    TextureSets[1].Blends.push_back(1.0f - theme.CloudUpperLayerTexture1Blend);
    TextureSets[1].Blends.push_back(theme.CloudUpperLayerTexture1Blend);
}

int main()
{
    CEnvironmentDef env; long* rows = &env.DiffuseLookupRow;
    for (int r = 0; r < 22; ++r) rows[r] = 10 + r;
    CEnvironmentThemeDaySetDef ds; float* dsf = &ds.SunTilt; for (int k = 0; k < 6; ++k) dsf[k] = 50.0f + k;
    CEnvironmentThemeDef th;
    th.TimeOfDay = 0.5f; th.MoonLit = true; th.FogStartZ = 1; th.FogEndZ = 2; th.SkyTexture0 = 7; th.SkyTexture1 = 8; th.SkyTexture1Blend = 0.25f;
    th.CloudLowerLayerTexture0 = 9; th.CloudLowerLayerTexture1 = 10; th.CloudLowerLayerTexture1Blend = 0.75f; th.CloudLowerLayerSpeedMultiplier = 3;
    th.CloudUpperLayerTexture0 = 11; th.CloudUpperLayerTexture1 = 12; th.CloudUpperLayerTexture1Blend = 0.5f; th.CloudUpperLayerSpeedMultiplier = 4;
    float* tf = &th.WaterColourToReflectionBlend; for (int k = 0; k < 19; ++k) tf[k] = 100.0f + k;
    CEnvironmentLookupTexture lk;
    NEnvironmentInternal::CBlendedEnvironmentTheme bt;
    bt.BuildFromSource(env, lk, ds, th, 3);
    CHECK(bt.TimeOfDay == 0.5f);
    CHECK(bt.Colours[0].R == 310.0f); CHECK(bt.Colours[1].G == 311.25f); CHECK(bt.Colours[6].R == 300.0f + env.SunColourLookupRow);
    CHECK(bt.Colours[8].R == 300.0f + env.MoonColourLookupRow); CHECK(bt.Colours[10].R == 300.0f + env.SunFlareColourLookupRow);
    CHECK(bt.Colours[5].R == 300.0f + env.FogColourLookupRow); CHECK(bt.Colours[5].A == 300.0f + env.FogAlphaLookupRow);
    CHECK(bt.Colours[12].A == 300.0f + env.SkyGradientTopAlphaLookupRow); CHECK(bt.Colours[13].A == 300.0f + env.SkyGradientBottomAlphaLookupRow);
    CHECK(bt.Colours[18].B == 300.5f + env.SunlightAttenuatorColourLookupRow);
    CHECK(bt.Scalers[0] == 3 && bt.Scalers[1] == 4 && bt.Scalers[2] == 100 && bt.Scalers[5] == 1.0f && bt.Scalers[6] == 1 && bt.Scalers[7] == 2);
    CHECK(bt.Scalers[8] == 50 && bt.Scalers[13] == 55 && bt.Scalers[15] == th.RainStrength && bt.Scalers[29] == th.WaterReflectionOffset);
    CHECK(bt.TextureSets[0].Textures.V.size() == 2 && bt.TextureSets[0].Textures.V[1] == 8);
    CHECK(bt.TextureSets[0].Blends.V[0] == 0.75f && bt.TextureSets[0].Blends.V[1] == 0.25f);
    CHECK(bt.TextureSets[2].Textures.V[0] == 9 && bt.TextureSets[2].Blends.V[0] == 0.25f);
    CHECK(bt.TextureSets[1].Textures.V[0] == 11 && bt.TextureSets[1].Textures.V[1] == 11);   // retail pushes Texture0 twice
    CHECK(bt.TextureSets[1].Blends.V[0] == 0.5f && bt.TextureSets[1].Blends.V[1] == 0.5f);
    th.MoonLit = false; NEnvironmentInternal::CBlendedEnvironmentTheme b2; b2.BuildFromSource(env, lk, ds, th, 0);
    CHECK(b2.Scalers[5] == 0.0f); CHECK(b2.Colours[0].R == 10.0f);
    if (g_fail) return 1;
    printf("008865c0_TEST PASS\n");
    return 0;
}