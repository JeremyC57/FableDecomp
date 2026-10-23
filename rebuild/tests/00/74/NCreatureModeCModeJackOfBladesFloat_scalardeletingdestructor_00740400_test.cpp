#include <cstdio>

struct CModeJackOfBladesFloat {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeJackOfBladesFloat* g_last_dtor = 0;

void __fastcall CModeJackOfBladesFloat_dtor(CModeJackOfBladesFloat* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeJackOfBladesFloat_scalar_deleting_destructor(CModeJackOfBladesFloat* mode, int edx_unused, unsigned int flags)
{
    CModeJackOfBladesFloat_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeJackOfBladesFloat a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeJackOfBladesFloat_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeJackOfBladesFloat b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeJackOfBladesFloat_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00740400_TEST PASS\n");
    return 0;
}