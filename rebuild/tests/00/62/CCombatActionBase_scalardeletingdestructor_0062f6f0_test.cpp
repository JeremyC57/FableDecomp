#include <cstdio>

struct CCombatActionBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatActionBase* g_last_dtor = 0;

void __fastcall CCombatActionBase_dtor(CCombatActionBase* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatActionBase_scalar_deleting_destructor(CCombatActionBase* action, int edx_unused, unsigned int flags)
{
    CCombatActionBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCombatActionBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatActionBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatActionBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatActionBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0062f6f0_TEST PASS\n");
    return 0;
}