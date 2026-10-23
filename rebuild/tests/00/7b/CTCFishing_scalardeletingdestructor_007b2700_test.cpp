#include <cstdio>

struct CTCFishing {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCFishing* g_last_dtor = 0;

void __fastcall CTCFishing_dtor(CTCFishing* fishing)
{
    g_last_dtor = fishing;
    fishing->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCFishing_scalar_deleting_destructor(CTCFishing* fishing, int edx_unused, unsigned int flags)
{
    CTCFishing_dtor(fishing);
    if (flags & 1)
        operator delete(fishing);
    return fishing;
}

int main()
{
    CTCFishing a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCFishing_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCFishing b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCFishing_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007b2700_TEST PASS\n");
    return 0;
}