#include <cstdio>

struct CTCDParticleEmitter {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDParticleEmitter* g_last_dtor = 0;

void __fastcall CTCDParticleEmitter_dtor(CTCDParticleEmitter* emitter)
{
    g_last_dtor = emitter;
    emitter->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDParticleEmitter_scalar_deleting_destructor(CTCDParticleEmitter* emitter, int edx_unused, unsigned int flags)
{
    CTCDParticleEmitter_dtor(emitter);
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}

int main()
{
    CTCDParticleEmitter a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDParticleEmitter_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDParticleEmitter b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDParticleEmitter_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006e1e40_TEST PASS\n");
    return 0;
}