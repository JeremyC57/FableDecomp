#include <cstdio>

class CCreatureAction_JackGetUp { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureAction_JackGetUp* g_last_dtor = 0;

void __fastcall CCreatureAction_JackGetUp_dtor(CCreatureAction_JackGetUp* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureAction_JackGetUp_scalar_deleting_destructor(CCreatureAction_JackGetUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackGetUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CCreatureAction_JackGetUp a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureAction_JackGetUp_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureAction_JackGetUp b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureAction_JackGetUp_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008c3af0_TEST PASS\n");
    return 0;
}