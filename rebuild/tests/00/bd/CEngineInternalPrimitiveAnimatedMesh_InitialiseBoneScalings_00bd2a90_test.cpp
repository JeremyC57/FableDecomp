// Behaviour test for CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings (0x00BD2A90): a model of the cache fill --
// no skeletal morph: nothing; otherwise the mesh's entry (keyed by mesh ID) is sized to the bone count and each scaling is
// taken from the combined morph target by bone name CRC.
#include <cstdio>
#include <map>
#include <vector>
struct Vec { float X, Y, Z; };
struct Target { void GetBoneScaling(Vec& v, unsigned long crc) const { v.X = (float)crc; v.Y = 1.0f; v.Z = 2.0f; } };
struct Morph { Target combined; const Target* PeekCombinedMorph() const { return &combined; } };
struct Bone { unsigned long NameCRC; };
struct Mesh { unsigned long MeshID; std::vector<Bone> Bones;
    unsigned long GetBoneCount() const { return (unsigned long)Bones.size(); }
    const Bone* GetBone(unsigned long i) const { return &Bones[i]; }
    unsigned long GetMeshID() const { return MeshID; } };
struct Prim {
    const Morph* skeletal;
    mutable std::map<unsigned long, std::vector<Vec> > BoneScalings;
    void InitialiseBoneScalings(const Mesh& mesh) const
    {
        if (skeletal) {
            const Target* target = skeletal->PeekCombinedMorph();
            unsigned long boneCount = mesh.GetBoneCount();
            std::vector<Vec>& scalings = BoneScalings[mesh.GetMeshID()];
            scalings.resize(boneCount);
            for (unsigned long i = 0; i < boneCount; ++i)
                target->GetBoneScaling(scalings[i], mesh.GetBone(i)->NameCRC);
        }
    }
};
int main()
{
    Mesh m; m.MeshID = 9; Bone b; b.NameCRC = 5; m.Bones.push_back(b); b.NameCRC = 7; m.Bones.push_back(b);
    Prim none; none.skeletal = 0; none.InitialiseBoneScalings(m);
    if (!none.BoneScalings.empty()) { std::printf("00bd2a90_TEST FAIL\n"); return 1; }
    Morph morph; Prim p; p.skeletal = &morph; p.InitialiseBoneScalings(m);
    if (p.BoneScalings.size() != 1 || p.BoneScalings[9].size() != 2 || p.BoneScalings[9][0].X != 5.0f || p.BoneScalings[9][1].X != 7.0f)
        { std::printf("00bd2a90_TEST FAIL\n"); return 1; }
    Mesh empty; empty.MeshID = 3; p.InitialiseBoneScalings(empty);
    if (p.BoneScalings.size() != 2 || !p.BoneScalings[3].empty()) { std::printf("00bd2a90_TEST FAIL\n"); return 1; }
    std::printf("00bd2a90_TEST PASS\n");
    return 0;
}