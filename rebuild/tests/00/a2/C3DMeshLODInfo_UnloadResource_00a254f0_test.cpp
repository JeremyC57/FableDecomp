#include <cstdio>
// Behaviour test for C3DMeshLODInfo::UnloadResource (0x00A254F0).
struct Info { unsigned int RefCount; };
struct MeshInfo { int removedLod; bool keep; void RemoveLOD(unsigned int lod, bool k) { removedLod = (int)lod; keep = k; } };
struct LODInfo {
    void* MeshData; Info* MeshInfoBlock; MeshInfo* Owner; unsigned char LOD;
    bool CanUnload() const
    {
        if (MeshData == 0) return false;
        return (MeshInfoBlock ? MeshInfoBlock->RefCount : 0) == 1;
    }
    bool UnloadResource() { if (CanUnload()) { Owner->RemoveLOD(LOD, true); return true; } return false; }
};
int main()
{
    int mesh; Info one = { 1 }, two = { 2 };
    MeshInfo owner = { -1, false };
    LODInfo l = { &mesh, &one, &owner, 3 };
    bool ok = l.UnloadResource() && owner.removedLod == 3 && owner.keep;
    owner.removedLod = -1; l.MeshInfoBlock = &two; ok = ok && !l.UnloadResource() && owner.removedLod == -1;
    l.MeshInfoBlock = 0; ok = ok && !l.UnloadResource();
    l.MeshInfoBlock = &one; l.MeshData = 0; ok = ok && !l.UnloadResource() && owner.removedLod == -1;
    if (!ok) { std::printf("00a254f0_TEST FAIL\n"); return 1; }
    std::printf("00a254f0_TEST PASS\n");
    return 0;
}