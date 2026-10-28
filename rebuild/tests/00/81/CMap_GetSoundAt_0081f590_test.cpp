extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x0081F590 CMap::GetSoundAt: the game-map cell is the map position divided by 4 (C division, toward
// zero) on each axis, and the result is that cell's sound theme.
class C2DCoordI { public: long X, Y; };
class CGameMap { public: long width; long themes[64]; long PeekCellTheme(const C2DCoordI& c) const { return themes[c.Y * width + c.X]; } };
class CMap { public: const CGameMap* GameMap; long GetSoundAt(const C2DCoordI& p) const { C2DCoordI c = { p.X / 4, p.Y / 4 }; return GameMap->PeekCellTheme(c); } };
int main()
{
    CGameMap g; g.width = 8; for (int i = 0; i < 64; ++i) g.themes[i] = 100 + i;
    CMap m; m.GameMap = &g;
    C2DCoordI a = { 0, 0 }, b = { 7, 3 }, c = { 9, 13 }, d = { 31, 31 };
    CHECK(m.GetSoundAt(a) == 100); CHECK(m.GetSoundAt(b) == 101);
    CHECK(m.GetSoundAt(c) == 100 + 3 * 8 + 2); CHECK(m.GetSoundAt(d) == 100 + 63);
    if (fails == 0) printf("0081f590_TEST PASS\n"); else printf("0081f590_TEST FAIL\n");
    return fails;
}