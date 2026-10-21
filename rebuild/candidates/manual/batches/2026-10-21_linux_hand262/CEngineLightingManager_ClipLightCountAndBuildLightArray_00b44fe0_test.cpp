// behaviour test for 00b44fe0 CEngineLightingManager::ClipLightCountAndBuildLightArray
#include <cstdio>
#include <cmath>
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour {
    float R, G, B, A;
    void operator*=(float f) { R *= f; G *= f; B *= f; A *= f; }
};
struct CEngineLocalLightElement {
    C3DVector WorldPos; float Radius, RadiusSquared, SpotRadius; CRGBFloatColour Colour;
    float AttenuationDistSquared, AttenuationConstant; unsigned long ChannelMask;
};
struct CEngineLocalLightArray { unsigned long LightCount; CEngineLocalLightElement Lights[8]; };
struct CEnginePrimitiveThemeChannelDesc { unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad; };
static float DistanceSquaredXY(const C3DVector& a, const C2DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
#define MAX(a, b) ((a) > (b) ? (a) : (b))
struct CManagerLocalLight {
    CEngineLocalLightElement LightElement; CManagerLocalLight* NextLocalLight; float ClippingScore; float ThemeWeight;
};
static CEngineLocalLightArray g_pool[2];
static int g_used = 0;
static CEngineLocalLightArray g_empty;
static CEngineLocalLightArray* Clip(CManagerLocalLight* lights, long lightCount, const C2DVector& position, long maxLights,
                                    const CEnginePrimitiveThemeChannelDesc& theme)
{
    if (theme.ChannelBlendFactor > 0 || lightCount + 1 > maxLights) {
        for (CManagerLocalLight* light = lights; light; light = light->NextLocalLight) {
            light->ThemeWeight = 0.0f;
            if (theme.ChannelBlendFactor < 255 && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex0)))
                light->ThemeWeight = 1.0f - theme.ChannelBlendFactor * (1.0f / 255.0f);
            if (theme.ChannelBlendFactor > 0 && (light->LightElement.ChannelMask & (1 << theme.ChannelIndex1)))
                light->ThemeWeight += theme.ChannelBlendFactor * (1.0f / 255.0f);
        }
    }
    if (lightCount + 1 > maxLights) {
        for (CManagerLocalLight* light = lights; light; light = light->NextLocalLight)
            light->ClippingScore = light->ThemeWeight / MAX(1.0f, DistanceSquaredXY(light->LightElement.WorldPos, position));
        CManagerLocalLight* sorted = 0;
        while (lights) {
            CManagerLocalLight* current = lights;
            lights = lights->NextLocalLight;
            CManagerLocalLight* prev = 0;
            CManagerLocalLight* it = sorted;
            while (it && it->ClippingScore > current->ClippingScore && maxLights > 0) {
                prev = it;
                it = it->NextLocalLight;
            }
            if (prev) prev->NextLocalLight = current; else sorted = current;
            current->NextLocalLight = it;
        }
        lights = sorted;
        lightCount = maxLights;
    }
    if (lightCount == 0)
        return &g_empty;
    CEngineLocalLightArray* array = &g_pool[g_used++];
    array->LightCount = lightCount;
    long i = 0;
    for (CManagerLocalLight* light = lights; light && i < maxLights; light = light->NextLocalLight, ++i) {
        array->Lights[i] = light->LightElement;
        if (theme.ChannelBlendFactor > 0)
            array->Lights[i].Colour *= light->ThemeWeight;
    }
    return array;
}
static void Init(CManagerLocalLight& l, float x, float y, unsigned long mask, CManagerLocalLight* next)
{
    l.LightElement.WorldPos.X = x; l.LightElement.WorldPos.Y = y; l.LightElement.WorldPos.Z = 0;
    l.LightElement.Colour.R = l.LightElement.Colour.G = l.LightElement.Colour.B = l.LightElement.Colour.A = 1.0f;
    l.LightElement.ChannelMask = mask; l.NextLocalLight = next;
}
int main()
{
    int bad = 0;
    CManagerLocalLight a, b, c;
    C2DVector pos = { 0, 0 };
    CEnginePrimitiveThemeChannelDesc theme = { 0, 1, 0, 0 };
    // three lights, room for two: nearest two survive, sorted by score (closest first)
    Init(a, 10, 0, 1, &b); Init(b, 2, 0, 1, &c); Init(c, 5, 0, 1, 0);
    CEngineLocalLightArray* r = Clip(&a, 3, pos, 2, theme);
    if (r->LightCount != 2 || r->Lights[0].WorldPos.X != 2 || r->Lights[1].WorldPos.X != 5) bad |= 1;
    // fits: plain copy in list order, no weighting
    Init(a, 10, 0, 1, &b); Init(b, 2, 0, 1, 0);
    r = Clip(&a, 2, pos, 4, theme);
    if (r->LightCount != 2 || r->Lights[0].WorldPos.X != 10 || r->Lights[1].Colour.R != 1.0f) bad |= 2;
    // nothing: the empty array
    if (Clip(0, 0, pos, 4, theme) != &g_empty) bad |= 4;
    if (bad) { std::printf("00b44fe0_TEST FAIL\n"); return 1; }
    // blend 51 (0.2): a light on channel 0 only gets weight 0.8, colour scaled
    g_used = 0;
    CEnginePrimitiveThemeChannelDesc blend = { 0, 1, 51, 0 };
    Init(a, 1, 0, 1, &b); Init(b, 1, 0, 2, 0);
    r = Clip(&a, 2, pos, 4, blend);
    if (std::fabs(r->Lights[0].Colour.R - 0.8f) > 1e-5f || std::fabs(r->Lights[1].Colour.G - 0.2f) > 1e-5f) bad |= 8;
    if (bad) { std::printf("00b44fe0_TEST FAIL\n"); return 1; }
    std::printf("00b44fe0_TEST PASS\n");
    return 0;
}
