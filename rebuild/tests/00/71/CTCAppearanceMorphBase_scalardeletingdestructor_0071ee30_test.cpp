#include <cstdio>

struct CTCAppearanceMorphBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCAppearanceMorphBase* g_last_dtor = 0;

void __fastcall CTCAppearanceMorphBase_dtor(CTCAppearanceMorphBase* morph)
{
    g_last_dtor = morph;
    morph->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCAppearanceMorphBase_scalar_deleting_destructor(CTCAppearanceMorphBase* morph, int edx_unused, unsigned int flags)
{
    CTCAppearanceMorphBase_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}

int main()
{
    CTCAppearanceMorphBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCAppearanceMorphBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCAppearanceMorphBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCAppearanceMorphBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0071ee30_TEST PASS\n");
    return 0;
}