// Behaviour test for CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData (0x007222B0): null-safe delete destroying the original morphs, then each mesh part, then the parts storage.
#include <cstdio>
#include <cstdlib>
static int g_order[8]; static int g_n = 0;
class CEngineTextureMorphSet { public: int tag; void* a; void* b; ~CEngineTextureMorphSet() { if (g_n < 8) g_order[g_n++] = tag; } };
struct Parts { unsigned int MeshIndex; CEngineTextureMorphSet TextureMorphs; };
struct PartsArray {
    Parts* Start; Parts* Finish; Parts* Eos;
    ~PartsArray() { for (Parts* p = Start; p != Finish; ++p) p->~Parts(); if (Start) { std::free(Start); if (g_n < 8) g_order[g_n++] = -1; } }
};
struct CMeshMorphSet { PartsArray MeshParts; CEngineTextureMorphSet OriginalMeshTextureMorphs; };
static void DeleteData(void* data) { delete (CMeshMorphSet*)data; }
int main()
{
    DeleteData(0);
    CMeshMorphSet* m = new CMeshMorphSet;
    m->OriginalMeshTextureMorphs.tag = 100;
    m->MeshParts.Start = (Parts*)std::malloc(2 * sizeof(Parts));
    m->MeshParts.Finish = m->MeshParts.Start + 2;
    m->MeshParts.Start[0].TextureMorphs.tag = 1; m->MeshParts.Start[1].TextureMorphs.tag = 2;
    DeleteData(m);
    if (g_n != 4 || g_order[0] != 100 || g_order[1] != 1 || g_order[2] != 2 || g_order[3] != -1) { std::printf("007222b0_TEST FAIL\n"); return 1; }
    std::printf("007222b0_TEST PASS\n");
    return 0;
}