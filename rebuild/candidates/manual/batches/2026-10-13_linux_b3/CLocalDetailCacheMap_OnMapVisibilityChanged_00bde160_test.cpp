#include <cstdio>

static int g_unload = 0, g_flush = 0;
struct Mgr { void RemoveFinishedBlocks() { ++g_flush; } };
struct QT { void UnloadStaticContents(Mgr*) { ++g_unload; } };
struct M { void* a; Mgr* FileBlockManager; void* b; QT QuadTree;
    void On(bool v) { if (!v) { QuadTree.UnloadStaticContents(FileBlockManager); Mgr* m = FileBlockManager; if (m) m->RemoveFinishedBlocks(); } } };
int main()
{
    Mgr mg; M m; m.FileBlockManager = &mg; m.On(true); if (g_unload) { std::printf("00bde160_TEST FAIL\n"); return 1; }
    m.On(false); if (g_unload != 1 || g_flush != 1) { std::printf("00bde160_TEST FAIL\n"); return 1; }
    m.FileBlockManager = 0; m.On(false); if (g_unload != 2 || g_flush != 1) { std::printf("00bde160_TEST FAIL\n"); return 1; }
    std::printf("00bde160_TEST PASS\n");
    return 0;
}
