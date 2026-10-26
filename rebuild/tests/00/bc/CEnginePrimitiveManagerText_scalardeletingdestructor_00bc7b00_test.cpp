#include <cstdio>

class CEnginePrimitiveManagerText { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerText* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerText_dtor(CEnginePrimitiveManagerText* text)
{
    g_last_dtor = text;
    text->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerText_scalar_deleting_destructor(CEnginePrimitiveManagerText* text, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerText_dtor(text);
    if (flags & 1)
        operator delete(text);
    return text;
}

int main()
{
    CEnginePrimitiveManagerText a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerText_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerText b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerText_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bc7b00_TEST PASS\n");
    return 0;
}