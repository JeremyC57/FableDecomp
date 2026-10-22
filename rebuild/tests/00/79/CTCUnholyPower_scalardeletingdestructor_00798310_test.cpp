#include <cstdio>

struct CTCUnholyPower {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCUnholyPower* g_last_dtor = 0;

void __fastcall CTCUnholyPower_dtor(CTCUnholyPower* power)
{
    g_last_dtor = power;
    power->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCUnholyPower_scalar_deleting_destructor(CTCUnholyPower* power, int edx_unused, unsigned int flags)
{
    CTCUnholyPower_dtor(power);
    if (flags & 1)
        operator delete(power);
    return power;
}

int main()
{
    CTCUnholyPower a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCUnholyPower_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCUnholyPower b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCUnholyPower_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}