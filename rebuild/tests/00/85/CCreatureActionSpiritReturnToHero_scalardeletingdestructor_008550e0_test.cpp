#include <cstdio>

struct CCreatureAction_SpiritReturnToHero {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_SpiritReturnToHero* g_last_dtor = 0;

void __fastcall CCreatureAction_SpiritReturnToHero_dtor(CCreatureAction_SpiritReturnToHero* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_SpiritReturnToHero_scalar_deleting_destructor(CCreatureAction_SpiritReturnToHero* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SpiritReturnToHero_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_SpiritReturnToHero a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_SpiritReturnToHero_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_SpiritReturnToHero b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_SpiritReturnToHero_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008550e0_TEST PASS\n");
    return 0;
}