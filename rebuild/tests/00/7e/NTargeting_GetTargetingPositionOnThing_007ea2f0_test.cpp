// Behaviour test for NTargeting::GetTargetingPositionOnThing (0x007EA2F0): host model -- bullet-time rescale of the interpolate,
// the first of four helper points that exists, else the interpolated physics position raised by 0.25.
#include <stdio.h>
#include <math.h>

struct Vec { float X, Y, Z; };
struct Appearance {
    int present[4];            // which helper names exist
    Vec point[4];
    float lastScale;
    bool GetHelperPoint(int name, Vec& out, float scale) { lastScale = scale; if (!present[name]) return false; out = point[name]; return true; }
};
struct Thing {
    Appearance* gfx;
    Vec pos, oldPos;
    bool affected;
};
struct World { bool bulletOverride; int bulletSources; };

static float Convert(float t) { return t * 0.5f; }

static Vec GetTargetingPositionOnThing(const World& world, Thing& thing, float scale)
{
    if ((world.bulletOverride || world.bulletSources != 0) && thing.affected)
        scale = Convert(scale);
    Vec pos;
    if (thing.gfx) {
        for (int i = 0; i < 4; ++i)
            if (thing.gfx->GetHelperPoint(i, pos, scale))
                return pos;
    }
    pos.X = (thing.pos.X - thing.oldPos.X) * scale + thing.oldPos.X;
    pos.Y = (thing.pos.Y - thing.oldPos.Y) * scale + thing.oldPos.Y;
    pos.Z = (thing.pos.Z - thing.oldPos.Z) * scale + thing.oldPos.Z + 0.25f;
    return pos;
}

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int fails = 0;
    World w = { false, 0 };
    Appearance g = { { 0, 0, 1, 1 }, { { 0, 0, 0 }, { 0, 0, 0 }, { 3, 4, 5 }, { 6, 7, 8 } }, 0.0f };
    Thing t = { &g, { 2, 4, 6 }, { 0, 0, 0 }, true };
    Vec r = GetTargetingPositionOnThing(w, t, 1.0f);
    if (!Near(r.X, 3) || !Near(r.Y, 4) || !Near(r.Z, 5)) ++fails;        // third helper wins
    t.gfx = 0;
    r = GetTargetingPositionOnThing(w, t, 0.5f);
    if (!Near(r.X, 1) || !Near(r.Y, 2) || !Near(r.Z, 3.25f)) ++fails;     // fallback, raised by a quarter
    w.bulletSources = 1;
    r = GetTargetingPositionOnThing(w, t, 0.5f);                          // rescaled to 0.25
    if (!Near(r.X, 0.5f) || !Near(r.Z, 1.75f)) ++fails;
    t.affected = false;
    r = GetTargetingPositionOnThing(w, t, 0.5f);
    if (!Near(r.X, 1)) ++fails;
    t.gfx = &g; g.present[2] = 0; g.present[3] = 0;
    r = GetTargetingPositionOnThing(w, t, 1.0f);
    if (!Near(r.Z, 6.25f)) ++fails;
    if (fails == 0) printf("007ea2f0_TEST PASS\n"); else printf("007ea2f0_TEST FAIL %d\n", fails);
    return fails;
}