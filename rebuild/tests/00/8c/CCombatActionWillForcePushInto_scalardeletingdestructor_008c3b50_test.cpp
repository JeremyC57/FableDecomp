#include <cstdio>

struct CCombatAction_WillForcePushInto {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatAction_WillForcePushInto* g_last_dtor = 0;

void __fastcall CCombatAction_WillForcePushInto_dtor(CCombatAction_WillForcePushInto* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatAction_WillForcePushInto_scalar_deleting_destructor(CCombatAction_WillForcePushInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillForcePushInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCombatAction_WillForcePushInto a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatAction_WillForcePushInto_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatAction_WillForcePushInto b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatAction_WillForcePushInto_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008c3b50_TEST PASS\n");
    return 0;
}