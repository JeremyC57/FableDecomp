#include <cstdio>

struct CAIBrain {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CAIBrain* g_last_dtor = 0;

void __fastcall CAIBrain_dtor(CAIBrain* brain)
{
    g_last_dtor = brain;
    brain->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CAIBrain_scalar_deleting_destructor(CAIBrain* brain, int edx_unused, unsigned int flags)
{
    CAIBrain_dtor(brain);
    if (flags & 1)
        operator delete(brain);
    return brain;
}

int main()
{
    CAIBrain a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CAIBrain_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CAIBrain b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CAIBrain_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}