#include <cstdio>

struct CTCCreatureFlyInCircles {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCCreatureFlyInCircles* g_last_dtor = 0;

void __fastcall CTCCreatureFlyInCircles_dtor(CTCCreatureFlyInCircles* circles)
{
    g_last_dtor = circles;
    circles->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCCreatureFlyInCircles_scalar_deleting_destructor(CTCCreatureFlyInCircles* circles, int edx_unused, unsigned int flags)
{
    CTCCreatureFlyInCircles_dtor(circles);
    if (flags & 1)
        operator delete(circles);
    return circles;
}

int main()
{
    CTCCreatureFlyInCircles a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCCreatureFlyInCircles_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCCreatureFlyInCircles b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCCreatureFlyInCircles_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007de420_TEST PASS\n");
    return 0;
}