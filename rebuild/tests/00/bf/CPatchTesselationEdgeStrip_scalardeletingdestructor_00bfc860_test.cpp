#include <cstdio>

class CPatchTesselationEdgeStrip { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPatchTesselationEdgeStrip* g_last_dtor = 0;

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* strip)
{
    g_last_dtor = strip;
    strip->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPatchTesselationEdgeStrip_scalar_deleting_destructor(CPatchTesselationEdgeStrip* strip, int edx_unused, unsigned int flags)
{
    CPatchTesselationEdgeStrip_dtor(strip);
    if (flags & 1)
        operator delete(strip);
    return strip;
}

int main()
{
    CPatchTesselationEdgeStrip a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPatchTesselationEdgeStrip_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPatchTesselationEdgeStrip b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPatchTesselationEdgeStrip_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bfc860_TEST PASS\n");
    return 0;
}