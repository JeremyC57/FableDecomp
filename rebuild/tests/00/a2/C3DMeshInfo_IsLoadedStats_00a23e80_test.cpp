#include <cstdio>
// Behaviour test for C3DMeshInfo::IsLoadedStats (0x00a23e80).
struct LOD { void* MeshData; };
struct MeshData { LOD* LODMeshes[4]; LOD* MeshStats; };
struct Info {
    MeshData* M; unsigned int LODDataSizes[4];
    bool HasLOD(unsigned int lod) const { return lod < 4 && LODDataSizes[lod] != 0; }
    bool Test() const
    {
        if (M) {
            if (1) { LOD* s = M->MeshStats; if (s && s->MeshData) return true; }
            for (unsigned int lod = 0; HasLOD(lod); ++lod) {
                LOD* info = M->LODMeshes[lod];
                if (info && info->MeshData) return true;
            }
        }
        return false;
    }
};
int main()
{
    int dummy;
    LOD empty = { 0 }, full = { &dummy };
    MeshData md = { { &empty, 0, &full, &full }, 0 };
    Info i = { &md, { 1, 1, 0, 1 } };
    bool ok = !i.Test();                       // LOD 2 not present stops the walk before the loaded LOD 2/3
    i.LODDataSizes[2] = 1; ok = ok && i.Test();
    i.M = 0; ok = ok && !i.Test();
    i.M = &md; i.LODDataSizes[0] = 0; md.MeshStats = &full;
    ok = ok && (i.Test() == (1 == 1));
    if (!ok) { std::printf("00a23e80_TEST FAIL\n"); return 1; }
    std::printf("00a23e80_TEST PASS\n");
    return 0;
}