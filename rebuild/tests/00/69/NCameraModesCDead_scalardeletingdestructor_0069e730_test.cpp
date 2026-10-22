#include <cstdio>

struct CDead {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CDead* g_last_dtor = 0;

void __fastcall CDead_dtor(CDead* dead)
{
    g_last_dtor = dead;
    dead->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CDead_scalar_deleting_destructor(CDead* dead, int edx_unused, unsigned int flags)
{
    CDead_dtor(dead);
    if (flags & 1)
        operator delete(dead);
    return dead;
}

int main()
{
    CDead a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CDead_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CDead b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CDead_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}