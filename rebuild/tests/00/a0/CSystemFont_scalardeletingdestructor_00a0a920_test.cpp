#include <cstdio>

struct CSystemFont {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSystemFont* g_last_dtor = 0;

void __fastcall CSystemFont_dtor(CSystemFont* font)
{
    g_last_dtor = font;
    font->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSystemFont_scalar_deleting_destructor(CSystemFont* font, int edx_unused, unsigned int flags)
{
    CSystemFont_dtor(font);
    if (flags & 1)
        operator delete(font);
    return font;
}

int main()
{
    CSystemFont a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSystemFont_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSystemFont b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSystemFont_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a0a920_TEST PASS\n");
    return 0;
}