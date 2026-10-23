#include <cstdio>

struct CEngineInternalPrimitiveParticleDecalGroup {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveParticleDecalGroup* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveParticleDecalGroup_dtor(CEngineInternalPrimitiveParticleDecalGroup* group)
{
    g_last_dtor = group;
    group->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveParticleDecalGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleDecalGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleDecalGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}

int main()
{
    CEngineInternalPrimitiveParticleDecalGroup a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveParticleDecalGroup_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveParticleDecalGroup b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveParticleDecalGroup_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bba240_TEST PASS\n");
    return 0;
}