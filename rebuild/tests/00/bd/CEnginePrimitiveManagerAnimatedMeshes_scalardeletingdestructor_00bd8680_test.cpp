#include <cstdio>

class CEnginePrimitiveManagerAnimatedMeshes { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerAnimatedMeshes* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerAnimatedMeshes_dtor(CEnginePrimitiveManagerAnimatedMeshes* meshes)
{
    g_last_dtor = meshes;
    meshes->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerAnimatedMeshes_scalar_deleting_destructor(CEnginePrimitiveManagerAnimatedMeshes* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerAnimatedMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}

int main()
{
    CEnginePrimitiveManagerAnimatedMeshes a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerAnimatedMeshes_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerAnimatedMeshes b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerAnimatedMeshes_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bd8680_TEST PASS\n");
    return 0;
}