#include <cstdio>

class CTCClock { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCClock* g_last_dtor = 0;

void __fastcall CTCClock_dtor(CTCClock* clock)
{
    g_last_dtor = clock;
    clock->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCClock_scalar_deleting_destructor(CTCClock* clock, int edx_unused, unsigned int flags)
{
    CTCClock_dtor(clock);
    if (flags & 1)
        operator delete(clock);
    return clock;
}

int main()
{
    CTCClock a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCClock_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCClock b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCClock_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00732c90_TEST PASS\n");
    return 0;
}