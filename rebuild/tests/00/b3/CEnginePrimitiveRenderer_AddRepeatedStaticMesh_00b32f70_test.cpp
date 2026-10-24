#include <cstdio>
struct CAddMeshDesc { long MeshId; };
static int g_handles = 0;
struct CEnginePrimitiveHandle {
    long PrimitiveId;
    CEnginePrimitiveHandle(long id) : PrimitiveId(id) { ++g_handles; }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : PrimitiveId(other.PrimitiveId) { ++g_handles; }
    ~CEnginePrimitiveHandle() { --g_handles; }
};
struct CEnginePrimitiveManagerRepeatedStaticMeshes {
    long Created;
    CEnginePrimitiveManagerRepeatedStaticMeshes() : Created(0) {}
    CEnginePrimitiveHandle CreatePrimitive(const CAddMeshDesc& desc) { ++Created; return CEnginePrimitiveHandle(desc.MeshId * 10); }
};
// The candidate logic: forward to the repeated-static-mesh manager.
struct CEnginePrimitiveRenderer {
    CEnginePrimitiveManagerRepeatedStaticMeshes* RepeatedStaticMeshManager;
    CEnginePrimitiveHandle AddRepeatedStaticMesh(const CAddMeshDesc& desc) { return RepeatedStaticMeshManager->CreatePrimitive(desc); }
};
int main()
{
    CEnginePrimitiveManagerRepeatedStaticMeshes manager;
    CEnginePrimitiveRenderer renderer;
    renderer.RepeatedStaticMeshManager = &manager;
    CAddMeshDesc desc = { 4 };
    bool ok = true;
    {
        CEnginePrimitiveHandle h = renderer.AddRepeatedStaticMesh(desc);
        if (h.PrimitiveId != 40 || manager.Created != 1) ok = false;
    }
    if (g_handles != 0) ok = false;
    if (!ok) { std::printf("00b32f70_TEST FAIL\n"); return 1; }
    std::printf("00b32f70_TEST PASS\n");
    return 0;
}