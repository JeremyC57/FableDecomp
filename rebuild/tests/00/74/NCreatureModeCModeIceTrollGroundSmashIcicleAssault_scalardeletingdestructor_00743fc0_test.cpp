#include <cstdio>

struct CModeIceTrollGroundSmashIcicleAssault {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeIceTrollGroundSmashIcicleAssault* g_last_dtor = 0;

void __fastcall CModeIceTrollGroundSmashIcicleAssault_dtor(CModeIceTrollGroundSmashIcicleAssault* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeIceTrollGroundSmashIcicleAssault_scalar_deleting_destructor(CModeIceTrollGroundSmashIcicleAssault* mode, int edx_unused, unsigned int flags)
{
    CModeIceTrollGroundSmashIcicleAssault_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeIceTrollGroundSmashIcicleAssault a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeIceTrollGroundSmashIcicleAssault_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeIceTrollGroundSmashIcicleAssault b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeIceTrollGroundSmashIcicleAssault_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00743fc0_TEST PASS\n");
    return 0;
}