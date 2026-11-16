// Behaviour test for 0x00A8B990 C3DMesh2::InitialiseBones: every bone chunk becomes a counted bone (OriginalNoChildren reset to 0,
// its parent's OriginalNoChildren incremented when Parent != -1) plus its name; the used-bone flags come from GetUsedBones and are
// propagated to every ancestor by one walk from the last bone down to the first (parents precede children in the file); BoneCount =
// number of used bones; the used bones are copied in file order into the mesh's bone array (default-constructed first: Parent -1,
// OriginalNoChildren 0) with their names, old index -> new index recorded in the remap; RemapBoneIndices rewrites the parents
// through the remap; PackBoneNames failing returns false (keyframes not built), otherwise the keyframes are built and true returned.
#include <cstdio>
#include <vector>
#include <map>
#include <string>

struct Bone { long Parent; long OriginalNoChildren; int tag; Bone() : Parent(-1), OriginalNoChildren(0), tag(-1) { } };
struct FileBone { long Parent; long Children; int tag; const char* name; };
struct Mesh {
    std::vector<Bone> Bones; unsigned long BoneCount; std::vector<std::string> names; bool keyframes;
    std::vector<int> explicitUsed;   // what GetUsedBones marks besides bone 0
    bool packOk;
};

static bool InitialiseBones(Mesh& m, const std::vector<FileBone>& file)
{
    std::vector<Bone> bones; std::vector<std::string> bone_names;
    for (size_t k = 0; k < file.size(); ++k) {
        Bone b; b.Parent = file[k].Parent; b.OriginalNoChildren = file[k].Children; b.tag = file[k].tag;
        b.OriginalNoChildren = 0;
        if (b.Parent != -1) ++bones[b.Parent].OriginalNoChildren;
        bones.push_back(b); bone_names.push_back(file[k].name);
    }
    std::vector<bool> used(bones.size(), false);
    if (!used.empty()) used[0] = true;                                   // GetUsedBones: root
    for (size_t k = 0; k < m.explicitUsed.size(); ++k) used[m.explicitUsed[k]] = true;
    for (long i = (long)used.size() - 1; i >= 0; --i)
        if (used[i] && bones[i].Parent != -1) used[bones[i].Parent] = true;
    m.BoneCount = 0;
    for (size_t i = 0; i < used.size(); ++i) if (used[i]) ++m.BoneCount;
    m.Bones.assign(m.BoneCount, Bone());
    std::vector<std::string> used_names(m.BoneCount);
    std::map<unsigned long, unsigned long> remap; unsigned long n = 0;
    for (size_t j = 0; j < used.size(); ++j)
        if (used[j]) { m.Bones[n] = bones[j]; used_names[n] = bone_names[j]; remap[(unsigned long)j] = n; ++n; }
    for (unsigned long i = 0; i < m.BoneCount; ++i)                      // RemapBoneIndices (parents)
        if (m.Bones[i].Parent != -1) {
            std::map<unsigned long, unsigned long>::iterator it = remap.find((unsigned long)m.Bones[i].Parent);
            m.Bones[i].Parent = it == remap.end() ? -1 : (long)it->second;
        }
    if (!used_names.empty() && !m.packOk) return false;                  // PackBoneNames
    m.names = used_names; m.keyframes = true;
    return true;
}

static int fails = 0;
static void check(bool c, const char* what) { if (!c) { ++fails; printf("FAIL %s\n", what); } }

int main()
{
    // 0 root; 1 <- 0; 2 <- 1; 3 <- 0; 4 <- 3; 5 <- 4   (children counts in the file are stale garbage: must be recomputed)
    FileBone f[6] = { {-1, 7, 10, "root"}, {0, 9, 11, "spine"}, {1, 9, 12, "head"}, {0, 9, 13, "leg"}, {3, 9, 14, "knee"}, {4, 9, 15, "foot"} };
    std::vector<FileBone> file(f, f + 6);
    {   // only the foot used explicitly: its whole chain (0,3,4,5) kept; spine/head dropped
        Mesh m; m.explicitUsed.push_back(5); m.packOk = true; m.keyframes = false;
        bool r = InitialiseBones(m, file);
        check(r, "returns true");
        check(m.BoneCount == 4, "BoneCount = chain of the foot");
        check(m.Bones.size() == 4 && m.Bones[0].tag == 10 && m.Bones[1].tag == 13 && m.Bones[2].tag == 14 && m.Bones[3].tag == 15, "file order kept");
        check(m.Bones[0].Parent == -1 && m.Bones[1].Parent == 0 && m.Bones[2].Parent == 1 && m.Bones[3].Parent == 2, "parents remapped");
        check(m.Bones[0].OriginalNoChildren == 2 && m.Bones[1].OriginalNoChildren == 1 && m.Bones[3].OriginalNoChildren == 0, "children recounted (all file bones)");
        check(m.names.size() == 4 && m.names[1] == "leg" && m.names[3] == "foot", "names follow the bones");
        check(m.keyframes, "keyframes built");
    }
    {   // head used: spine pulled in (ancestor walk), leg chain dropped
        Mesh m; m.explicitUsed.push_back(2); m.packOk = true; m.keyframes = false;
        InitialiseBones(m, file);
        check(m.BoneCount == 3 && m.Bones[1].tag == 11 && m.Bones[2].tag == 12 && m.Bones[2].Parent == 1, "head chain");
    }
    {   // name packing refused: false, keyframes not built
        Mesh m; m.packOk = false; m.keyframes = false;
        bool r = InitialiseBones(m, file);
        check(!r && !m.keyframes && m.BoneCount == 1, "pack failure returns false");
    }
    {   // no bones at all: nothing used, packing of an empty list succeeds
        Mesh m; m.packOk = false; m.keyframes = false;
        bool r = InitialiseBones(m, std::vector<FileBone>());
        check(r && m.BoneCount == 0 && m.keyframes, "empty mesh");
    }
    if (fails == 0) printf("00a8b990_TEST PASS\n");
    return fails;
}