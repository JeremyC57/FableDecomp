#include <cstdio>

struct CPSCSpline {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCSpline* g_last_dtor = 0;

void __fastcall CPSCSpline_dtor(CPSCSpline* spline)
{
    g_last_dtor = spline;
    spline->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCSpline_scalar_deleting_destructor(CPSCSpline* spline, int edx_unused, unsigned int flags)
{
    CPSCSpline_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}

int main()
{
    CPSCSpline a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCSpline_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCSpline b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCSpline_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}