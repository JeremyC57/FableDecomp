#include <cstdio>

struct CEngineVSConstantLayoutParticles {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineVSConstantLayoutParticles* g_last_dtor = 0;

void __fastcall CEngineVSConstantLayoutParticles_dtor(CEngineVSConstantLayoutParticles* particles)
{
    g_last_dtor = particles;
    particles->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineVSConstantLayoutParticles_scalar_deleting_destructor(CEngineVSConstantLayoutParticles* particles, int edx_unused, unsigned int flags)
{
    CEngineVSConstantLayoutParticles_dtor(particles);
    if (flags & 1)
        operator delete(particles);
    return particles;
}

int main()
{
    CEngineVSConstantLayoutParticles a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineVSConstantLayoutParticles_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineVSConstantLayoutParticles b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineVSConstantLayoutParticles_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bdb060_TEST PASS\n");
    return 0;
}