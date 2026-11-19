// Behaviour test for NParticleEngine::CPSCSingleSprite::GetComponentCost (0x00B05C60): a copy of the row's logic on stand-alone types.
// Sizes are unpacked through the shared GFUnpackFloat (packed / 2047 * 20, rounded to two decimals through GFFloatToLongNear: retail
// asm under VC7.1, the FABLE_PORTABLE twin elsewhere); the larger size is scaled by the system's largest Scale component when
// ScaleParticles is set; crossed sprites cost NoCrossedSprites * 2 polygons, otherwise 2; a trail costs (length - 1) * 16 polygons.
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#endif
#include <stdio.h>
#include <math.h>
#include "engine/GFPackFloat.h"

struct Cost { float PolyCount, PixelCount, LightCount, RenderCost, SetupCost, LightCost; };
struct System {
    bool ScaleParticles; float SX, SY, SZ;
    void ScaleParticleDimensions(float& size) const
    { if (ScaleParticles) size *= ((SX > SY ? SX : SY) > SZ ? (SX > SY ? SX : SY) : SZ); }
};
struct Sprite {
    bool UseSplinePoints; long SpriteBankIndex; float StartRenderSize, EndRenderSize; long NoCrossedSprites;
    long TrailBankIndex; float TrailWidth; long MaxTrailLength; bool FaceMe2D, CrossedSprites; int splineCalls;
    void GetComponentCost(Cost& component_cost, const System& system)
    {
        if (UseSplinePoints) ++splineCalls;
        if (SpriteBankIndex) {
            float start_render_size = GFUnpackFloat((unsigned long)StartRenderSize, 11, 0.0f, 20.0f, 2);
            float end_render_size = GFUnpackFloat((unsigned long)EndRenderSize, 11, 0.0f, 20.0f, 2);
            float render_size = start_render_size > end_render_size ? start_render_size : end_render_size;
            system.ScaleParticleDimensions(render_size);
            float poly_count = 0.0f;
            if (CrossedSprites) poly_count = (float)(NoCrossedSprites * 2);
            else if (FaceMe2D) poly_count = 2.0f;
            else poly_count = 2.0f;
            float pixel_count = render_size * render_size;
            component_cost.PolyCount += poly_count;
            component_cost.PixelCount += pixel_count;
            component_cost.RenderCost += poly_count + pixel_count;
        }
        if (TrailBankIndex) {
            float poly_count = (float)((MaxTrailLength - 1) * 16);
            float trail_width = TrailWidth;
            system.ScaleParticleDimensions(trail_width);
            float pixel_count = trail_width * poly_count;
            component_cost.PolyCount += poly_count;
            component_cost.PixelCount += pixel_count;
            component_cost.RenderCost += poly_count + pixel_count;
        }
    }
};

static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int fails = 0;
    System sys = { true, 1.0f, 3.0f, 2.0f };
    // packed 1023 -> 1023/2047*20 = 9.9951 -> 10.00 (two decimals); packed 205 -> 2.0029 -> 2.00; scale x3 -> 30
    Sprite s = { true, 1, 1023.0f, 205.0f, 3, 0, 0.0f, 0, false, true, 0 };
    Cost c = { 1.0f, 2.0f, 0.0f, 3.0f, 0.0f, 0.0f };
    s.GetComponentCost(c, sys);
    if (s.splineCalls != 1) ++fails;
    if (!Near(c.PolyCount, 1.0f + 6.0f) || !Near(c.PixelCount, 2.0f + 900.0f) || !Near(c.RenderCost, 3.0f + 906.0f)) ++fails;
    // not crossed: 2 polygons; no spline; trail 5 long, width 0.5 scaled x3 = 1.5 -> 64 polys, 96 pixels
    Sprite t = { false, 1, 205.0f, 1023.0f, 3, 1, 0.5f, 5, false, false, 0 };
    Cost d = { 0, 0, 0, 0, 0, 0 };
    t.GetComponentCost(d, sys);
    if (t.splineCalls != 0) ++fails;
    if (!Near(d.PolyCount, 2.0f + 64.0f) || !Near(d.PixelCount, 900.0f + 96.0f) || !Near(d.RenderCost, 902.0f + 160.0f)) ++fails;
    // no scaling, no banks: nothing added
    System flat = { false, 5.0f, 5.0f, 5.0f };
    Sprite u = { false, 0, 0.0f, 0.0f, 0, 0, 0.0f, 0, false, false, 0 };
    Cost e = { 0, 0, 0, 0, 0, 0 };
    u.GetComponentCost(e, flat);
    if (e.PolyCount != 0.0f || e.PixelCount != 0.0f || e.RenderCost != 0.0f) ++fails;
    printf(fails ? "00b05c60_TEST FAIL\n" : "00b05c60_TEST PASS\n");
    return fails ? 1 : 0;
}
