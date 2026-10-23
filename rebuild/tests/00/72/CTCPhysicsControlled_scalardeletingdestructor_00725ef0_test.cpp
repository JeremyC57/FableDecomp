#include <cstdio>

struct CTCPhysicsControlled {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCPhysicsControlled* g_last_dtor = 0;

void __fastcall CTCPhysicsControlled_dtor(CTCPhysicsControlled* controlled)
{
    g_last_dtor = controlled;
    controlled->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCPhysicsControlled_scalar_deleting_destructor(CTCPhysicsControlled* controlled, int edx_unused, unsigned int flags)
{
    CTCPhysicsControlled_dtor(controlled);
    if (flags & 1)
        operator delete(controlled);
    return controlled;
}

int main()
{
    CTCPhysicsControlled a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCPhysicsControlled_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCPhysicsControlled b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCPhysicsControlled_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00725ef0_TEST PASS\n");
    return 0;
}