#include <cstdio>

struct CTCStealth {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCStealth* g_last_dtor = 0;

void __fastcall CTCStealth_dtor(CTCStealth* stealth)
{
    g_last_dtor = stealth;
    stealth->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCStealth_scalar_deleting_destructor(CTCStealth* stealth, int edx_unused, unsigned int flags)
{
    CTCStealth_dtor(stealth);
    if (flags & 1)
        operator delete(stealth);
    return stealth;
}

int main()
{
    CTCStealth a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCStealth_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCStealth b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCStealth_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007495b0_TEST PASS\n");
    return 0;
}