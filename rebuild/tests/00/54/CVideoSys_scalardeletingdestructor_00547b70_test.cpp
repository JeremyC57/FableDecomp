#include <cstdio>

struct CVideoSys {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CVideoSys* g_last_dtor = 0;

void __fastcall CVideoSys_dtor(CVideoSys* sys)
{
    g_last_dtor = sys;
    sys->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CVideoSys_scalar_deleting_destructor(CVideoSys* sys, int edx_unused, unsigned int flags)
{
    CVideoSys_dtor(sys);
    if (flags & 1)
        operator delete(sys);
    return sys;
}

int main()
{
    CVideoSys a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CVideoSys_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CVideoSys b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CVideoSys_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}