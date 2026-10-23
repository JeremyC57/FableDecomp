#include <cstdio>

struct CSharedData {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSharedData* g_last_dtor = 0;

void __fastcall CSharedData_dtor(CSharedData* shared)
{
    g_last_dtor = shared;
    shared->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSharedData_scalar_deleting_destructor(CSharedData* shared, int edx_unused, unsigned int flags)
{
    CSharedData_dtor(shared);
    if (flags & 1)
        operator delete(shared);
    return shared;
}

int main()
{
    CSharedData a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSharedData_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSharedData b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSharedData_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a3d440_TEST PASS\n");
    return 0;
}