#include <cstdio>

struct CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* g_last_dtor = 0;

void __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_dtor(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_scalar_deleting_destructor(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}