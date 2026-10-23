#include <cstdio>

struct CEnvironment {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnvironment* g_last_dtor = 0;

void __fastcall CEnvironment_dtor(CEnvironment* environment)
{
    g_last_dtor = environment;
    environment->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnvironment_scalar_deleting_destructor(CEnvironment* environment, int edx_unused, unsigned int flags)
{
    CEnvironment_dtor(environment);
    if (flags & 1)
        operator delete(environment);
    return environment;
}

int main()
{
    CEnvironment a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnvironment_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnvironment b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnvironment_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006b7360_TEST PASS\n");
    return 0;
}