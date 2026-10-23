#include <cstdio>

struct CParticleContext {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CParticleContext* g_last_dtor = 0;

void __fastcall CParticleContext_dtor(CParticleContext* context)
{
    g_last_dtor = context;
    context->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CParticleContext_scalar_deleting_destructor(CParticleContext* context, int edx_unused, unsigned int flags)
{
    CParticleContext_dtor(context);
    if (flags & 1)
        operator delete(context);
    return context;
}

int main()
{
    CParticleContext a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CParticleContext_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CParticleContext b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CParticleContext_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b806a0_TEST PASS\n");
    return 0;
}