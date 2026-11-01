// Test for CEngineSceneGrid::~CEngineSceneGrid (0x00B93E40): every non-null grid cell is destroyed and deleted (null
// slots skipped), the cell array buffer is freed, then the base destructor runs.
#include <cstdio>
#include <cstdlib>
static int g_cellDtors = 0, g_frees = 0, g_baseDtor = 0;
struct Cell { int id; ~Cell() { ++g_cellDtors; } };
struct Base { virtual ~Base() { ++g_baseDtor; } };
struct Array {
    Cell** start; Cell** finish; Cell** eos;
    unsigned long size() const { return (unsigned long)(finish - start); }
    Cell*& operator[](unsigned long n) { return *(start + n); }
    ~Array() { if (start) { std::free(start); ++g_frees; } }
};
struct SceneGrid : Base {
    unsigned long Level; SceneGrid* LevelUp; SceneGrid* LevelDown; unsigned long Size;
    Array GridCells;
    ~SceneGrid() { for (unsigned long i = 0; i < GridCells.size(); ++i) delete GridCells[i]; }
};
int main()
{
    SceneGrid* g = new SceneGrid;
    g->GridCells.start = (Cell**)std::malloc(4 * sizeof(Cell*)); g->GridCells.finish = g->GridCells.start + 4; g->GridCells.eos = g->GridCells.finish;
    g->GridCells[0] = new Cell; g->GridCells[1] = 0; g->GridCells[2] = new Cell; g->GridCells[3] = new Cell;
    delete g;
    if (g_cellDtors != 3 || g_frees != 1 || g_baseDtor != 1) { std::printf("00b93e40_TEST FAIL\n"); return 1; }
    g = new SceneGrid; g->GridCells.start = g->GridCells.finish = g->GridCells.eos = 0;
    delete g;
    if (g_cellDtors != 3 || g_frees != 1 || g_baseDtor != 2) { std::printf("00b93e40_TEST FAIL\n"); return 1; }
    std::printf("00b93e40_TEST PASS\n");
    return 0;
}