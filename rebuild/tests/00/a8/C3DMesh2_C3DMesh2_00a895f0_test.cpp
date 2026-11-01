// Behaviour test for C3DMesh2::~C3DMesh2 (0x00A895F0): when MemoryHandle is allocated (!= -1) the materials and primitives are
// destroyed in place (GFExplicitDelete: null array or zero count destroys nothing) and the handle is freed and reset; the
// primitive and bone-name handles are freed and reset when allocated; then the C3DMeshStats base runs.
#include <cstdio>
#include <new>
static int g_materialDtors, g_primitiveDtors, g_frees, g_statsDtor;
static long g_freedHandles[8];
struct PoolModel { void FreeResource(long h) { g_freedHandles[g_frees++] = h; } };
struct MaterialModel { virtual ~MaterialModel() { ++g_materialDtors; } char body[0x2c]; };
struct PrimitiveModel { virtual ~PrimitiveModel() { ++g_primitiveDtors; } char body[0x9c]; };
template <class T> void ExplicitDelete(T* objects, unsigned long count)
{
    if (objects) {
        for (unsigned long i = 0; i < count; ++i)
            objects[i].~T();
    }
}
struct StatsModel { PoolModel* MemoryPool; virtual ~StatsModel() { ++g_statsDtor; } };
struct MeshModel : StatsModel {
    long MemoryHandle, PrimMemoryHandle, BoneNameMemoryHandle;
    unsigned long MaterialCount; MaterialModel* Materials;
    unsigned long PrimitiveCount; PrimitiveModel* Primitives;
    ~MeshModel()
    {
        if (MemoryHandle != -1) {
            ExplicitDelete(Materials, MaterialCount);
            ExplicitDelete(Primitives, PrimitiveCount);
            MemoryPool->FreeResource(MemoryHandle);
            MemoryHandle = -1;
        }
        if (PrimMemoryHandle != -1) { MemoryPool->FreeResource(PrimMemoryHandle); PrimMemoryHandle = -1; }
        if (BoneNameMemoryHandle != -1) { MemoryPool->FreeResource(BoneNameMemoryHandle); BoneNameMemoryHandle = -1; }
    }
};
static double g_blockStorage[(3 * sizeof(MaterialModel) + 2 * sizeof(PrimitiveModel)) / sizeof(double) + 2];   // 8-aligned pool block
static unsigned char* const g_block = (unsigned char*)g_blockStorage;
int main()
{
    PoolModel pool;
    MaterialModel* mats = (MaterialModel*)g_block;
    PrimitiveModel* prims = (PrimitiveModel*)(g_block + 3 * sizeof(MaterialModel));
    for (int i = 0; i < 3; ++i) new (&mats[i]) MaterialModel;
    for (int j = 0; j < 2; ++j) new (&prims[j]) PrimitiveModel;
    StatsModel* p = new MeshModel;
    MeshModel* m = (MeshModel*)p;
    m->MemoryPool = &pool; m->MemoryHandle = 5; m->PrimMemoryHandle = -1; m->BoneNameMemoryHandle = 9;
    m->MaterialCount = 3; m->Materials = mats; m->PrimitiveCount = 2; m->Primitives = prims;
    delete p;
    if (g_materialDtors != 3 || g_primitiveDtors != 2 || g_frees != 2 || g_freedHandles[0] != 5 || g_freedHandles[1] != 9
        || g_statsDtor != 1) { std::printf("00a895f0_TEST FAIL\n"); return 1; }
    {
        MeshModel n;
        n.MemoryPool = &pool; n.MemoryHandle = -1; n.PrimMemoryHandle = 7; n.BoneNameMemoryHandle = -1;
        n.MaterialCount = 3; n.Materials = mats; n.PrimitiveCount = 0; n.Primitives = 0;
    }
    if (g_materialDtors != 3 || g_frees != 3 || g_freedHandles[2] != 7 || g_statsDtor != 2) { std::printf("00a895f0_TEST FAIL\n"); return 1; }
    {
        MeshModel z;
        z.MemoryPool = &pool; z.MemoryHandle = 1; z.PrimMemoryHandle = -1; z.BoneNameMemoryHandle = -1;
        z.MaterialCount = 4; z.Materials = 0; z.PrimitiveCount = 0; z.Primitives = prims;
    }
    if (g_materialDtors != 3 || g_primitiveDtors != 2 || g_frees != 4 || g_freedHandles[3] != 1) { std::printf("00a895f0_TEST FAIL\n"); return 1; }
    std::printf("00a895f0_TEST PASS\n");
    return 0;
}