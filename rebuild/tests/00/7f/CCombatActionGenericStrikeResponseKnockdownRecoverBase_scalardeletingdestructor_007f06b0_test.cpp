#include <cstdio>

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatAction_GenericStrikeResponseKnockdownRecoverBase* g_last_dtor = 0;

void __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase_dtor(CCombatAction_GenericStrikeResponseKnockdownRecoverBase* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase_scalar_deleting_destructor(CCombatAction_GenericStrikeResponseKnockdownRecoverBase* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatAction_GenericStrikeResponseKnockdownRecoverBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatAction_GenericStrikeResponseKnockdownRecoverBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatAction_GenericStrikeResponseKnockdownRecoverBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}