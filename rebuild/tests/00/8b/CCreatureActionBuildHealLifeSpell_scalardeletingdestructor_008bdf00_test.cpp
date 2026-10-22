#include <cstdio>

struct CCreatureAction_BuildHealLifeSpell {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_BuildHealLifeSpell* g_last_dtor = 0;

void __fastcall CCreatureAction_BuildHealLifeSpell_dtor(CCreatureAction_BuildHealLifeSpell* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_BuildHealLifeSpell_scalar_deleting_destructor(CCreatureAction_BuildHealLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildHealLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_BuildHealLifeSpell a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_BuildHealLifeSpell_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_BuildHealLifeSpell b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_BuildHealLifeSpell_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}