#include <cstdio>

struct CRotateAround {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CRotateAround* g_last_dtor = 0;

void __fastcall CRotateAround_dtor(CRotateAround* around)
{
    g_last_dtor = around;
    around->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CRotateAround_scalar_deleting_destructor(CRotateAround* around, int edx_unused, unsigned int flags)
{
    CRotateAround_dtor(around);
    if (flags & 1)
        operator delete(around);
    return around;
}

int main()
{
    CRotateAround a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CRotateAround_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CRotateAround b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CRotateAround_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0069e750_TEST PASS\n");
    return 0;
}