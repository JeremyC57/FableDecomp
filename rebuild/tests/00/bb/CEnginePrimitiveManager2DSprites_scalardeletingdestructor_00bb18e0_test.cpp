#include <cstdio>

class CEnginePrimitiveManager2DSprites { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManager2DSprites* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManager2DSprites_dtor(CEnginePrimitiveManager2DSprites* sprites)
{
    g_last_dtor = sprites;
    sprites->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManager2DSprites_scalar_deleting_destructor(CEnginePrimitiveManager2DSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManager2DSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}

int main()
{
    CEnginePrimitiveManager2DSprites a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManager2DSprites_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManager2DSprites b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManager2DSprites_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bb18e0_TEST PASS\n");
    return 0;
}