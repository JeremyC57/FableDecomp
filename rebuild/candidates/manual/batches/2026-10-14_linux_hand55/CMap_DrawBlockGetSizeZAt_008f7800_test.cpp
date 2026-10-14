#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CHeightMapCell { float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavFlags0 : 1; unsigned char NavFlags1 : 1; unsigned char NavFlags2 : 1; unsigned char CameraPassable : 1; unsigned char NavFlags4 : 4; unsigned char _pad0a[2]; };
struct CGameMapGrid { unsigned char _pad[0x10]; CHeightMapCell* m_pCells; unsigned char _pad14[0x1f0]; long m_width; long m_maxY; };
struct C2DCoordI { long X; long Y; };
static CHeightMapCell g_cells[12]; static CGameMapGrid g_grid;
static void SetupGrid() { std::memset(g_cells, 0, sizeof g_cells); g_grid.m_pCells = g_cells; g_grid.m_width = 2; g_grid.m_maxY = 3; for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].EngineThemePaletteIndex[0] = (unsigned char)i; g_cells[i].EngineThemePaletteIndex[1] = (unsigned char)(i + 1); g_cells[i].EngineBlend[0] = (unsigned char)(i * 2); g_cells[i].EngineBlend[1] = (unsigned char)(i * 3); } }
struct CMap { unsigned char _pad[0x1a7c]; CGameMapGrid* m_pGameMapGrid;
    float DrawBlockGetSizeZAt(long x, long y, long which) { CGameMapGrid* grid = m_pGameMapGrid; return grid->m_pCells[(grid->m_maxY - y) * (grid->m_width + 1) + x].Height; } };

int main()
{
    SetupGrid(); CMap* map = new CMap; map->m_pGameMapGrid = &g_grid; if (map->DrawBlockGetSizeZAt(1, 2, 5) != 4.0f || map->DrawBlockGetSizeZAt(2, 0, 0) != 11.0f) { std::printf("008f7800_TEST FAIL\n"); return 1; } delete map;
    std::printf("008f7800_TEST PASS\n");
    return 0;
}
