#include <cstdio>
#include <cstdlib>
#include <cstring>
class CHeightMapCell { public: float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavFlags0 : 1; unsigned char NavFlags1 : 1; unsigned char NavFlags2 : 1; unsigned char CameraPassable : 1; unsigned char NavFlags4 : 4; unsigned char _pad0a[2]; };
struct CGameMapGrid { unsigned char _pad[0x10]; CHeightMapCell* m_pCells; unsigned char _pad14[0x1f0]; long m_width; long m_maxY; };
class C2DCoordI { public: long X; long Y; };
static CHeightMapCell g_cells[12]; static CGameMapGrid g_grid;
static void SetupGrid() { std::memset(g_cells, 0, sizeof g_cells); g_grid.m_pCells = g_cells; g_grid.m_width = 2; g_grid.m_maxY = 3; for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].EngineThemePaletteIndex[0] = (unsigned char)i; g_cells[i].EngineThemePaletteIndex[1] = (unsigned char)(i + 1); g_cells[i].EngineBlend[0] = (unsigned char)(i * 2); g_cells[i].EngineBlend[1] = (unsigned char)(i * 3); } }
class CMap { public: unsigned char _pad[0x1a7c]; CGameMapGrid* m_pGameMapGrid;
    unsigned char DrawGetEngineBlendAt(long x, long y, long which) { CGameMapGrid* grid = m_pGameMapGrid; return grid->m_pCells[(grid->m_maxY - y) * (grid->m_width + 1) + x].EngineBlend[which]; } };

int main()
{
    SetupGrid(); CMap* map = new CMap; map->m_pGameMapGrid = &g_grid; if (map->DrawGetEngineBlendAt(1, 2, 0) != 8 || map->DrawGetEngineBlendAt(1, 2, 1) != 12 || map->DrawGetEngineBlendAt(2, 0, 1) != 33) { std::printf("008f75f0_TEST FAIL\n"); return 1; } delete map;
    std::printf("008f75f0_TEST PASS\n");
    return 0;
}