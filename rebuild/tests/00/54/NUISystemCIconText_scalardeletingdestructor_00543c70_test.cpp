#include <cstdio>

struct CIconText {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CIconText* g_last_dtor = 0;

void __fastcall CIconText_dtor(CIconText* text)
{
    g_last_dtor = text;
    text->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CIconText_scalar_deleting_destructor(CIconText* text, int edx_unused, unsigned int flags)
{
    CIconText_dtor(text);
    if (flags & 1)
        operator delete(text);
    return text;
}

int main()
{
    CIconText a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CIconText_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CIconText b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CIconText_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}