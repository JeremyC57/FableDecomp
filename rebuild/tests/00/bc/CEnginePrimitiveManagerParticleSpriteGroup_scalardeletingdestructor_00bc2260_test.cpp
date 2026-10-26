#include <cstdio>

class CEnginePrimitiveManagerParticleSpriteGroup { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerParticleSpriteGroup* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerParticleSpriteGroup_dtor(CEnginePrimitiveManagerParticleSpriteGroup* group)
{
    g_last_dtor = group;
    group->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerParticleSpriteGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleSpriteGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleSpriteGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}

int main()
{
    CEnginePrimitiveManagerParticleSpriteGroup a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerParticleSpriteGroup_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerParticleSpriteGroup b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerParticleSpriteGroup_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bc2260_TEST PASS\n");
    return 0;
}