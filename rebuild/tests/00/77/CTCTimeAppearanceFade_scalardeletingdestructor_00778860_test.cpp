#include <cstdio>

struct CTCTimeAppearanceFade {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTimeAppearanceFade* g_last_dtor = 0;

void __fastcall CTCTimeAppearanceFade_dtor(CTCTimeAppearanceFade* fade)
{
    g_last_dtor = fade;
    fade->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTimeAppearanceFade_scalar_deleting_destructor(CTCTimeAppearanceFade* fade, int edx_unused, unsigned int flags)
{
    CTCTimeAppearanceFade_dtor(fade);
    if (flags & 1)
        operator delete(fade);
    return fade;
}

int main()
{
    CTCTimeAppearanceFade a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTimeAppearanceFade_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTimeAppearanceFade b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTimeAppearanceFade_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}