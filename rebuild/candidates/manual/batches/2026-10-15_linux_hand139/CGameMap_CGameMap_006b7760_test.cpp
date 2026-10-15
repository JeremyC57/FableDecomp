// Behaviour test for 0x006B7760 CGameMap constructor.
#include <cstdio>
struct CGameMapCell { unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music; };
static int g_resized = -1, g_cleared = 0;
struct CGameMapCellVector {
    CGameMapCell* a; CGameMapCell* b; CGameMapCell* c;
    CGameMapCellVector() : a(0), b(0), c(0) {}
    void resize(unsigned int n, const CGameMapCell&) { g_resized = (int)n; }
};
struct C2DCoordI { int X, Y; };
struct CGameMap {
    CGameMapCellVector Cells; void* Map; void* World; unsigned int* WorldSeed; int SizeX, SizeY;
    void Clear() { ++g_cleared; }
    CGameMap(void* world, void* map, const C2DCoordI& size) : Map(map), World(world), SizeX(size.X), SizeY(size.Y)
    {
        long long total = (long long)size.X * size.Y;
        if (total >= 0 && total <= 0x7fffffff) { CGameMapCell blank = CGameMapCell(); Cells.resize(size.X * size.Y, blank); Clear(); }
    }
};
static int Fail() { std::printf("006b7760_TEST FAIL\n"); return 1; }
int main()
{
    int w, m;
    C2DCoordI s = { 16, 8 };
    CGameMap a(&w, &m, s);
    if (a.World != &w || a.Map != &m || a.SizeX != 16 || a.SizeY != 8 || g_resized != 128 || g_cleared != 1 || a.Cells.a) return Fail();
    C2DCoordI big = { 0x10000, 0x10000 };
    g_resized = -1; CGameMap b(&w, &m, big);
    if (g_resized != -1 || g_cleared != 1) return Fail();
    C2DCoordI neg = { -2, 3 };
    CGameMap c(&w, &m, neg);
    if (g_resized != -1 || g_cleared != 1) return Fail();
    std::printf("006b7760_TEST PASS\n");
    return 0;
}
