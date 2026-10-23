#include <cstdio>

struct CActionWait {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CActionWait* g_last_dtor = 0;

void __fastcall CActionWait_dtor(CActionWait* action)
{
    g_last_dtor = action;
    action->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CActionWait_scalar_deleting_destructor(CActionWait* action, int edx_unused, unsigned int flags)
{
    CActionWait_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}

int main()
{
    CActionWait a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CActionWait_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CActionWait b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CActionWait_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_009050d0_TEST PASS\n");
    return 0;
}