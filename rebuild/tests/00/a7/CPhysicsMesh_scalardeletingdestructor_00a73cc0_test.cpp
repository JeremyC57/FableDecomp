#include <cstdio>

struct CPhysicsMesh {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPhysicsMesh* g_last_dtor = 0;

void __fastcall CPhysicsMesh_dtor(CPhysicsMesh* mesh)
{
    g_last_dtor = mesh;
    mesh->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPhysicsMesh_scalar_deleting_destructor(CPhysicsMesh* mesh, int edx_unused, unsigned int flags)
{
    CPhysicsMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}

int main()
{
    CPhysicsMesh a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPhysicsMesh_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPhysicsMesh b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPhysicsMesh_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}