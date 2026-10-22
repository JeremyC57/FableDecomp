#include <cstdio>

struct CEngineClothSimulationStaticMesh {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineClothSimulationStaticMesh* g_last_dtor = 0;

void __fastcall CEngineClothSimulationStaticMesh_dtor(CEngineClothSimulationStaticMesh* mesh)
{
    g_last_dtor = mesh;
    mesh->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineClothSimulationStaticMesh_scalar_deleting_destructor(CEngineClothSimulationStaticMesh* mesh, int edx_unused, unsigned int flags)
{
    CEngineClothSimulationStaticMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}

int main()
{
    CEngineClothSimulationStaticMesh a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineClothSimulationStaticMesh_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineClothSimulationStaticMesh b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineClothSimulationStaticMesh_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}