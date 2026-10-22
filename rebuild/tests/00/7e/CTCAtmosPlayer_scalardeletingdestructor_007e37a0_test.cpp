#include <cstdio>

struct CTCAtmosPlayer {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCAtmosPlayer* g_last_dtor = 0;

void __fastcall CTCAtmosPlayer_dtor(CTCAtmosPlayer* player)
{
    g_last_dtor = player;
    player->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCAtmosPlayer_scalar_deleting_destructor(CTCAtmosPlayer* player, int edx_unused, unsigned int flags)
{
    CTCAtmosPlayer_dtor(player);
    if (flags & 1)
        operator delete(player);
    return player;
}

int main()
{
    CTCAtmosPlayer a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCAtmosPlayer_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCAtmosPlayer b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCAtmosPlayer_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}