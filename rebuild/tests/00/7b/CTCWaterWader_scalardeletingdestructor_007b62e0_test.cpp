#include <cstdio>

struct CTCWaterWader {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWaterWader* g_last_dtor = 0;

void __fastcall CTCWaterWader_dtor(CTCWaterWader* wader)
{
    g_last_dtor = wader;
    wader->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWaterWader_scalar_deleting_destructor(CTCWaterWader* wader, int edx_unused, unsigned int flags)
{
    CTCWaterWader_dtor(wader);
    if (flags & 1)
        operator delete(wader);
    return wader;
}

int main()
{
    CTCWaterWader a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWaterWader_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWaterWader b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWaterWader_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007b62e0_TEST PASS\n");
    return 0;
}