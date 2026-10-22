#include <cstdio>

struct CDraggable {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CDraggable* g_last_dtor = 0;

void __fastcall CDraggable_dtor(CDraggable* draggable)
{
    g_last_dtor = draggable;
    draggable->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CDraggable_scalar_deleting_destructor(CDraggable* draggable, int edx_unused, unsigned int flags)
{
    CDraggable_dtor(draggable);
    if (flags & 1)
        operator delete(draggable);
    return draggable;
}

int main()
{
    CDraggable a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CDraggable_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CDraggable b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CDraggable_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}