#include <cstdio>

class CTCHitOnCollision { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHitOnCollision* g_last_dtor = 0;

void __fastcall CTCHitOnCollision_dtor(CTCHitOnCollision* collision)
{
    g_last_dtor = collision;
    collision->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHitOnCollision_scalar_deleting_destructor(CTCHitOnCollision* collision, int edx_unused, unsigned int flags)
{
    CTCHitOnCollision_dtor(collision);
    if (flags & 1)
        operator delete(collision);
    return collision;
}

int main()
{
    CTCHitOnCollision a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHitOnCollision_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHitOnCollision b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHitOnCollision_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007c8540_TEST PASS\n");
    return 0;
}