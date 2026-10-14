#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CHeightMapCell { float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavFlags0 : 1; unsigned char NavFlags1 : 1; unsigned char NavFlags2 : 1; unsigned char CameraPassable : 1; unsigned char NavFlags4 : 4; unsigned char _pad0a[2]; };
struct CGameMapGrid { unsigned char _pad[0x10]; CHeightMapCell* m_pCells; unsigned char _pad14[0x1f0]; long m_width; long m_maxY; };
struct C2DCoordI { long X; long Y; };
static CHeightMapCell g_cells[12]; static CGameMapGrid g_grid;
static void SetupGrid() { std::memset(g_cells, 0, sizeof g_cells); g_grid.m_pCells = g_cells; g_grid.m_width = 2; g_grid.m_maxY = 3; for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].EngineThemePaletteIndex[0] = (unsigned char)i; g_cells[i].EngineThemePaletteIndex[1] = (unsigned char)(i + 1); g_cells[i].EngineBlend[0] = (unsigned char)(i * 2); g_cells[i].EngineBlend[1] = (unsigned char)(i * 3); } }
struct SThemePaletteEntry { unsigned long m_themeId; unsigned long m_reserved; };
struct CMap { unsigned char _pad[0x1a6c]; SThemePaletteEntry* m_pPalette; unsigned char _pad1a70[0xc]; CGameMapGrid* m_pGameMapGrid;
    unsigned long DrawGetEngineThemeAt(long x, long y, long which) { CGameMapGrid* grid = m_pGameMapGrid; return m_pPalette[grid->m_pCells[(grid->m_maxY - y) * (grid->m_width + 1) + x].EngineThemePaletteIndex[which]].m_themeId; } };

int main()
{
    SetupGrid(); SThemePaletteEntry palette[16]; for (int i = 0; i < 16; ++i) { palette[i].m_themeId = 1000 + i; palette[i].m_reserved = 0; } CMap* map = new CMap; map->m_pPalette = palette; map->m_pGameMapGrid = &g_grid;
    if (map->DrawGetEngineThemeAt(1, 2, 0) != 1004 || map->DrawGetEngineThemeAt(1, 2, 1) != 1005 || map->DrawGetEngineThemeAt(2, 0, 0) != 1011) { std::printf("008f75b0_TEST FAIL\n"); return 1; } delete map;
    std::printf("008f75b0_TEST PASS\n");
    return 0;
}