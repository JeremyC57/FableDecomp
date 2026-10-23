#include <cstdio>

struct CPSCSingleSprite {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCSingleSprite* g_last_dtor = 0;

void __fastcall CPSCSingleSprite_dtor(CPSCSingleSprite* sprite)
{
    g_last_dtor = sprite;
    sprite->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCSingleSprite_scalar_deleting_destructor(CPSCSingleSprite* sprite, int edx_unused, unsigned int flags)
{
    CPSCSingleSprite_dtor(sprite);
    if (flags & 1)
        operator delete(sprite);
    return sprite;
}

int main()
{
    CPSCSingleSprite a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCSingleSprite_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCSingleSprite b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCSingleSprite_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b0a140_TEST PASS\n");
    return 0;
}