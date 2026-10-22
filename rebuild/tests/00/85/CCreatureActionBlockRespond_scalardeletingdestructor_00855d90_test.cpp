#include <cstdio>

struct CCreatureAction_BlockRespond {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_BlockRespond* g_last_dtor = 0;

void __fastcall CCreatureAction_BlockRespond_dtor(CCreatureAction_BlockRespond* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_BlockRespond_scalar_deleting_destructor(CCreatureAction_BlockRespond* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BlockRespond_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_BlockRespond a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_BlockRespond_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_BlockRespond b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_BlockRespond_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}