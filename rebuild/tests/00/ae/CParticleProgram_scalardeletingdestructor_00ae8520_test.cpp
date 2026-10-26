#include <cstdio>

class CParticleProgram { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CParticleProgram* g_last_dtor = 0;

void __fastcall CParticleProgram_dtor(CParticleProgram* program)
{
    g_last_dtor = program;
    program->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CParticleProgram_scalar_deleting_destructor(CParticleProgram* program, int edx_unused, unsigned int flags)
{
    CParticleProgram_dtor(program);
    if (flags & 1)
        operator delete(program);
    return program;
}

int main()
{
    CParticleProgram a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CParticleProgram_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CParticleProgram b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CParticleProgram_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00ae8520_TEST PASS\n");
    return 0;
}