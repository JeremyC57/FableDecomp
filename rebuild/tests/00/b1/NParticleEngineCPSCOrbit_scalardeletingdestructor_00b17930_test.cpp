#include <cstdio>

struct CPSCOrbit {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCOrbit* g_last_dtor = 0;

void __fastcall CPSCOrbit_dtor(CPSCOrbit* orbit)
{
    g_last_dtor = orbit;
    orbit->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCOrbit_scalar_deleting_destructor(CPSCOrbit* orbit, int edx_unused, unsigned int flags)
{
    CPSCOrbit_dtor(orbit);
    if (flags & 1)
        operator delete(orbit);
    return orbit;
}

int main()
{
    CPSCOrbit a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCOrbit_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCOrbit b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCOrbit_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b17930_TEST PASS\n");
    return 0;
}