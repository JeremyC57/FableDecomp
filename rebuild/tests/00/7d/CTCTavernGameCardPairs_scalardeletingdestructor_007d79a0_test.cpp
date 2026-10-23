#include <cstdio>

struct CTCTavernGameCardPairs {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameCardPairs* g_last_dtor = 0;

void __fastcall CTCTavernGameCardPairs_dtor(CTCTavernGameCardPairs* pairs)
{
    g_last_dtor = pairs;
    pairs->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameCardPairs_scalar_deleting_destructor(CTCTavernGameCardPairs* pairs, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardPairs_dtor(pairs);
    if (flags & 1)
        operator delete(pairs);
    return pairs;
}

int main()
{
    CTCTavernGameCardPairs a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameCardPairs_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameCardPairs b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameCardPairs_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007d79a0_TEST PASS\n");
    return 0;
}