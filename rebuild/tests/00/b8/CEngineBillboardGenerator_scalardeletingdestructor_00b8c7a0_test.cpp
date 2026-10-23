#include <cstdio>

struct CEngineBillboardGenerator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineBillboardGenerator* g_last_dtor = 0;

void __fastcall CEngineBillboardGenerator_dtor(CEngineBillboardGenerator* generator)
{
    g_last_dtor = generator;
    generator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineBillboardGenerator_scalar_deleting_destructor(CEngineBillboardGenerator* generator, int edx_unused, unsigned int flags)
{
    CEngineBillboardGenerator_dtor(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}

int main()
{
    CEngineBillboardGenerator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineBillboardGenerator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineBillboardGenerator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineBillboardGenerator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b8c7a0_TEST PASS\n");
    return 0;
}