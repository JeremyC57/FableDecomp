#include <cstdio>

struct C3DAnimationInterpolator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DAnimationInterpolator* g_last_dtor = 0;

void __fastcall C3DAnimationInterpolator_dtor(C3DAnimationInterpolator* interpolator)
{
    g_last_dtor = interpolator;
    interpolator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DAnimationInterpolator_scalar_deleting_destructor(C3DAnimationInterpolator* interpolator, int edx_unused, unsigned int flags)
{
    C3DAnimationInterpolator_dtor(interpolator);
    if (flags & 1)
        operator delete(interpolator);
    return interpolator;
}

int main()
{
    C3DAnimationInterpolator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DAnimationInterpolator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DAnimationInterpolator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DAnimationInterpolator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004c3d10_TEST PASS\n");
    return 0;
}