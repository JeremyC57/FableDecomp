#include <cstdio>

struct CDeviceIndependent {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CDeviceIndependent* g_last_dtor = 0;

void __fastcall CDeviceIndependent_dtor(CDeviceIndependent* independent)
{
    g_last_dtor = independent;
    independent->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CDeviceIndependent_scalar_deleting_destructor(CDeviceIndependent* independent, int edx_unused, unsigned int flags)
{
    CDeviceIndependent_dtor(independent);
    if (flags & 1)
        operator delete(independent);
    return independent;
}

int main()
{
    CDeviceIndependent a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CDeviceIndependent_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CDeviceIndependent b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CDeviceIndependent_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a978a0_TEST PASS\n");
    return 0;
}