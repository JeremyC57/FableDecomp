#include <cstdio>

struct CProgressDisplay {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CProgressDisplay* g_last_dtor = 0;

void __fastcall CProgressDisplay_dtor(CProgressDisplay* display)
{
    g_last_dtor = display;
    display->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CProgressDisplay_scalar_deleting_destructor(CProgressDisplay* display, int edx_unused, unsigned int flags)
{
    CProgressDisplay_dtor(display);
    if (flags & 1)
        operator delete(display);
    return display;
}

int main()
{
    CProgressDisplay a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CProgressDisplay_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CProgressDisplay b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CProgressDisplay_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0049b7d0_TEST PASS\n");
    return 0;
}