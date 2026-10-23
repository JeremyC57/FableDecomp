#include <cstdio>

struct CHoverable {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CHoverable* g_last_dtor = 0;

void __fastcall CHoverable_dtor(CHoverable* hoverable)
{
    g_last_dtor = hoverable;
    hoverable->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CHoverable_scalar_deleting_destructor(CHoverable* hoverable, int edx_unused, unsigned int flags)
{
    CHoverable_dtor(hoverable);
    if (flags & 1)
        operator delete(hoverable);
    return hoverable;
}

int main()
{
    CHoverable a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CHoverable_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CHoverable b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CHoverable_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0055c4a0_TEST PASS\n");
    return 0;
}