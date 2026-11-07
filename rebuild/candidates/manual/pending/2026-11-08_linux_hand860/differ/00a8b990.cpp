// C3DMesh2::InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh) -- read the sub-mesh's bone chunks into counted bones (counting each parent's children) and names, keep only the bones the mesh uses (and their ancestors), pack them into the mesh's bone array with their names, remap every bone index, then build the bone keyframes; false when the names do not pack
// Grade: DIFFER(1608v1624, md=317, 61 lines ignoring registers) -- in the bone loop retail keeps the bones vector's _M_finish in edi (eos in ebp) and spills bone.Info to [esp+0x34]; ours keeps Info in edi and _M_finish in memory, so retail's spilled `bone` takes the bottom frame slot that used_bone_names then shares and every later slot is 0xC higher; the used_bone_names copy loop's pointer induction (retail reads used_bone_names.begin() after the map ctor and steps a pointer kept in [esp+0x14]) follows from that
// Next idea: ~190 grades (comb.py over 9 sites: decl order, Reset / ctor / raw-pointer forms, parent local, PUSHBACK wrapper, name temporary, loop forms, the copy-loop pointer / index / iterator forms) never change which of _M_finish / bone.Info gets edi; the register choice is the allocator's weighting of the two live ranges across the inlined _M_insert_overflow path, so try spellings that change their use counts there (a by-value CCountedPointer argument, an explicit `bones.insert(bones.end(), bone)` with the overflow body visible) before parking for good
// Leaf: ?InitialiseBones@C3DMesh2@@AAE_NABVC3DMeshFileSubMeshChunk@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8B990, 1622 bytes, thiscall `ret 4`, returns bool, no EH frame.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK_FINDBONE
#define FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"

template <class T> inline void __fastcall GFExplicitNew(T* data, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&data[i]) T;
}

class C3DBone2 {                                   // Ego_r.pdb, 0x3c bytes (no shared header)
public:
    C3DBone2() : Parent(-1), OriginalNoChildren(0) { }
    long GetParent() const { return Parent; }
    long GetOriginalNoChildren() const { return OriginalNoChildren; }
    void SetOriginalNoChildren(long count) { OriginalNoChildren = count; }
private:
    unsigned long NameCRC;                         // +0x00
    long          Parent;                          // +0x04
    long          OriginalNoChildren;              // +0x08
    CMatrix3x4    LocalizationMatrix;              // +0x0c
};

class C3DMesh2 {                                   // retail layout: C3DMeshStats base 0x7c, then the Ego_r members less 8
public:
    virtual ~C3DMesh2();
private:
    unsigned char  _base_and_members[0x94];        // +0x04 .. +0x97 (C3DMeshStats, BoneNameMemoryHandle, PrimitiveCount, Primitives)
    unsigned long  BoneCount;                      // +0x98
    C3DBone2*      Bones;                          // +0x9c
    void GetUsedBones(CArray<bool>& used) const;                                   // 0x00A8B5F0
    bool PackBoneNames(const CArray<CCharString>& names);                          // 0x00A8B6C0
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap);          // 0x00A8B820
    void CalculateBoneKeyframes();                                                 // 0x00A8A9D0
    bool InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh);
};

bool C3DMesh2::InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh)
{
    CArray<CCountedPointer<C3DBone2> > bones;
    CArray<CCharString> bone_names;
    CCountedPointer<const C3DMeshFileBoneChunk> bone_chunk = sub_mesh.FindFirstBone();
    while (bone_chunk.Get()) {
        CCountedPointer<C3DBone2> bone;
        bone.Reset(new C3DBone2(bone_chunk->GetBone()));
        bone->SetOriginalNoChildren(0);
        if (bone->GetParent() != -1)
            bones[bone->GetParent()]->SetOriginalNoChildren(bones[bone->GetParent()]->GetOriginalNoChildren() + 1);
        bones.push_back(bone);
        bone_names.push_back(bone_chunk->GetName());
        bone_chunk = sub_mesh.FindNextBone(bone_chunk);
    }

    CArray<bool> used(bones.size(), false);
    GetUsedBones(used);
    for (long i = used.size() - 1; i >= 0; --i)
        if (used[i] && bones[i]->GetParent() != -1)
            used[bones[i]->GetParent()] = true;

    BoneCount = 0;
    for (unsigned long i = 0; i < used.size(); ++i)
        if (used[i])
            ++BoneCount;
    GFExplicitNew(Bones, BoneCount);

    CArray<CCharString> used_bone_names(BoneCount);
    std::map<unsigned long, unsigned long> bone_remap;
    unsigned long new_index = 0;
    for (unsigned long j = 0; j < used.size(); ++j) {
        if (used[j]) {
            Bones[new_index] = *bones[j];
            used_bone_names[new_index] = bone_names[j];
            bone_remap[j] = new_index;
            ++new_index;
        }
    }
    RemapBoneIndices(bone_remap);
    if (!PackBoneNames(used_bone_names))
        return false;
    CalculateBoneKeyframes();
    return true;
}
