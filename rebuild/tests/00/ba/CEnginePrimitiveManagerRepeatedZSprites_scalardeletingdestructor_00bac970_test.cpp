#include <cstdio>

struct CEnginePrimitiveManagerRepeatedZSprites {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerRepeatedZSprites* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerRepeatedZSprites_dtor(CEnginePrimitiveManagerRepeatedZSprites* sprites)
{
    g_last_dtor = sprites;
    sprites->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerRepeatedZSprites_scalar_deleting_destructor(CEnginePrimitiveManagerRepeatedZSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerRepeatedZSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}

int main()
{
    CEnginePrimitiveManagerRepeatedZSprites a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerRepeatedZSprites_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerRepeatedZSprites b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerRepeatedZSprites_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}