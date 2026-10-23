#include <cstdio>

struct CTCTavernGameCardBlackjack {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameCardBlackjack* g_last_dtor = 0;

void __fastcall CTCTavernGameCardBlackjack_dtor(CTCTavernGameCardBlackjack* blackjack)
{
    g_last_dtor = blackjack;
    blackjack->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameCardBlackjack_scalar_deleting_destructor(CTCTavernGameCardBlackjack* blackjack, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardBlackjack_dtor(blackjack);
    if (flags & 1)
        operator delete(blackjack);
    return blackjack;
}

int main()
{
    CTCTavernGameCardBlackjack a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameCardBlackjack_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameCardBlackjack b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameCardBlackjack_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007dca70_TEST PASS\n");
    return 0;
}