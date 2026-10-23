#include <cstdio>

struct CTCDegradableBarrier {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDegradableBarrier* g_last_dtor = 0;

void __fastcall CTCDegradableBarrier_dtor(CTCDegradableBarrier* barrier)
{
    g_last_dtor = barrier;
    barrier->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDegradableBarrier_scalar_deleting_destructor(CTCDegradableBarrier* barrier, int edx_unused, unsigned int flags)
{
    CTCDegradableBarrier_dtor(barrier);
    if (flags & 1)
        operator delete(barrier);
    return barrier;
}

int main()
{
    CTCDegradableBarrier a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDegradableBarrier_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDegradableBarrier b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDegradableBarrier_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00771120_TEST PASS\n");
    return 0;
}