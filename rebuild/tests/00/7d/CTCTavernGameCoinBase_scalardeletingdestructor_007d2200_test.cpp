#include <cstdio>

struct CTCTavernGameCoinBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameCoinBase* g_last_dtor = 0;

void __fastcall CTCTavernGameCoinBase_dtor(CTCTavernGameCoinBase* coin)
{
    g_last_dtor = coin;
    coin->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameCoinBase_scalar_deleting_destructor(CTCTavernGameCoinBase* coin, int edx_unused, unsigned int flags)
{
    CTCTavernGameCoinBase_dtor(coin);
    if (flags & 1)
        operator delete(coin);
    return coin;
}

int main()
{
    CTCTavernGameCoinBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameCoinBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameCoinBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameCoinBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007d2200_TEST PASS\n");
    return 0;
}