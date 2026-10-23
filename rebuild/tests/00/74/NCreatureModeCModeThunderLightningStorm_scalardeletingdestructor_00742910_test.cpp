#include <cstdio>

struct CModeThunderLightningStorm {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeThunderLightningStorm* g_last_dtor = 0;

void __fastcall CModeThunderLightningStorm_dtor(CModeThunderLightningStorm* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeThunderLightningStorm_scalar_deleting_destructor(CModeThunderLightningStorm* mode, int edx_unused, unsigned int flags)
{
    CModeThunderLightningStorm_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeThunderLightningStorm a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeThunderLightningStorm_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeThunderLightningStorm b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeThunderLightningStorm_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00742910_TEST PASS\n");
    return 0;
}