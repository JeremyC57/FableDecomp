#include <cstdio>

struct CPSCRenderSprite {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCRenderSprite* g_last_dtor = 0;

void __fastcall CPSCRenderSprite_dtor(CPSCRenderSprite* sprite)
{
    g_last_dtor = sprite;
    sprite->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCRenderSprite_scalar_deleting_destructor(CPSCRenderSprite* sprite, int edx_unused, unsigned int flags)
{
    CPSCRenderSprite_dtor(sprite);
    if (flags & 1)
        operator delete(sprite);
    return sprite;
}

int main()
{
    CPSCRenderSprite a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCRenderSprite_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCRenderSprite b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCRenderSprite_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}