#include <cstdio>

struct CTCCreatureOpinionOfHero {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCCreatureOpinionOfHero* g_last_dtor = 0;

void __fastcall CTCCreatureOpinionOfHero_dtor(CTCCreatureOpinionOfHero* hero)
{
    g_last_dtor = hero;
    hero->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCCreatureOpinionOfHero_scalar_deleting_destructor(CTCCreatureOpinionOfHero* hero, int edx_unused, unsigned int flags)
{
    CTCCreatureOpinionOfHero_dtor(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}

int main()
{
    CTCCreatureOpinionOfHero a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCCreatureOpinionOfHero_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCCreatureOpinionOfHero b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCCreatureOpinionOfHero_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006866d0_TEST PASS\n");
    return 0;
}