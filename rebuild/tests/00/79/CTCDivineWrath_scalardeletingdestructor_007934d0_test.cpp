#include <cstdio>

struct CTCDivineWrath {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDivineWrath* g_last_dtor = 0;

void __fastcall CTCDivineWrath_dtor(CTCDivineWrath* wrath)
{
    g_last_dtor = wrath;
    wrath->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDivineWrath_scalar_deleting_destructor(CTCDivineWrath* wrath, int edx_unused, unsigned int flags)
{
    CTCDivineWrath_dtor(wrath);
    if (flags & 1)
        operator delete(wrath);
    return wrath;
}

int main()
{
    CTCDivineWrath a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDivineWrath_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDivineWrath b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDivineWrath_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007934d0_TEST PASS\n");
    return 0;
}