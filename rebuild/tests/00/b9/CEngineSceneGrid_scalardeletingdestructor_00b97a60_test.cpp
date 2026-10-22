#include <cstdio>

struct CEngineSceneGrid {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineSceneGrid* g_last_dtor = 0;

void __fastcall CEngineSceneGrid_dtor(CEngineSceneGrid* grid)
{
    g_last_dtor = grid;
    grid->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineSceneGrid_scalar_deleting_destructor(CEngineSceneGrid* grid, int edx_unused, unsigned int flags)
{
    CEngineSceneGrid_dtor(grid);
    if (flags & 1)
        operator delete(grid);
    return grid;
}

int main()
{
    CEngineSceneGrid a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineSceneGrid_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineSceneGrid b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineSceneGrid_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}