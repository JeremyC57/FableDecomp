#include <cstdio>

class CEnginePrimitiveManagerSprites { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerSprites* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerSprites_dtor(CEnginePrimitiveManagerSprites* sprites)
{
    g_last_dtor = sprites;
    sprites->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerSprites_scalar_deleting_destructor(CEnginePrimitiveManagerSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}

int main()
{
    CEnginePrimitiveManagerSprites a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerSprites_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerSprites b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerSprites_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bb18b0_TEST PASS\n");
    return 0;
}