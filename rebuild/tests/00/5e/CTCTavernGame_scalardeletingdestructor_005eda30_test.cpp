#include <cstdio>

struct CTCTavernGame {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGame* g_last_dtor = 0;

void __fastcall CTCTavernGame_dtor(CTCTavernGame* game)
{
    g_last_dtor = game;
    game->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGame_scalar_deleting_destructor(CTCTavernGame* game, int edx_unused, unsigned int flags)
{
    CTCTavernGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}

int main()
{
    CTCTavernGame a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGame_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGame b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGame_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}