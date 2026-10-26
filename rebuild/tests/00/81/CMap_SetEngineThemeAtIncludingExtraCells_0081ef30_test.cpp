// Behaviour test for 0x0081EF30 CMap::SetEngineThemeAtIncludingExtraCells: palette index stored in the addressed cell layer, zero clears it.
#include <cstdio>
struct Cell { float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char Flags; };
struct HeightMap {
    Cell* Cells; int SizeX, SizeY;
    Cell& GetCellIncludingExtraCells(int x, int y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
class CMap { public:
    HeightMap* hm; unsigned int lastTheme;
    unsigned char AddThemeDefIndexToPalette(unsigned int t) { lastTheme = t; return (unsigned char)(t + 1); }
    void SetEngineThemeAtIncludingExtraCells(int x, int y, unsigned int theme, int layer)
    {
        if (theme > 0) { int p = AddThemeDefIndexToPalette(theme); hm->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = (unsigned char)p; }
        else hm->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = 0;
    }
};
int main()
{
    static Cell cells[5 * 4];
    HeightMap h; h.Cells = cells; h.SizeX = 4; h.SizeY = 3;
    CMap m; m.hm = &h; m.lastTheme = 0;
    m.SetEngineThemeAtIncludingExtraCells(2, 1, 9, 1);         // index (3-1)*5+2 = 12
    bool ok = cells[12].EngineThemePaletteIndex[1] == 10 && m.lastTheme == 9;
    m.SetEngineThemeAtIncludingExtraCells(2, 1, 0, 1);
    ok = ok && cells[12].EngineThemePaletteIndex[1] == 0 && m.lastTheme == 9;
    if (!ok) { std::printf("0081ef30_TEST FAIL\n"); return 1; }
    std::printf("0081ef30_TEST PASS\n");
    return 0;
}