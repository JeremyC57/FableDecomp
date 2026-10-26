#include <cstdio>

class CEnginePrimitiveManagerDecal { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerDecal* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerDecal_dtor(CEnginePrimitiveManagerDecal* decal)
{
    g_last_dtor = decal;
    decal->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerDecal_scalar_deleting_destructor(CEnginePrimitiveManagerDecal* decal, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerDecal_dtor(decal);
    if (flags & 1)
        operator delete(decal);
    return decal;
}

int main()
{
    CEnginePrimitiveManagerDecal a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerDecal_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerDecal b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerDecal_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bbbba0_TEST PASS\n");
    return 0;
}