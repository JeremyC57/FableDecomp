#include <cstdio>

class CScriptedMapBrush { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScriptedMapBrush* g_last_dtor = 0;

void __fastcall CScriptedMapBrush_dtor(CScriptedMapBrush* brush)
{
    g_last_dtor = brush;
    brush->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScriptedMapBrush_scalar_deleting_destructor(CScriptedMapBrush* brush, int edx_unused, unsigned int flags)
{
    CScriptedMapBrush_dtor(brush);
    if (flags & 1)
        operator delete(brush);
    return brush;
}

int main()
{
    CScriptedMapBrush a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScriptedMapBrush_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScriptedMapBrush b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScriptedMapBrush_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008270c0_TEST PASS\n");
    return 0;
}