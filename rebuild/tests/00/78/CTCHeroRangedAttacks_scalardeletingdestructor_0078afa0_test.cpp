#include <cstdio>

struct CTCHeroRangedAttacks {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroRangedAttacks* g_last_dtor = 0;

void __fastcall CTCHeroRangedAttacks_dtor(CTCHeroRangedAttacks* attacks)
{
    g_last_dtor = attacks;
    attacks->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroRangedAttacks_scalar_deleting_destructor(CTCHeroRangedAttacks* attacks, int edx_unused, unsigned int flags)
{
    CTCHeroRangedAttacks_dtor(attacks);
    if (flags & 1)
        operator delete(attacks);
    return attacks;
}

int main()
{
    CTCHeroRangedAttacks a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroRangedAttacks_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroRangedAttacks b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroRangedAttacks_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0078afa0_TEST PASS\n");
    return 0;
}