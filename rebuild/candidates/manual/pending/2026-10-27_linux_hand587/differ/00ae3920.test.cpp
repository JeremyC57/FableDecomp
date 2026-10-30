// Behaviour test for CParticleProgram::RemapBoneIndices(map, groups) (0x00AE3920): groups whose bone is in the remap
// table survive in order with the remapped bone; the others are dropped.
#include <cstdio>
struct Remap { unsigned long keys[8]; unsigned long values[8]; int n;
    int find(unsigned long k) const { for (int i = 0; i < n; i++) if (keys[i] == k) return i; return -1; }
    unsigned long& operator[](unsigned long k) { int i = find(k); if (i < 0) { keys[n] = k; values[n] = 0; i = n++; } return values[i]; } };
struct Group { long BoneIndex; int Tag; void SetBoneIndex(long b) { BoneIndex = b; } };
struct Groups { Group items[8]; unsigned long count; };
static void RemapBoneIndices(Remap& remap, Groups& groups)
{
    Groups new_groups; new_groups.count = 0;
    for (unsigned long i = 0; i < groups.count; ++i) {
        Group& group = groups.items[i];
        if (remap.find(group.BoneIndex) >= 0) {
            group.SetBoneIndex(remap[group.BoneIndex]);
            new_groups.items[new_groups.count++] = group;
        }
    }
    groups = new_groups;
}
int main()
{
    Remap m; m.n = 0; m[3] = 30; m[5] = 50; m[0] = 7;
    Groups g; g.count = 5;
    long bones[] = { 3, 4, 5, 0, 9 };
    for (int i = 0; i < 5; i++) { g.items[i].BoneIndex = bones[i]; g.items[i].Tag = i; }
    RemapBoneIndices(m, g);
    bool ok = g.count == 3 && g.items[0].BoneIndex == 30 && g.items[0].Tag == 0 && g.items[1].BoneIndex == 50
           && g.items[1].Tag == 2 && g.items[2].BoneIndex == 7 && g.items[2].Tag == 3 && m.n == 3;
    if (!ok) { std::printf("00ae3920_TEST FAIL\n"); return 1; }
    std::printf("00ae3920_TEST PASS\n");
    return 0;
}
