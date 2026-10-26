#include <cstdio>

class CTCDrunkenness { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDrunkenness* g_last_dtor = 0;

void __fastcall CTCDrunkenness_dtor(CTCDrunkenness* drunkenness)
{
    g_last_dtor = drunkenness;
    drunkenness->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDrunkenness_scalar_deleting_destructor(CTCDrunkenness* drunkenness, int edx_unused, unsigned int flags)
{
    CTCDrunkenness_dtor(drunkenness);
    if (flags & 1)
        operator delete(drunkenness);
    return drunkenness;
}

int main()
{
    CTCDrunkenness a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDrunkenness_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDrunkenness b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDrunkenness_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007bba30_TEST PASS\n");
    return 0;
}