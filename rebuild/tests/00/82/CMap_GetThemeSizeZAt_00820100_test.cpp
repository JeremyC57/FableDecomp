#include <cstdio>
#include <cstdlib>
#include <cstring>
class CHeightMapCell { public: float Height; unsigned char EngineThemePaletteIndex[3]; unsigned char EngineBlend[2]; unsigned char NavFlags0 : 1; unsigned char NavFlags1 : 1; unsigned char NavFlags2 : 1; unsigned char CameraPassable : 1; unsigned char NavFlags4 : 4; unsigned char _pad0a[2]; };
struct CGameMapGrid { unsigned char _pad[0x10]; CHeightMapCell* m_pCells; unsigned char _pad14[0x1f0]; long m_width; long m_maxY; };
class C2DCoordI { public: long X; long Y; };
static CHeightMapCell g_cells[12]; static CGameMapGrid g_grid;
static void SetupGrid() { std::memset(g_cells, 0, sizeof g_cells); g_grid.m_pCells = g_cells; g_grid.m_width = 2; g_grid.m_maxY = 3; for (int i = 0; i < 12; ++i) { g_cells[i].Height = (float)i; g_cells[i].EngineThemePaletteIndex[0] = (unsigned char)i; g_cells[i].EngineThemePaletteIndex[1] = (unsigned char)(i + 1); g_cells[i].EngineBlend[0] = (unsigned char)(i * 2); g_cells[i].EngineBlend[1] = (unsigned char)(i * 3); } }
static const C2DCoordI* g_lastPos = 0;
class CMap { public: unsigned char _pad[0x1a7c]; CGameMapGrid* m_pGameMapGrid;
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) { g_lastPos = &pos; return 100.0f; }
    float GetThemeSizeZAt(const C2DCoordI& pos) { CGameMapGrid* grid = m_pGameMapGrid; long index = (grid->m_maxY - pos.Y) * (grid->m_width + 1) + pos.X; CHeightMapCell* cells = grid->m_pCells; return GetThemeMinCameraSizeZOffsetAt(pos) + cells[index].Height; } };

int main()
{
    SetupGrid(); CMap* map = new CMap; map->m_pGameMapGrid = &g_grid; C2DCoordI p = { 1, 2 }; if (map->GetThemeSizeZAt(p) != 4.0f + 100.0f || g_lastPos != &p) { std::printf("00820100_TEST FAIL\n"); return 1; } p.X = 2; p.Y = 0; if (map->GetThemeSizeZAt(p) != 111.0f) { std::printf("00820100_TEST FAIL\n"); return 1; } delete map;
    std::printf("00820100_TEST PASS\n");
    return 0;
}