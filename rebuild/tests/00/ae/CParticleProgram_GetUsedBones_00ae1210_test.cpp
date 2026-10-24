// behaviour test for 0x00AE1210 CParticleProgram::GetUsedBones (static): mark every bone that owns at least one blend
#include <cstdio>
struct C3DVertexBlend2 { long Vertex; float Weight; };
struct C3DGroup2 { long BoneIndex; C3DVertexBlend2* Start; C3DVertexBlend2* Finish;
    unsigned int BlendCount() const { return (unsigned int)(Finish - Start); } };
struct CGroupArray { C3DGroup2* Start; C3DGroup2* Finish;
    unsigned int size() const { return (unsigned int)(Finish - Start); }
    const C3DGroup2& operator[](unsigned int i) const { return Start[i]; } };
struct CBoolArray { bool* Start; bool& operator[](unsigned int i) { return Start[i]; } };
void GetUsedBones(CBoolArray& used, const CGroupArray& groups)
{
    for (unsigned int i = 0; i < groups.size(); ++i)
        if ((long)groups[i].BlendCount() > 0)
            used[groups[i].BoneIndex] = true;
}
int main()
{
    C3DVertexBlend2 b[2] = { { 0, 1.0f }, { 1, 1.0f } };
    C3DGroup2 g[3];
    g[0].BoneIndex = 1; g[0].Start = b; g[0].Finish = b + 1;
    g[1].BoneIndex = 3; g[1].Start = b + 1; g[1].Finish = b + 1;   // empty: not marked
    g[2].BoneIndex = 4; g[2].Start = b; g[2].Finish = b + 2;
    bool flags[6] = { false, false, false, false, false, false };
    CBoolArray used; used.Start = flags;
    CGroupArray arr; arr.Start = g; arr.Finish = g + 3;
    GetUsedBones(used, arr);
    bool expect[6] = { false, true, false, false, true, false };
    for (int i = 0; i < 6; ++i)
        if (flags[i] != expect[i]) { std::printf("00ae1210_TEST FAIL\n"); return 1; }
    std::printf("00ae1210_TEST PASS\n");
    return 0;
}