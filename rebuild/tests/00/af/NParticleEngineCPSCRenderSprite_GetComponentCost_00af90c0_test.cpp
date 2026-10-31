// Behaviour test for CPSCRenderSprite::GetComponentCost (0x00AF90C0): the sprite part quantises the start/end render sizes
// (bits * 20/2047, rounded to two decimals), scales the larger one by the system and adds crossed-sprite polygons
// (2, or 2 * NoCrossedSprites when flag 0x40 is set) and size^2 per particle; the trail part adds 2 polygons per particle
// and width (TrailWidth * 10/1023, two decimals) x min(length * 0.001, 2) / 2, both scaled.
#include <cstdio>
#include <cmath>

struct Cost { float PolyCount, PixelCount, LightCount, RenderCost, SetupCost, LightCost; };

static float Round2(float v) { float s = 100.0f; float x = v * s; long r = (long)floor(x + 0.5f); return r / s; }

static void GetComponentCost(Cost& cost, float scale, long spriteBank, long trailBank, unsigned flags, unsigned noCrossed,
                             unsigned startSize, unsigned endSize, unsigned trailWidth, unsigned trailLength, float n)
{
    if (spriteBank) {
        float a = Round2((float)startSize * (1.0f / 2047.0f) * 20.0f);
        float b = Round2((float)endSize * (1.0f / 2047.0f) * 20.0f);
        float size = a > b ? a : b;
        size *= scale;
        float polys = (flags & 0x40) ? (float)(noCrossed * 2) : 2.0f;
        polys *= n;
        cost.PolyCount += polys;
        cost.RenderCost += size * size * n + polys;
    }
    if (trailBank) {
        float width = Round2((float)trailWidth * (1.0f / 1023.0f) * 10.0f);
        float polys = n + n;
        float length = (float)trailLength * 0.001f;
        if (length > 2.0f) length = 2.0f;
        length *= 0.5f;
        width *= scale; length *= scale;
        float area = length * width * n;
        cost.PolyCount += polys; cost.PixelCount += area; cost.RenderCost += area + polys;
    }
}

int main()
{
    int ok = 1;
    Cost c = { 0, 0, 0, 0, 0, 0 };
    GetComponentCost(c, 1.0f, 1, 0, 0, 3, 2047, 1023, 0, 0, 10.0f);   // start 20.00, end 9.99 -> 20
    ok &= c.PolyCount == 20.0f && fabs(c.RenderCost - (400.0f * 10.0f + 20.0f)) < 1e-2f;
    Cost d = { 0, 0, 0, 0, 0, 0 };
    GetComponentCost(d, 2.0f, 1, 1, 0x40, 3, 0, 2047, 1023, 4000, 1.0f);
    ok &= d.PolyCount == 6.0f + 2.0f;                                   // 3 crossed sprites * 2 + trail 2
    ok &= fabs(d.PixelCount - (1.0f * 2.0f) * (10.0f * 2.0f)) < 1e-3f;  // length clamped to 2 then halved, both scaled
    if (!ok) { std::printf("00af90c0_TEST FAIL\n"); return 1; }
    std::printf("00af90c0_TEST PASS\n");
    return 0;
}