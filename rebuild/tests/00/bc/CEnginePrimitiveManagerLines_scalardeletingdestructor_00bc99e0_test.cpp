#include <cstdio>

struct CEnginePrimitiveManagerLines {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerLines* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerLines_dtor(CEnginePrimitiveManagerLines* lines)
{
    g_last_dtor = lines;
    lines->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerLines_scalar_deleting_destructor(CEnginePrimitiveManagerLines* lines, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerLines_dtor(lines);
    if (flags & 1)
        operator delete(lines);
    return lines;
}

int main()
{
    CEnginePrimitiveManagerLines a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerLines_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerLines b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerLines_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bc99e0_TEST PASS\n");
    return 0;
}