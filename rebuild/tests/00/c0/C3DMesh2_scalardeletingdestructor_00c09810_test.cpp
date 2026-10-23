#include <cstdio>

struct C3DMesh2 {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DMesh2* g_last_dtor = 0;

void __fastcall C3DMesh2_dtor(C3DMesh2* mesh2)
{
    g_last_dtor = mesh2;
    mesh2->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DMesh2_scalar_deleting_destructor(C3DMesh2* mesh2, int edx_unused, unsigned int flags)
{
    C3DMesh2_dtor(mesh2);
    if (flags & 1)
        operator delete(mesh2);
    return mesh2;
}

int main()
{
    C3DMesh2 a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DMesh2_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DMesh2 b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DMesh2_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00c09810_TEST PASS\n");
    return 0;
}