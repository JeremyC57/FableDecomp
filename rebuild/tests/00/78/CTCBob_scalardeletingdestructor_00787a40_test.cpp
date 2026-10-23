#include <cstdio>

struct CTCBob {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBob* g_last_dtor = 0;

void __fastcall CTCBob_dtor(CTCBob* bob)
{
    g_last_dtor = bob;
    bob->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBob_scalar_deleting_destructor(CTCBob* bob, int edx_unused, unsigned int flags)
{
    CTCBob_dtor(bob);
    if (flags & 1)
        operator delete(bob);
    return bob;
}

int main()
{
    CTCBob a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBob_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBob b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBob_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00787a40_TEST PASS\n");
    return 0;
}