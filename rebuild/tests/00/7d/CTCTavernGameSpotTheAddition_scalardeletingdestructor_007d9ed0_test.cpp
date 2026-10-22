#include <cstdio>

struct CTCTavernGameSpotTheAddition {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernGameSpotTheAddition* g_last_dtor = 0;

void __fastcall CTCTavernGameSpotTheAddition_dtor(CTCTavernGameSpotTheAddition* addition)
{
    g_last_dtor = addition;
    addition->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernGameSpotTheAddition_scalar_deleting_destructor(CTCTavernGameSpotTheAddition* addition, int edx_unused, unsigned int flags)
{
    CTCTavernGameSpotTheAddition_dtor(addition);
    if (flags & 1)
        operator delete(addition);
    return addition;
}

int main()
{
    CTCTavernGameSpotTheAddition a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernGameSpotTheAddition_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernGameSpotTheAddition b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernGameSpotTheAddition_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}