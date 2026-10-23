#include <cstdio>

struct CTCOverheadDisplay {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCOverheadDisplay* g_last_dtor = 0;

void __fastcall CTCOverheadDisplay_dtor(CTCOverheadDisplay* display)
{
    g_last_dtor = display;
    display->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCOverheadDisplay_scalar_deleting_destructor(CTCOverheadDisplay* display, int edx_unused, unsigned int flags)
{
    CTCOverheadDisplay_dtor(display);
    if (flags & 1)
        operator delete(display);
    return display;
}

int main()
{
    CTCOverheadDisplay a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCOverheadDisplay_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCOverheadDisplay b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCOverheadDisplay_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007b8090_TEST PASS\n");
    return 0;
}