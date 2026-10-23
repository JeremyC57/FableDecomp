#include <cstdio>

struct CEngineInternalPrimitiveParticleSpriteTrailGroup {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveParticleSpriteTrailGroup* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_dtor(CEngineInternalPrimitiveParticleSpriteTrailGroup* group)
{
    g_last_dtor = group;
    group->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleSpriteTrailGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleSpriteTrailGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}

int main()
{
    CEngineInternalPrimitiveParticleSpriteTrailGroup a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveParticleSpriteTrailGroup b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bbf180_TEST PASS\n");
    return 0;
}