// Behaviour test for CMap::CopyPos 0x0081F240: a copy of the model (cell grid stored top-down with one extra column/row, an
// 8-byte-entry theme palette per map) and of the function's logic. Checks the whole-cell copy, that each theme layer is
// re-registered in the DESTINATION palette (indices differ between the two maps), and that both blend bytes are copied.
#include <cstdio>
#include <cstring>

class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};

class CHeightMapCell { public:
    float         Height;
    unsigned char EngineThemePaletteIndex[3];
    unsigned char EngineBlend[2];
    unsigned char Flags;
    unsigned char _pad[2];
    unsigned char GetEngineThemePaletteIndex(long i) const { return EngineThemePaletteIndex[i]; }
    unsigned char GetEngineBlend(long i) const { return EngineBlend[i]; }
    void SetEngineBlend(unsigned char blend, long i) { EngineBlend[i] = blend; }
};

class CThemePaletteEntry { public:
    unsigned long ThemeDefIndex;
    bool          ThemePaletteIndexUsed;
};

class CHeightMap { public:
    CHeightMapCell Cells[64];
    long SizeX, SizeY;
    CHeightMapCell& GetCell(const C2DCoordI& pos) { return Cells[(SizeY - pos.Y) * (SizeX + 1) + pos.X]; }
};

class CMap { public:
    CThemePaletteEntry ThemePaletteEntryList[16];
    CHeightMap*        HeightMap;

    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex)
    {
        unsigned long i;
        for (i = 1; i < 16; ++i)
            if (ThemePaletteEntryList[i].ThemePaletteIndexUsed && ThemePaletteEntryList[i].ThemeDefIndex == themeDefIndex)
                return (unsigned char)i;
        for (i = 1; i < 16; ++i)
            if (!ThemePaletteEntryList[i].ThemePaletteIndexUsed) {
                ThemePaletteEntryList[i].ThemeDefIndex = themeDefIndex;
                ThemePaletteEntryList[i].ThemePaletteIndexUsed = true;
                return (unsigned char)i;
            }
        return 0;
    }
    void SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, unsigned long layer)
    {
        if (themeDefIndex > 0) {
            long paletteIndex = AddThemeDefIndexToPalette(themeDefIndex);
            HeightMap->GetCell(C2DCoordI(x, y)).EngineThemePaletteIndex[layer] = (unsigned char)paletteIndex;
        } else
            HeightMap->GetCell(C2DCoordI(x, y)).EngineThemePaletteIndex[layer] = 0;
    }
    unsigned long GetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long layer) const
    {
        return ThemePaletteEntryList[HeightMap->GetCell(C2DCoordI(x, y)).GetEngineThemePaletteIndex(layer)].ThemeDefIndex;
    }
    unsigned char GetEngineBlendAtIncludingExtraCells(long x, long y, unsigned long index) const
    {
        return HeightMap->GetCell(C2DCoordI(x, y)).GetEngineBlend(index);
    }
    void SetEngineBlendAtIncludingExtraCells(long x, long y, unsigned char blend, unsigned long index)
    {
        HeightMap->GetCell(C2DCoordI(x, y)).SetEngineBlend(blend, index);
    }
    void CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to)
    {
        HeightMap->GetCell(to) = source.HeightMap->GetCell(from);
        for (long i = 0; i < 3; ++i)
            SetEngineThemeAtIncludingExtraCells(to.X, to.Y, source.GetEngineThemeAtIncludingExtraCells(from.X, from.Y, i), i);
        SetEngineBlendAtIncludingExtraCells(to.X, to.Y, source.GetEngineBlendAtIncludingExtraCells(from.X, from.Y, 0), 0);
        SetEngineBlendAtIncludingExtraCells(to.X, to.Y, source.GetEngineBlendAtIncludingExtraCells(from.X, from.Y, 1), 1);
    }
};

static CHeightMap g_srcHeight, g_dstHeight;
static CMap g_src, g_dst;

