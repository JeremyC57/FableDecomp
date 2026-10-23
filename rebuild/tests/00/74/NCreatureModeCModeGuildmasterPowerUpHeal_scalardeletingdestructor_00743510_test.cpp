#include <cstdio>

struct CModeGuildmasterPowerUpHeal {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeGuildmasterPowerUpHeal* g_last_dtor = 0;

void __fastcall CModeGuildmasterPowerUpHeal_dtor(CModeGuildmasterPowerUpHeal* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeGuildmasterPowerUpHeal_scalar_deleting_destructor(CModeGuildmasterPowerUpHeal* mode, int edx_unused, unsigned int flags)
{
    CModeGuildmasterPowerUpHeal_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeGuildmasterPowerUpHeal a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeGuildmasterPowerUpHeal_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeGuildmasterPowerUpHeal b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeGuildmasterPowerUpHeal_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00743510_TEST PASS\n");
    return 0;
}