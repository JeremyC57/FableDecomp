#include <cstdio>

struct CTCTavernGameShoveHaPenny {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameShoveHaPenny* g_last_dtor = 0;

void __fastcall CTCTavernGameShoveHaPenny_dtor(CTCTavernGameShoveHaPenny* penny)
{
    g_last_dtor = penny;
    penny->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameShoveHaPenny_scalar_deleting_destructor(CTCTavernGameShoveHaPenny* penny, int edx_unused, unsigned int flags)
{
    CTCTavernGameShoveHaPenny_dtor(penny);
    if (flags & 1)
        operator delete(penny);
    return penny;
}

int main()
{
    CTCTavernGameShoveHaPenny a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameShoveHaPenny_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameShoveHaPenny b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameShoveHaPenny_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007d2240_TEST PASS\n");
    return 0;
}