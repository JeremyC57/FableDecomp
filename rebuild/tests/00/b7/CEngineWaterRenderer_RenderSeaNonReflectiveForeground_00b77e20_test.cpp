// Behaviour test for CEngineWaterRenderer::RenderSeaNonReflectiveForeground @ 0x00B77E20 (restated with fakes):
// the swell, fresnel clamp, bump and sea-colour constants.
#include <cstdio>
#include <cstring>

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

struct Sea {
    bool swell, fresnel;
    float swellX, swellY, swellZ, fresnelFactor, bumpFactor, bumpScale, brightness, colour[3];
};
struct Out { float c0[4]; float ps5[4]; float c3[4]; float ps7[4]; int meshes; long tris; };

static void Render(const Sea& s, float envBrightness, int meshCount, long polys, Out& o)
{
    if (s.swell) { o.c0[0] = s.swellX; o.c0[1] = s.swellY; o.c0[2] = s.swellZ; o.c0[3] = 0.0f; }
    else { o.c0[0] = o.c0[1] = o.c0[2] = o.c0[3] = 0.0f; }
    float f = s.fresnel ? Clamp(s.fresnelFactor * 0.25f, 0.0f, 1.0f) : 0.0f;
    o.ps5[0] = o.ps5[1] = o.ps5[2] = o.ps5[3] = f;
    float bump = s.bumpFactor * s.bumpScale;
    o.c3[0] = bump; o.c3[1] = 0.0f; o.c3[2] = bump * -0.5f; o.c3[3] = 1.0f;
    o.ps7[0] = s.colour[0]; o.ps7[1] = s.colour[1]; o.ps7[2] = s.colour[2]; o.ps7[3] = envBrightness * s.brightness;
    for (int i = 0; i < meshCount; ++i) { ++o.meshes; o.tris = polys; }
}

int main()
{
    int fails = 0;
    Sea s; std::memset(&s, 0, sizeof s);
    Out o; std::memset(&o, 0, sizeof o);
    s.swell = true; s.swellX = 1.0f; s.swellY = 2.0f; s.swellZ = 3.0f;
    s.fresnel = true; s.fresnelFactor = 8.0f; s.bumpFactor = 2.0f; s.bumpScale = 3.0f; s.brightness = 0.5f;
    s.colour[0] = 0.1f; s.colour[1] = 0.2f; s.colour[2] = 0.3f;
    Render(s, 4.0f, 3, 77, o);
    if (o.c0[2] != 3.0f || o.ps5[0] != 1.0f || o.c3[0] != 6.0f || o.c3[2] != -3.0f || o.ps7[3] != 2.0f) ++fails;
    if (o.meshes != 3 || o.tris != 77) ++fails;
    s.fresnelFactor = -4.0f; s.swell = false;
    Render(s, 4.0f, 0, 0, o);
    if (o.ps5[3] != 0.0f || o.c0[0] != 0.0f) ++fails;
    s.fresnelFactor = 2.0f;
    Render(s, 4.0f, 0, 0, o);
    if (o.ps5[1] != 0.5f) ++fails;
    s.fresnel = false;
    Render(s, 4.0f, 0, 0, o);
    if (o.ps5[2] != 0.0f) ++fails;
    if (fails == 0) std::printf("00b77e20_TEST PASS\n"); else std::printf("00b77e20_TEST FAIL %d\n", fails);
    return fails;
}