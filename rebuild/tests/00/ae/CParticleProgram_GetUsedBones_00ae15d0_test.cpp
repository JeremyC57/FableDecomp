// Behaviour test for 00ae15d0 (CParticleProgram::GetUsedBones): bones of every group with at least one vertex blend are marked used, simulated groups first, then the non-simulated groups; groups with no blends mark nothing.
#include <cstdio>
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int i) { return _M_start[i]; }
    const T& operator[](unsigned int i) const { return _M_start[i]; }
    void Set(T* first, unsigned int n) { _M_start = first; _M_finish = first + n; _M_end_of_storage = first + n; }
};
struct C3DVertexBlend2 { long Vertex; float Weight; };
struct C3DGroup2 {
    long                     BoneIndex;
    CArray<C3DVertexBlend2>  VertexBlends;
};
static int g_calls = 0;
class CParticleProgram {
public:
    CArray<C3DGroup2> SimGroups;
    CArray<C3DGroup2> NonSimGroups;
    void GetUsedBones(CArray<bool>& used) const;
private:
    static void GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups);
};
void CParticleProgram::GetUsedBones(CArray<bool>& used, const CArray<C3DGroup2>& groups)
{
    ++g_calls;
    for (unsigned int i = 0; i < groups.size(); ++i)
        if ((long)groups[i].VertexBlends.size() > 0)
            used[groups[i].BoneIndex] = true;
}
void CParticleProgram::GetUsedBones(CArray<bool>& used) const
{
    GetUsedBones(used, SimGroups);
    GetUsedBones(used, NonSimGroups);
}
int main()
{
    C3DVertexBlend2 blends[2];
    blends[0].Vertex = 0; blends[0].Weight = 1.0f;
    blends[1].Vertex = 1; blends[1].Weight = 0.5f;
    C3DGroup2 sim[2], nonSim[2];
    sim[0].BoneIndex = 1; sim[0].VertexBlends.Set(blends, 2);
    sim[1].BoneIndex = 3; sim[1].VertexBlends.Set(blends, 0);      // no blends: bone 3 stays unused
    nonSim[0].BoneIndex = 4; nonSim[0].VertexBlends.Set(blends + 1, 1);
    nonSim[1].BoneIndex = 1; nonSim[1].VertexBlends.Set(blends, 1);
    CParticleProgram program;
    program.SimGroups.Set(sim, 2);
    program.NonSimGroups.Set(nonSim, 2);
    bool flags[6] = { false, false, false, false, false, false };
    CArray<bool> used;
    used.Set(flags, 6);
    program.GetUsedBones(used);
    if (g_calls != 2) { std::printf("00ae15d0_TEST FAIL\n"); return 1; }
    if (flags[0] || !flags[1] || flags[2] || flags[3] || !flags[4] || flags[5]) { std::printf("00ae15d0_TEST FAIL\n"); return 1; }
    std::printf("00ae15d0_TEST PASS\n");
    return 0;
}