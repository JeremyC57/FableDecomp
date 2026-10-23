#include <cstdio>

struct CCreatureAction_CallSpouseHome {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_CallSpouseHome* g_last_dtor = 0;

void __fastcall CCreatureAction_CallSpouseHome_dtor(CCreatureAction_CallSpouseHome* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_CallSpouseHome_scalar_deleting_destructor(CCreatureAction_CallSpouseHome* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CallSpouseHome_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_CallSpouseHome a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_CallSpouseHome_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_CallSpouseHome b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_CallSpouseHome_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00933d30_TEST PASS\n");
    return 0;
}