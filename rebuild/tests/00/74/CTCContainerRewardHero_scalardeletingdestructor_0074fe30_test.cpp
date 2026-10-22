#include <cstdio>

struct CTCContainerRewardHero {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCContainerRewardHero* g_last_dtor = 0;

void __fastcall CTCContainerRewardHero_dtor(CTCContainerRewardHero* hero)
{
    g_last_dtor = hero;
    hero->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCContainerRewardHero_scalar_deleting_destructor(CTCContainerRewardHero* hero, int edx_unused, unsigned int flags)
{
    CTCContainerRewardHero_dtor(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}

int main()
{
    CTCContainerRewardHero a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCContainerRewardHero_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCContainerRewardHero b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCContainerRewardHero_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}