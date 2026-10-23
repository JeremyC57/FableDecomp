#include <cstdio>

struct CTCDiggingSpot {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDiggingSpot* g_last_dtor = 0;

void __fastcall CTCDiggingSpot_dtor(CTCDiggingSpot* spot)
{
    g_last_dtor = spot;
    spot->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDiggingSpot_scalar_deleting_destructor(CTCDiggingSpot* spot, int edx_unused, unsigned int flags)
{
    CTCDiggingSpot_dtor(spot);
    if (flags & 1)
        operator delete(spot);
    return spot;
}

int main()
{
    CTCDiggingSpot a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDiggingSpot_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDiggingSpot b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDiggingSpot_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007e25c0_TEST PASS\n");
    return 0;
}