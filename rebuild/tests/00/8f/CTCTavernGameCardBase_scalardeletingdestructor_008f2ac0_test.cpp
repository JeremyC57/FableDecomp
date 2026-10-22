#include <cstdio>

struct CTCTavernGameCardBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameCardBase* g_last_dtor = 0;

void __fastcall CTCTavernGameCardBase_dtor(CTCTavernGameCardBase* card)
{
    g_last_dtor = card;
    card->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameCardBase_scalar_deleting_destructor(CTCTavernGameCardBase* card, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardBase_dtor(card);
    if (flags & 1)
        operator delete(card);
    return card;
}

int main()
{
    CTCTavernGameCardBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameCardBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameCardBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameCardBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}