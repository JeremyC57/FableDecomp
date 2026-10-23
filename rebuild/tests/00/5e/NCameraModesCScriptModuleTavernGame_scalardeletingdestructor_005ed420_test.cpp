#include <cstdio>

struct CScriptModuleTavernGame {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScriptModuleTavernGame* g_last_dtor = 0;

void __fastcall CScriptModuleTavernGame_dtor(CScriptModuleTavernGame* game)
{
    g_last_dtor = game;
    game->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScriptModuleTavernGame_scalar_deleting_destructor(CScriptModuleTavernGame* game, int edx_unused, unsigned int flags)
{
    CScriptModuleTavernGame_dtor(game);
    if (flags & 1)
        operator delete(game);
    return game;
}

int main()
{
    CScriptModuleTavernGame a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScriptModuleTavernGame_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScriptModuleTavernGame b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScriptModuleTavernGame_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_005ed420_TEST PASS\n");
    return 0;
}