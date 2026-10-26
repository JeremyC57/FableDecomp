#include <cstdio>

class CTCPhysicsBase { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCPhysicsBase* g_last_dtor = 0;

void __fastcall CTCPhysicsBase_dtor(CTCPhysicsBase* physics)
{
    g_last_dtor = physics;
    physics->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCPhysicsBase_scalar_deleting_destructor(CTCPhysicsBase* physics, int edx_unused, unsigned int flags)
{
    CTCPhysicsBase_dtor(physics);
    if (flags & 1)
        operator delete(physics);
    return physics;
}

int main()
{
    CTCPhysicsBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCPhysicsBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCPhysicsBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCPhysicsBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006b14d0_TEST PASS\n");
    return 0;
}