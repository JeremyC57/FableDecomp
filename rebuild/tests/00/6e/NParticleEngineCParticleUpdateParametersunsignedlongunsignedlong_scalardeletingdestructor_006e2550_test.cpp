#include <cstdio>

struct CParticleUpdateParameters {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CParticleUpdateParameters* g_last_dtor = 0;

void __fastcall CParticleUpdateParameters_dtor(CParticleUpdateParameters* parameters)
{
    g_last_dtor = parameters;
    parameters->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CParticleUpdateParameters_scalar_deleting_destructor(CParticleUpdateParameters* parameters, int edx_unused, unsigned int flags)
{
    CParticleUpdateParameters_dtor(parameters);
    if (flags & 1)
        operator delete(parameters);
    return parameters;
}

int main()
{
    CParticleUpdateParameters a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CParticleUpdateParameters_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CParticleUpdateParameters b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CParticleUpdateParameters_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006e2550_TEST PASS\n");
    return 0;
}