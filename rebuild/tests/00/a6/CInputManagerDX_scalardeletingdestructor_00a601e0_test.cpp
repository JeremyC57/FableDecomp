#include <cstdio>

struct CInputManagerDX {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputManagerDX* g_last_dtor = 0;

void __fastcall CInputManagerDX_dtor(CInputManagerDX* managerDX)
{
    g_last_dtor = managerDX;
    managerDX->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputManagerDX_scalar_deleting_destructor(CInputManagerDX* managerDX, int edx_unused, unsigned int flags)
{
    CInputManagerDX_dtor(managerDX);
    if (flags & 1)
        operator delete(managerDX);
    return managerDX;
}

int main()
{
    CInputManagerDX a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputManagerDX_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputManagerDX b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputManagerDX_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a601e0_TEST PASS\n");
    return 0;
}