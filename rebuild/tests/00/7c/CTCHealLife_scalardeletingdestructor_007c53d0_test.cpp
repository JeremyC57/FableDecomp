#include <cstdio>

struct CTCHealLife {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHealLife* g_last_dtor = 0;

void __fastcall CTCHealLife_dtor(CTCHealLife* life)
{
    g_last_dtor = life;
    life->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHealLife_scalar_deleting_destructor(CTCHealLife* life, int edx_unused, unsigned int flags)
{
    CTCHealLife_dtor(life);
    if (flags & 1)
        operator delete(life);
    return life;
}

int main()
{
    CTCHealLife a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHealLife_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHealLife b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHealLife_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}