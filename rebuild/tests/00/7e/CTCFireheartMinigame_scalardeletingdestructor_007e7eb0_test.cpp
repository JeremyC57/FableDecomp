#include <cstdio>

class CTCFireheartMinigame { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCFireheartMinigame* g_last_dtor = 0;

void __fastcall CTCFireheartMinigame_dtor(CTCFireheartMinigame* minigame)
{
    g_last_dtor = minigame;
    minigame->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCFireheartMinigame_scalar_deleting_destructor(CTCFireheartMinigame* minigame, int edx_unused, unsigned int flags)
{
    CTCFireheartMinigame_dtor(minigame);
    if (flags & 1)
        operator delete(minigame);
    return minigame;
}

int main()
{
    CTCFireheartMinigame a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCFireheartMinigame_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCFireheartMinigame b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCFireheartMinigame_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007e7eb0_TEST PASS\n");
    return 0;
}