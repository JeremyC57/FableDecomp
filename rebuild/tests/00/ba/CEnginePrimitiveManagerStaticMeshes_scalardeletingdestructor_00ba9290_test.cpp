#include <cstdio>

class CEnginePrimitiveManagerStaticMeshes { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerStaticMeshes* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerStaticMeshes_dtor(CEnginePrimitiveManagerStaticMeshes* meshes)
{
    g_last_dtor = meshes;
    meshes->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerStaticMeshes_scalar_deleting_destructor(CEnginePrimitiveManagerStaticMeshes* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerStaticMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}

int main()
{
    CEnginePrimitiveManagerStaticMeshes a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerStaticMeshes_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerStaticMeshes b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerStaticMeshes_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00ba9290_TEST PASS\n");
    return 0;
}