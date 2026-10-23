#include <cstdio>

struct CCombatAction_GenericBreakOffDrain {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatAction_GenericBreakOffDrain* g_last_dtor = 0;

void __fastcall CCombatAction_GenericBreakOffDrain_dtor(CCombatAction_GenericBreakOffDrain* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatAction_GenericBreakOffDrain_scalar_deleting_destructor(CCombatAction_GenericBreakOffDrain* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericBreakOffDrain_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCombatAction_GenericBreakOffDrain a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatAction_GenericBreakOffDrain_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatAction_GenericBreakOffDrain b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatAction_GenericBreakOffDrain_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00690e10_TEST PASS\n");
    return 0;
}