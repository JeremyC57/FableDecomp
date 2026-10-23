#include <cstdio>

struct CTCSoundPlayer {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCSoundPlayer* g_last_dtor = 0;

void __fastcall CTCSoundPlayer_dtor(CTCSoundPlayer* player)
{
    g_last_dtor = player;
    player->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCSoundPlayer_scalar_deleting_destructor(CTCSoundPlayer* player, int edx_unused, unsigned int flags)
{
    CTCSoundPlayer_dtor(player);
    if (flags & 1)
        operator delete(player);
    return player;
}

int main()
{
    CTCSoundPlayer a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCSoundPlayer_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCSoundPlayer b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCSoundPlayer_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0066c2d0_TEST PASS\n");
    return 0;
}