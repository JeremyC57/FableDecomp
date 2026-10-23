#include <cstdio>

struct CTCCameraPointScriptedSpline {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCCameraPointScriptedSpline* g_last_dtor = 0;

void __fastcall CTCCameraPointScriptedSpline_dtor(CTCCameraPointScriptedSpline* spline)
{
    g_last_dtor = spline;
    spline->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCCameraPointScriptedSpline_scalar_deleting_destructor(CTCCameraPointScriptedSpline* spline, int edx_unused, unsigned int flags)
{
    CTCCameraPointScriptedSpline_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}

int main()
{
    CTCCameraPointScriptedSpline a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCCameraPointScriptedSpline_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCCameraPointScriptedSpline b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCCameraPointScriptedSpline_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0075fc20_TEST PASS\n");
    return 0;
}