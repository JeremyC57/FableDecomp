#include <cstdio>

struct CModeScreamerSuperAttack {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeScreamerSuperAttack* g_last_dtor = 0;

void __fastcall CModeScreamerSuperAttack_dtor(CModeScreamerSuperAttack* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeScreamerSuperAttack_scalar_deleting_destructor(CModeScreamerSuperAttack* mode, int edx_unused, unsigned int flags)
{
    CModeScreamerSuperAttack_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeScreamerSuperAttack a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeScreamerSuperAttack_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeScreamerSuperAttack b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeScreamerSuperAttack_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}