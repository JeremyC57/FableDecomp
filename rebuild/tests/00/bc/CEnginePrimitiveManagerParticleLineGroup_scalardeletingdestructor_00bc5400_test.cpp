#include <cstdio>

struct CEnginePrimitiveManagerParticleLineGroup {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerParticleLineGroup* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerParticleLineGroup_dtor(CEnginePrimitiveManagerParticleLineGroup* group)
{
    g_last_dtor = group;
    group->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerParticleLineGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleLineGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleLineGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}

int main()
{
    CEnginePrimitiveManagerParticleLineGroup a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerParticleLineGroup_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerParticleLineGroup b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerParticleLineGroup_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bc5400_TEST PASS\n");
    return 0;
}