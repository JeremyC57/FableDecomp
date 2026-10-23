#include <cstdio>

struct CPSCSplineBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCSplineBase* g_last_dtor = 0;

void __fastcall CPSCSplineBase_dtor(CPSCSplineBase* spline)
{
    g_last_dtor = spline;
    spline->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCSplineBase_scalar_deleting_destructor(CPSCSplineBase* spline, int edx_unused, unsigned int flags)
{
    CPSCSplineBase_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}

int main()
{
    CPSCSplineBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCSplineBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCSplineBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCSplineBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b230c0_TEST PASS\n");
    return 0;
}