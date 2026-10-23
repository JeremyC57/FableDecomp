#include <cstdio>

struct CActionTalkBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CActionTalkBase* g_last_dtor = 0;

void __fastcall CActionTalkBase_dtor(CActionTalkBase* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CActionTalkBase_scalar_deleting_destructor(CActionTalkBase* action, int edx_unused, unsigned int flags)
{
    CActionTalkBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CActionTalkBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CActionTalkBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CActionTalkBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CActionTalkBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0071a530_TEST PASS\n");
    return 0;
}