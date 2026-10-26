// Behaviour test for 00ae3a30 (CParticleProgram::RemapBoneIndices): the same remap table is applied to the simulated groups, then to the non-simulated groups.
#include <cstdio>
class C3DGroup2 { public: long BoneIndex; };
template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CBoneRemap {                         // stands in for std::map<unsigned long, unsigned long>
    long offset;
};
static const void* g_groupsSeen[4];
static int g_calls = 0;
class CParticleProgram {
public:
    CArray<C3DGroup2> SimGroups;
    CArray<C3DGroup2> NonSimGroups;
    void RemapBoneIndices(CBoneRemap& remap);
private:
    static void RemapBoneIndices(CBoneRemap& remap, CArray<C3DGroup2>& groups);
};
void CParticleProgram::RemapBoneIndices(CBoneRemap& remap, CArray<C3DGroup2>& groups)   // stands in for 0x00AE3920
{
    g_groupsSeen[g_calls++] = &groups;
    for (C3DGroup2* g = groups._M_start; g != groups._M_finish; ++g)
        g->BoneIndex += remap.offset;
}
void CParticleProgram::RemapBoneIndices(CBoneRemap& remap)
{
    RemapBoneIndices(remap, SimGroups);
    RemapBoneIndices(remap, NonSimGroups);
}
int main()
{
    C3DGroup2 sim[2], nonSim[1];
    sim[0].BoneIndex = 1; sim[1].BoneIndex = 2; nonSim[0].BoneIndex = 7;
    CParticleProgram program;
    program.SimGroups._M_start = sim; program.SimGroups._M_finish = sim + 2; program.SimGroups._M_end_of_storage = sim + 2;
    program.NonSimGroups._M_start = nonSim; program.NonSimGroups._M_finish = nonSim + 1; program.NonSimGroups._M_end_of_storage = nonSim + 1;
    CBoneRemap remap;
    remap.offset = 10;
    program.RemapBoneIndices(remap);
    if (g_calls != 2 || g_groupsSeen[0] != &program.SimGroups || g_groupsSeen[1] != &program.NonSimGroups) { std::printf("00ae3a30_TEST FAIL\n"); return 1; }
    if (sim[0].BoneIndex != 11 || sim[1].BoneIndex != 12 || nonSim[0].BoneIndex != 17) { std::printf("00ae3a30_TEST FAIL\n"); return 1; }
    std::printf("00ae3a30_TEST PASS\n");
    return 0;
}