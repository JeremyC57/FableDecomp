#include <cstdio>

struct CTCCreditsUI {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCCreditsUI* g_last_dtor = 0;

void __fastcall CTCCreditsUI_dtor(CTCCreditsUI* creditsUI)
{
    g_last_dtor = creditsUI;
    creditsUI->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCCreditsUI_scalar_deleting_destructor(CTCCreditsUI* creditsUI, int edx_unused, unsigned int flags)
{
    CTCCreditsUI_dtor(creditsUI);
    if (flags & 1)
        operator delete(creditsUI);
    return creditsUI;
}

int main()
{
    CTCCreditsUI a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCCreditsUI_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCCreditsUI b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCCreditsUI_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007e4c50_TEST PASS\n");
    return 0;
}