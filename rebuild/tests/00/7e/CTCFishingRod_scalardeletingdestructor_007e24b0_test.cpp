#include <cstdio>

struct CTCFishingRod {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCFishingRod* g_last_dtor = 0;

void __fastcall CTCFishingRod_dtor(CTCFishingRod* rod)
{
    g_last_dtor = rod;
    rod->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCFishingRod_scalar_deleting_destructor(CTCFishingRod* rod, int edx_unused, unsigned int flags)
{
    CTCFishingRod_dtor(rod);
    if (flags & 1)
        operator delete(rod);
    return rod;
}

int main()
{
    CTCFishingRod a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCFishingRod_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCFishingRod b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCFishingRod_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}