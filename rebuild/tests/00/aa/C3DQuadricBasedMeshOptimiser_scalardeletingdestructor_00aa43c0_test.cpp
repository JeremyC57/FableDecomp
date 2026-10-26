#include <cstdio>

class C3DQuadricBasedMeshOptimiser { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DQuadricBasedMeshOptimiser* g_last_dtor = 0;

void __fastcall C3DQuadricBasedMeshOptimiser_dtor(C3DQuadricBasedMeshOptimiser* optimiser)
{
    g_last_dtor = optimiser;
    optimiser->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DQuadricBasedMeshOptimiser_scalar_deleting_destructor(C3DQuadricBasedMeshOptimiser* optimiser, int edx_unused, unsigned int flags)
{
    C3DQuadricBasedMeshOptimiser_dtor(optimiser);
    if (flags & 1)
        operator delete(optimiser);
    return optimiser;
}

int main()
{
    C3DQuadricBasedMeshOptimiser a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DQuadricBasedMeshOptimiser_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DQuadricBasedMeshOptimiser b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DQuadricBasedMeshOptimiser_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aa43c0_TEST PASS\n");
    return 0;
}