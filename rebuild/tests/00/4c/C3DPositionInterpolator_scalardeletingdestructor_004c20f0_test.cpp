#include <cstdio>

struct C3DPositionInterpolator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DPositionInterpolator* g_last_dtor = 0;

void __fastcall C3DPositionInterpolator_dtor(C3DPositionInterpolator* interpolator)
{
    g_last_dtor = interpolator;
    interpolator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DPositionInterpolator_scalar_deleting_destructor(C3DPositionInterpolator* interpolator, int edx_unused, unsigned int flags)
{
    C3DPositionInterpolator_dtor(interpolator);
    if (flags & 1)
        operator delete(interpolator);
    return interpolator;
}

int main()
{
    C3DPositionInterpolator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DPositionInterpolator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DPositionInterpolator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DPositionInterpolator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004c20f0_TEST PASS\n");
    return 0;
}