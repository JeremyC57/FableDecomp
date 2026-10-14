#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CHeightMapCell { float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavFlags0 : 1; unsigned char NavFlags1 : 1; unsigned char NavFlags2 : 1; unsigned char CameraPassable : 1; unsigned char NavFlags4 : 4; unsigned char _pad0a[2]; };
struct CGameMapGrid { unsigned char _pad[0x10]; CHeightMapCell* m_pCells; unsigned char _pad14[0x1f0]; long m_width; long m_maxY; };
struct C2DCoordI { long X; long Y; };
static CHeightMapCell g_cells[12]; static CGameMapGrid g_grid;
static void SetupGrid() { std::memset(g_cells, 0, sizeof g_cells); g_grid.m_pCells = g_cells; g_grid.m_width = 2; g_grid.m_maxY = 3; for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].EngineThemePaletteIndex[0] = (unsigned char)i; g_cells[i].EngineThemePaletteIndex[1] = (unsigned char)(i + 1); g_cells[i].EngineBlend[0] = (unsigned char)(i * 2); g_cells[i].EngineBlend[1] = (unsigned char)(i * 3); } }
static long g_locks = 0; struct CEngineGlobals { virtual void Lock() { ++g_locks; } }; static CEngineGlobals* g_pEngineGlobals;
struct CMap { unsigned char _pad[0x1a7c]; CGameMapGrid* m_pGameMapGrid;
    bool IsCameraPassableAt(const C2DCoordI& pos) { g_pEngineGlobals->Lock(); CGameMapGrid* grid = m_pGameMapGrid; return grid->m_pCells[(grid->m_maxY - pos.Y) * (grid->m_width + 1) + pos.X].CameraPassable; } };

int main()
{
    SetupGrid(); CEngineGlobals eng; g_pEngineGlobals = &eng; CMap* map = new CMap; map->m_pGameMapGrid = &g_grid; C2DCoordI p = { 1, 2 }; g_cells[4].CameraPassable = 1; g_cells[4].NavFlags2 = 1; g_cells[5].NavFlags0 = 1;
    if (!map->IsCameraPassableAt(p) || g_locks != 1) { std::printf("0081f460_TEST FAIL\n"); return 1; } p.X = 2; if (map->IsCameraPassableAt(p) || g_locks != 2) { std::printf("0081f460_TEST FAIL\n"); return 1; } p.X = 0; p.Y = 3; g_cells[0].CameraPassable = 1; if (!map->IsCameraPassableAt(p)) { std::printf("0081f460_TEST FAIL\n"); return 1; } delete map;
    std::printf("0081f460_TEST PASS\n");
    return 0;
}