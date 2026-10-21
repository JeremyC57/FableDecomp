// Behaviour test for 0x0063EB20 (CWorldMap::DestroyThemeAt): on a position inside the world map whose strongest theme
// has a destruction theme, paint it on layer 0, record the block, clear layers 1-2, full blend, and reset the loaded
// changeable map's local engine blend.
#include <cstdio>
#include <vector>
struct V3 { float X, Y, Z; };
struct Coord { long X, Y; };
static unsigned long g_theme[3];           // per layer at the single test block
static unsigned char g_blend = 0;
static std::vector<Coord> g_destroyed;
static long g_mapBlendX = -1, g_mapBlendY = -1;
static long g_destructionTheme = 7;
static bool g_changeable = true;
static int g_releases = 0;
static bool InMap(const V3& p) { return p.X >= 0 && p.Y >= 0 && p.X < 100 && p.Y < 100; }
static void DestroyThemeAt(const V3& pos)
{
    if (!InMap(pos))
        return;
    long destruction = g_destructionTheme;     // def lookup of the strongest theme
    if (destruction != 0) {
        Coord block = { (long)pos.X, (long)pos.Y };
        g_theme[0] = destruction;
        g_destroyed.push_back(block);
        for (long i = 1; i < 3; ++i)
            g_theme[i] = 0;
        g_blend = 0xff;
        long map = block.X < 50 ? 1 : 0;
        if (map != 0 && g_changeable) {
            g_mapBlendX = block.X - 0; g_mapBlendY = block.Y - 0;
        }
    }
    ++g_releases;                              // the def pointer is released on every path past the map test
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    V3 out = { -1.0f, 5.0f, 0 };
    DestroyThemeAt(out);
    CHECK(g_destroyed.empty() && g_releases == 0);
    g_theme[0] = 3; g_theme[1] = 4; g_theme[2] = 5;
    V3 p = { 10.7f, 20.2f, 0 };
    DestroyThemeAt(p);
    CHECK(g_theme[0] == 7 && g_theme[1] == 0 && g_theme[2] == 0 && g_blend == 0xff);
    CHECK(g_destroyed.size() == 1 && g_destroyed[0].X == 10 && g_destroyed[0].Y == 20);
    CHECK(g_mapBlendX == 10 && g_mapBlendY == 20 && g_releases == 1);
    g_destructionTheme = 0; g_theme[0] = 3;
    DestroyThemeAt(p);
    CHECK(g_theme[0] == 3 && g_destroyed.size() == 1 && g_releases == 2);
    g_destructionTheme = 9; g_mapBlendX = -1;
    V3 q = { 60.0f, 1.0f, 0 };                 // block on map 0: no engine blend reset
    DestroyThemeAt(q);
    CHECK(g_theme[0] == 9 && g_destroyed.size() == 2 && g_mapBlendX == -1);
    if (g_fail) { std::printf("0063eb20_TEST FAIL\n"); return 1; }
    std::printf("0063eb20_TEST PASS\n");
    return 0;
}