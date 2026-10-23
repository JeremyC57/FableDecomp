#include <cstdio>

struct C3DAnimationStats {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DAnimationStats* g_last_dtor = 0;

void __fastcall C3DAnimationStats_dtor(C3DAnimationStats* stats)
{
    g_last_dtor = stats;
    stats->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DAnimationStats_scalar_deleting_destructor(C3DAnimationStats* stats, int edx_unused, unsigned int flags)
{
    C3DAnimationStats_dtor(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}

int main()
{
    C3DAnimationStats a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DAnimationStats_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DAnimationStats b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DAnimationStats_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a555a0_TEST PASS\n");
    return 0;
}