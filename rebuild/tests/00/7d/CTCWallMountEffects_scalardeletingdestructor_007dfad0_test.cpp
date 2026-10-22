#include <cstdio>

struct CTCWallMountEffects {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWallMountEffects* g_last_dtor = 0;

void __fastcall CTCWallMountEffects_dtor(CTCWallMountEffects* effects)
{
    g_last_dtor = effects;
    effects->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWallMountEffects_scalar_deleting_destructor(CTCWallMountEffects* effects, int edx_unused, unsigned int flags)
{
    CTCWallMountEffects_dtor(effects);
    if (flags & 1)
        operator delete(effects);
    return effects;
}

int main()
{
    CTCWallMountEffects a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWallMountEffects_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWallMountEffects b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWallMountEffects_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}