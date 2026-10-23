#include <cstdio>

struct CPSCLight {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPSCLight* g_last_dtor = 0;

void __fastcall CPSCLight_dtor(CPSCLight* light)
{
    g_last_dtor = light;
    light->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPSCLight_scalar_deleting_destructor(CPSCLight* light, int edx_unused, unsigned int flags)
{
    CPSCLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}

int main()
{
    CPSCLight a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPSCLight_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPSCLight b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPSCLight_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b1ae90_TEST PASS\n");
    return 0;
}