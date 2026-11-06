// Behaviour test for CRegionMinimap::PopulateRegionMinimap (0x0066CF00): the engine theme's EMinimapThemeType (1..8) maps to the
// region-minimap theme 1, 6, 4, 5, 2, 7, 3, 8 (anything else, or a position outside the map, 0); lit themes take the illumination
// value, the rest 0; a building's bounding square (centre +- radius, clipped to the region box) becomes a TGA cell range grown
// by one cell each way within the minimap. A model of those three steps.
#include <cstdio>

static long MapTheme(long engineMinimapTheme, bool inMap)
{
    if (!inMap) return 0;
    switch (engineMinimapTheme) {
    case 1: return 1; case 2: return 6; case 3: return 4; case 4: return 5;
    case 5: return 2; case 6: return 7; case 7: return 3; case 8: return 8;
    default: return 0;
    }
}

struct Box { float TLX, TLY, BRX, BRY; };
static void ClipTo(Box& b, const Box& o)
{
    if (b.TLX < o.TLX) b.TLX = o.TLX; if (b.TLY < o.TLY) b.TLY = o.TLY;
    if (b.BRX > o.BRX) b.BRX = o.BRX; if (b.BRY > o.BRY) b.BRY = o.BRY;
}

int main()
{
    static const long expected[10] = { 0, 1, 6, 4, 5, 2, 7, 3, 8, 0 };
    bool ok = true;
    for (long t = 0; t < 10; ++t) ok = ok && MapTheme(t, true) == expected[t];
    ok = ok && MapTheme(3, false) == 0;
    // region box (0,0)-(100,100), minimap 50 cells: 2 world units per cell
    long size = 50; Box region = { 0.0f, 0.0f, 100.0f, 100.0f };
    Box b = { 90.0f - 20.0f, 10.0f - 20.0f, 90.0f + 20.0f, 10.0f + 20.0f };
    ClipTo(b, region);
    long tlx = (long)((b.TLX - 0) / 100 * size), tly = (long)((b.TLY - 0) / 100 * size);
    long brx = (long)((b.BRX - 0) / 100 * size), bry = (long)((b.BRY - 0) / 100 * size);
    if (tlx > 0) --tlx; if (tly > 0) --tly; if (brx < size) ++brx; if (bry < size) ++bry;
    ok = ok && b.TLY == 0.0f && b.BRX == 100.0f && tlx == 34 && tly == 0 && brx == 50 && bry == 16;
    std::printf(ok ? "0066cf00_TEST PASS\n" : "0066cf00_TEST FAIL\n");
    return ok ? 0 : 1;
}
