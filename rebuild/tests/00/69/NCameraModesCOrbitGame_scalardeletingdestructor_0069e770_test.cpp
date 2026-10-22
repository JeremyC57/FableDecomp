#include <cstdio>

struct COrbitGame {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static COrbitGame* g_last_dtor = 0;

void __fastcall COrbitGame_dtor(COrbitGame* game)
{
    g_last_dtor = game;
    game->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall COrbitGame_scalar_deleting_destructor(COrbitGame* game, int edx_unused, unsigned int flags)
{
    COrbitGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}

int main()
{
    COrbitGame a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = COrbitGame_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    COrbitGame b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = COrbitGame_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}