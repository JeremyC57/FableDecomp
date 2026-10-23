#include <cstdio>

struct CTCTrophy {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTrophy* g_last_dtor = 0;

void __fastcall CTCTrophy_dtor(CTCTrophy* trophy)
{
    g_last_dtor = trophy;
    trophy->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTrophy_scalar_deleting_destructor(CTCTrophy* trophy, int edx_unused, unsigned int flags)
{
    CTCTrophy_dtor(trophy);
    if (flags & 1)
        operator delete(trophy);
    return trophy;
}

int main()
{
    CTCTrophy a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTrophy_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTrophy b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTrophy_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006dfb40_TEST PASS\n");
    return 0;
}