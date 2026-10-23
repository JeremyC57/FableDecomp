#include <cstdio>

struct CEnginePrimitiveManagerParticleDecalGroup {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerParticleDecalGroup* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerParticleDecalGroup_dtor(CEnginePrimitiveManagerParticleDecalGroup* group)
{
    g_last_dtor = group;
    group->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerParticleDecalGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleDecalGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleDecalGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}

int main()
{
    CEnginePrimitiveManagerParticleDecalGroup a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerParticleDecalGroup_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerParticleDecalGroup b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerParticleDecalGroup_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bb9980_TEST PASS\n");
    return 0;
}