int main()
{
    std::memset(&g_srcHeight, 0, sizeof(g_srcHeight));
    std::memset(&g_dstHeight, 0, sizeof(g_dstHeight));
    std::memset(&g_src, 0, sizeof(g_src));
    std::memset(&g_dst, 0, sizeof(g_dst));
    g_srcHeight.SizeX = 4; g_srcHeight.SizeY = 5;        // 5 columns x 6 rows = 30 cells
    g_dstHeight.SizeX = 6; g_dstHeight.SizeY = 3;        // 7 columns x 4 rows = 28 cells
    g_src.HeightMap = &g_srcHeight;
    g_dst.HeightMap = &g_dstHeight;

    // Source palette: entry 2 = theme 700, entry 5 = theme 900, entry 7 = theme 0 (cleared layer).
    g_src.ThemePaletteEntryList[2].ThemeDefIndex = 700; g_src.ThemePaletteEntryList[2].ThemePaletteIndexUsed = true;
    g_src.ThemePaletteEntryList[5].ThemeDefIndex = 900; g_src.ThemePaletteEntryList[5].ThemePaletteIndexUsed = true;
    g_src.ThemePaletteEntryList[7].ThemeDefIndex = 0;   g_src.ThemePaletteEntryList[7].ThemePaletteIndexUsed = true;
    // Destination palette already holds theme 900 at entry 1 and theme 123 at entry 2; theme 700 is new there (first free entry is 3).
    g_dst.ThemePaletteEntryList[1].ThemeDefIndex = 900; g_dst.ThemePaletteEntryList[1].ThemePaletteIndexUsed = true;
    g_dst.ThemePaletteEntryList[2].ThemeDefIndex = 123; g_dst.ThemePaletteEntryList[2].ThemePaletteIndexUsed = true;

    C2DCoordI from(3, 2), to(5, 1);
    CHeightMapCell& s = g_srcHeight.Cells[(5 - 2) * 5 + 3];     // cell 18
    s.Height = 12.5f;
    s.EngineThemePaletteIndex[0] = 5;      // theme 900
    s.EngineThemePaletteIndex[1] = 2;      // theme 700
    s.EngineThemePaletteIndex[2] = 7;      // theme 0
    s.EngineBlend[0] = 0x40;
    s.EngineBlend[1] = 0xC3;
    s.Flags = 0x15;
    CHeightMapCell& d = g_dstHeight.Cells[(3 - 1) * 7 + 5];     // cell 19
    d.Height = -1.0f;
    d.EngineThemePaletteIndex[0] = d.EngineThemePaletteIndex[1] = d.EngineThemePaletteIndex[2] = 9;
    d.EngineBlend[0] = 1; d.EngineBlend[1] = 2;

    g_dst.CopyPos(g_src, from, to);

    bool ok = true;
    if (&g_srcHeight.GetCell(from) != &s || &g_dstHeight.GetCell(to) != &d) ok = false;
    if (d.Height != 12.5f || d.Flags != 0x15) ok = false;                 // whole-cell copy
    if (d.EngineThemePaletteIndex[0] != 1) ok = false;                     // theme 900 already at dst entry 1
    if (d.EngineThemePaletteIndex[1] != 3) ok = false;                     // theme 700 added at dst's first free entry
    if (g_dst.ThemePaletteEntryList[3].ThemeDefIndex != 700 || !g_dst.ThemePaletteEntryList[3].ThemePaletteIndexUsed) ok = false;
    if (d.EngineThemePaletteIndex[2] != 0) ok = false;                     // theme 0 clears the layer
    if (d.EngineBlend[0] != 0x40 || d.EngineBlend[1] != 0xC3) ok = false;  // both blend bytes
    // The source is untouched and no neighbour cell of the destination was written.
    if (s.EngineThemePaletteIndex[0] != 5 || s.EngineThemePaletteIndex[1] != 2 || s.EngineBlend[1] != 0xC3) ok = false;
    for (int i = 0; i < 28; ++i)
        if (i != 19 && (g_dstHeight.Cells[i].Height != 0.0f || g_dstHeight.Cells[i].EngineBlend[0] != 0 ||
                        g_dstHeight.Cells[i].EngineThemePaletteIndex[0] != 0))
            ok = false;
    if (!ok) { std::printf("0081f240_TEST FAIL\n"); return 1; }
    std::printf("0081f240_TEST PASS\n");
    return 0;
}