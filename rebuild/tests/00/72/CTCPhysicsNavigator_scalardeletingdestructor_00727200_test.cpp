#include <cstdio>

struct CTCPhysicsNavigator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCPhysicsNavigator* g_last_dtor = 0;

void __fastcall CTCPhysicsNavigator_dtor(CTCPhysicsNavigator* navigator)
{
    g_last_dtor = navigator;
    navigator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCPhysicsNavigator_scalar_deleting_destructor(CTCPhysicsNavigator* navigator, int edx_unused, unsigned int flags)
{
    CTCPhysicsNavigator_dtor(navigator);
    if (flags & 1)
        operator delete(navigator);
    return navigator;
}

int main()
{
    CTCPhysicsNavigator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCPhysicsNavigator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCPhysicsNavigator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCPhysicsNavigator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}