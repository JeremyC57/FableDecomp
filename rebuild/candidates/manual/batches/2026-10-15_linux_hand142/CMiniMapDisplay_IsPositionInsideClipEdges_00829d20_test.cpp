#include <cstdio>
// Host model of the four-edge clip test: each enabled edge rejects positions beyond the Z / Y limits.
struct Vec { float X, Y, Z; };
static const float g_edgeZ = 51.0f;
static const float g_edgeY = 4740.0f;
struct Clip {
    bool aboveZ, belowZ, belowY, aboveY;
    bool Inside(const Vec& p) const
    {
        if (aboveZ && p.Z > g_edgeZ) return false;
        if (belowZ && p.Z <= g_edgeZ) return false;
        if (belowY && p.Y <= g_edgeY) return false;
        if (aboveY && p.Y > g_edgeY) return false;
        return true;
    }
};
int main()
{
    int fail = 0;
    Clip c = { false, false, false, false };
    Vec lo = { 0.0f, 100.0f, 10.0f };
    Vec hi = { 0.0f, 5000.0f, 60.0f };
    if (!c.Inside(lo) || !c.Inside(hi)) fail = 1;
    c.aboveZ = true;
    if (!c.Inside(lo) || c.Inside(hi)) fail = 1;
    c.aboveZ = false; c.belowZ = true;
    if (c.Inside(lo) || !c.Inside(hi)) fail = 1;
    c.belowZ = false; c.belowY = true;
    if (c.Inside(lo) || !c.Inside(hi)) fail = 1;
    c.belowY = false; c.aboveY = true;
    if (!c.Inside(lo) || c.Inside(hi)) fail = 1;
    if (fail) { std::printf("00829d20_TEST FAIL\n"); return 1; }
    std::printf("00829d20_TEST PASS\n");
    return 0;
}
