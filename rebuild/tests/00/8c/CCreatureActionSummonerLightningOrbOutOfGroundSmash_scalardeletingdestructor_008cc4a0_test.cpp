#include <cstdio>

class CCreatureAction_SummonerLightningOrbOutOfGroundSmash { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_SummonerLightningOrbOutOfGroundSmash* g_last_dtor = 0;

void __fastcall CCreatureAction_SummonerLightningOrbOutOfGroundSmash_dtor(CCreatureAction_SummonerLightningOrbOutOfGroundSmash* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_SummonerLightningOrbOutOfGroundSmash_scalar_deleting_destructor(CCreatureAction_SummonerLightningOrbOutOfGroundSmash* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerLightningOrbOutOfGroundSmash_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_SummonerLightningOrbOutOfGroundSmash a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_SummonerLightningOrbOutOfGroundSmash_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_SummonerLightningOrbOutOfGroundSmash b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_SummonerLightningOrbOutOfGroundSmash_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008cc4a0_TEST PASS\n");
    return 0;
}