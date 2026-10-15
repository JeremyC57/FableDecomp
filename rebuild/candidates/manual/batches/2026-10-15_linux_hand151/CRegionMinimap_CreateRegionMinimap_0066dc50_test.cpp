// behaviour test for 0x0066DC50 CRegionMinimap::CreateRegionMinimap: cells sized, surface/box set up, populate, build TGA, cells freed
#include <cstdio>
#include <cstdlib>
static int g_order[8]; static int g_n = 0;
struct Cell { int theme; float light; };
struct CCells {
    Cell* first; Cell* last; Cell* end;
    void resize(unsigned int n) { first = (Cell*)std::malloc(n * sizeof(Cell)); last = end = first + n; g_order[g_n++] = 1; }
    void Free() { Cell* p = first; first = 0; last = 0; end = 0; if (p) std::free(p); }
};
struct CRegionMinimap {
    int RegionMinimapSize; int surfW, surfH; unsigned int cellsAtBuild; CCells RegionMinimapCellList;
    void InitialiseSurface() { int size = RegionMinimapSize; surfW = size; surfH = size; g_order[g_n++] = 2; }
    void InitialiseRegionBox() { g_order[g_n++] = 3; }
    void PopulateRegionMinimap(void*) { cellsAtBuild = (unsigned int)(RegionMinimapCellList.last - RegionMinimapCellList.first); g_order[g_n++] = 4; }
    void BuildRegionMinimapTGA(const char*) { g_order[g_n++] = 5; }
    void CreateRegionMinimap(void* map, const char* file)
    {
        RegionMinimapCellList.resize(RegionMinimapSize * RegionMinimapSize);
        InitialiseSurface(); InitialiseRegionBox(); PopulateRegionMinimap(map); BuildRegionMinimapTGA(file);
        RegionMinimapCellList.Free();
    }
};
int main()
{
    CRegionMinimap m; m.RegionMinimapSize = 4;
    m.CreateRegionMinimap(0, "x.tga");
    if (g_n != 5 || g_order[0] != 1 || g_order[4] != 5 || m.cellsAtBuild != 16 || m.surfW != 4 || m.RegionMinimapCellList.first != 0 ||
        m.RegionMinimapCellList.end != 0) { std::printf("0066dc50_TEST FAIL\n"); return 1; }
    std::printf("0066dc50_TEST PASS\n");
    return 0;
}
