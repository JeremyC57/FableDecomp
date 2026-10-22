#include <stdio.h>
#include <string.h>
// CMap::GetDominantEngineThemeAt(const C2DCoordI&, long*) -- the most-weighted of a game-map cell's three engine themes
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081F910, 263 bytes, `ret 8`; leaf, no calls. Layout: CMap theme palette vector +0x1a6c,
//   game-map grid +0x1a7c (cells +0x10, width-1 +0x204, max Y +0x208); cell 12 bytes (palette indices +4, blends +7).
// Shape: the weight is 3 - blend0 - blend1 for the third theme; the two blend reads take pos by reference, the final
//   theme lookup is an inline helper taking pos BY VALUE (the copy loads X then Y before the grid pointer).
struct C2DCoordI { long X; long Y; };
struct CHeightMapCell {                           // engine/CHeightMapCell.h: 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char NavFlags;                        // +0x09
    unsigned char _pad0a[2];
};
struct CGameMapGrid {                              // the game map's cell grid (+0x1a7c of CMap)
    unsigned char   _pad[0x10];
    CHeightMapCell* m_pCells;                      // +0x10
    unsigned char   _pad14[0x1f0];
    long            m_width;                       // +0x204 (cells per row minus one)
    long            m_maxY;                        // +0x208 (rows are stored top-down)
    CHeightMapCell& GetCell(const C2DCoordI& pos) { return m_pCells[(m_maxY - pos.Y) * (m_width + 1) + pos.X]; }
};
struct SThemePaletteEntry {                        // 8 bytes
    unsigned long ThemeId;
    bool          Used;
};
struct CMap {
    unsigned char       _pad[0x1a6c];
    SThemePaletteEntry* m_pThemePalette;           // +0x1a6c (vector begin)
    SThemePaletteEntry* m_pThemePaletteEnd;        // +0x1a70
    SThemePaletteEntry* m_pThemePaletteCapacity;   // +0x1a74
    void*               m_pQuarterGrid;            // +0x1a78
    CGameMapGrid*       m_pGameMapGrid;            // +0x1a7c
    unsigned long GetEngineThemeAt(C2DCoordI pos, long index) { return m_pThemePalette[m_pGameMapGrid->GetCell(pos).EngineThemePaletteIndex[index]].ThemeId; }
    unsigned long GetDominantEngineThemeAt(const C2DCoordI& pos, long* weight);
};

unsigned long CMap::GetDominantEngineThemeAt(const C2DCoordI& pos, long* weight)
{
    long best = 0;
    long bestIndex = -1;
    long blend0 = m_pGameMapGrid->GetCell(pos).EngineBlend[0];
    if (blend0 > 0) {
        bestIndex = 0;
        best = blend0;
    }
    long remaining = 3 - blend0;
    long blend1 = m_pGameMapGrid->GetCell(pos).EngineBlend[1];
    if (blend1 > best) {
        bestIndex = 1;
        best = blend1;
    }
    remaining -= blend1;
    if (remaining > best) {
        *weight = remaining;
        return GetEngineThemeAt(pos, 2);
    }
    *weight = best;
    return GetEngineThemeAt(pos, bestIndex);
}

// ---- test ----
static int fails;
static void check(int ok, const char* what) { if (!ok) { char b[120]; _snprintf(b, sizeof b, "FAIL %s", what); puts(b); fails++; } }
static double g_mapStore[0x1a80 / 8 + 1];
static double g_gridStore[0x210 / 8 + 1];
static CHeightMapCell g_cells[4 * 3];
static SThemePaletteEntry g_pal[8];
static unsigned long run(long x, long y, unsigned char b0, unsigned char b1, long* w)
{
    CMap* map = (CMap*)g_mapStore; CGameMapGrid* grid = (CGameMapGrid*)g_gridStore;
    map->m_pThemePalette = g_pal; map->m_pGameMapGrid = grid;
    grid->m_pCells = g_cells; grid->m_width = 3; grid->m_maxY = 2;
    CHeightMapCell& c = g_cells[(2 - y) * 4 + x];
    c.EngineBlend[0] = b0; c.EngineBlend[1] = b1;
    c.EngineThemePaletteIndex[0] = 5; c.EngineThemePaletteIndex[1] = 6; c.EngineThemePaletteIndex[2] = 7;
    C2DCoordI pos; pos.X = x; pos.Y = y;
    return map->GetDominantEngineThemeAt(pos, w);
}
int main()
{
    for (int i = 0; i < 8; ++i) { g_pal[i].ThemeId = 100 + i; g_pal[i].Used = false; }
    long w = -9;
    check(run(1, 0, 2, 1, &w) == 105 && w == 2, "blend0 dominant");
    check(run(2, 1, 0, 2, &w) == 106 && w == 2, "blend1 dominant");
    check(run(3, 2, 1, 0, &w) == 107 && w == 2, "third dominant (remaining)");
    check(run(0, 0, 1, 1, &w) == 105 && w == 1, "tie: blend0 kept, remaining not greater");
    check(run(0, 1, 0, 0, &w) == 107 && w == 3, "both zero: third gets 3");
    g_pal[255 & 0].ThemeId = 100;
    if (fails) { puts("0081f910_TEST FAIL"); return 1; }
    puts("0081f910_TEST PASS");
    return 0;
}