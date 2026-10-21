// Behaviour test for 0x0063A420 (CWorldMap::GetWaterSizeZAt): all three sample cells (pos, +X, +Y) must be on the map
// and on the same loaded map; the water offset counts only above 0.05.
#include <cstdio>
struct V3 { float X, Y, Z; };
static long MapAt(long x, long y) { return x < 10 ? 1 : 2; }            // map 1 covers x < 10
static bool InMap(long x, long y) { return x >= 0 && y >= 0 && x < 20 && y < 20; }
static bool g_loaded[3] = { false, true, true };
static float g_water = 0.5f;
static float GetWaterSizeZAt(const V3& pos)
{
    long x1 = (long)(pos.X + 1.0f), y1 = (long)pos.Y;
    long x2 = (long)pos.X, y2 = (long)(pos.Y + 1.0f);
    if (InMap((long)pos.X, (long)pos.Y) && InMap(x1, y1) && InMap(x2, y2)) {
        long map = MapAt((long)pos.X, (long)pos.Y);
        if (map != 0 && MapAt(x1, y1) == map && MapAt(x2, y2) == map && g_loaded[map]) {
            float height = g_water;
            if (height > 0.05f)
                return height;
        }
    }
    return 0.0f;
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    V3 a = { 3.5f, 4.5f, 0 }; CHECK(GetWaterSizeZAt(a) == 0.5f);
    V3 b = { 9.5f, 4.5f, 0 }; CHECK(GetWaterSizeZAt(b) == 0.0f);    // +X neighbour on another map
    V3 c = { 3.5f, 19.5f, 0 }; CHECK(GetWaterSizeZAt(c) == 0.0f);   // +Y neighbour off the world map
    g_loaded[1] = false; CHECK(GetWaterSizeZAt(a) == 0.0f); g_loaded[1] = true;
    g_water = 0.05f; CHECK(GetWaterSizeZAt(a) == 0.0f);
    g_water = 0.06f; CHECK(GetWaterSizeZAt(a) == 0.06f);
    if (g_fail) { std::printf("0063a420_TEST FAIL\n"); return 1; }
    std::printf("0063a420_TEST PASS\n");
    return 0;
}