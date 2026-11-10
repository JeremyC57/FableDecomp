// Behaviour test for 0x00AE3920 CParticleProgram::RemapBoneIndices(map<unsigned long,unsigned long>&, CArray<C3DGroup2>&): keep
// only the groups whose bone is a key of the remap table, each with its bone index replaced by the mapped value, in order.
#include <stdio.h>
#include <map>
#include <vector>

class C3DGroup2 { public:
    long BoneIndex;
    int Payload;
    long GetBoneIndex() const { return BoneIndex; }
    void SetBoneIndex(long index) { BoneIndex = index; }
};

static void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap, std::vector<C3DGroup2>& groups)
{
    std::vector<C3DGroup2> new_groups;
    new_groups.reserve(groups.size());
    for (unsigned long i = 0; i < groups.size(); ++i) {
        C3DGroup2& group = groups[i];
        unsigned long bone_index = group.GetBoneIndex();
        if (remap.find(bone_index) != remap.end()) {
            group.SetBoneIndex(remap[bone_index]);
            new_groups.push_back(group);
        }
    }
    groups = new_groups;
}

int main()
{
    std::map<unsigned long, unsigned long> remap;
    remap[2] = 20; remap[5] = 0; remap[7] = 3;
    std::vector<C3DGroup2> groups;
    long bones[6] = { 1, 2, 5, 6, 7, 2 };
    for (int i = 0; i < 6; ++i) { C3DGroup2 g; g.BoneIndex = bones[i]; g.Payload = 100 + i; groups.push_back(g); }
    RemapBoneIndices(remap, groups);
    int ok = groups.size() == 4
        && groups[0].BoneIndex == 20 && groups[0].Payload == 101
        && groups[1].BoneIndex == 0 && groups[1].Payload == 102
        && groups[2].BoneIndex == 3 && groups[2].Payload == 104
        && groups[3].BoneIndex == 20 && groups[3].Payload == 105
        && remap.size() == 3;
    std::vector<C3DGroup2> none;
    RemapBoneIndices(remap, none);
    ok = ok && none.empty();
    printf(ok ? "00ae3920_TEST PASS\n" : "00ae3920_TEST FAIL\n");
    return ok ? 0 : 1;
}