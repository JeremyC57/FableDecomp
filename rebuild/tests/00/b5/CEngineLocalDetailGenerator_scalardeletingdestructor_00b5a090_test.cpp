#include <cstdio>

struct CEngineLocalDetailGenerator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineLocalDetailGenerator* g_last_dtor = 0;

void __fastcall CEngineLocalDetailGenerator_dtor(CEngineLocalDetailGenerator* generator)
{
    g_last_dtor = generator;
    generator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineLocalDetailGenerator_scalar_deleting_destructor(CEngineLocalDetailGenerator* generator, int edx_unused, unsigned int flags)
{
    CEngineLocalDetailGenerator_dtor(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}

int main()
{
    CEngineLocalDetailGenerator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineLocalDetailGenerator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineLocalDetailGenerator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineLocalDetailGenerator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}