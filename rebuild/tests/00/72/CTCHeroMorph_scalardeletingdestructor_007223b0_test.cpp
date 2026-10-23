#include <cstdio>

struct CTCHeroMorph {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroMorph* g_last_dtor = 0;

void __fastcall CTCHeroMorph_dtor(CTCHeroMorph* morph)
{
    g_last_dtor = morph;
    morph->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroMorph_scalar_deleting_destructor(CTCHeroMorph* morph, int edx_unused, unsigned int flags)
{
    CTCHeroMorph_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}

int main()
{
    CTCHeroMorph a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroMorph_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroMorph b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroMorph_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007223b0_TEST PASS\n");
    return 0;
}