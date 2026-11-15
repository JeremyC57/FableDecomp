// C3DMesh2::InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh) -- read the sub-mesh's bone chunks into counted bones (counting each parent's children) and names, keep only the bones the mesh uses (and their ancestors), pack them into the mesh's bone array with their names, remap every bone index, then build the bone keyframes; false when the names do not pack
// Grade: DIFFER(1608v1624) -- wfE8: ported to shared headers (engine/C3DMesh2.h + new INITIALISEBONES opt-in, engine/C3DBone2.h + new CTOR opt-in, direct OriginalNoChildren / Parent member access as retail 0x00A8BA20 / 0x00A8BA36). Residue unchanged from chunk 864: bone loop register allocation -- retail keeps bones._M_finish in edi and _M_end_of_storage in ebp across the loop and spills bone.Info to [esp+34h]; ours keeps Info in edi and _M_finish in memory [esp+40h], so every later frame slot is 0xC off; all calls / blocks / release shapes equal.
// Leaf: ?InitialiseBones@C3DMesh2@@AAE_NABVC3DMeshFileSubMeshChunk@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8B990, 1624 bytes, thiscall `ret 4`, returns bool, no EH frame.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK_FINDBONE
#define FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESH2_INITIALISEBONES
#define FABLE_RETAIL_C3DBONE2_CTOR
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"

template <class T> inline void __fastcall GFExplicitNew(T* data, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&data[i]) T;
}

bool C3DMesh2::InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh)
{
    CArray<CCountedPointer<C3DBone2> > bones;
    CArray<CCharString> bone_names;
    CCountedPointer<const C3DMeshFileBoneChunk> bone_chunk = sub_mesh.FindFirstBone();
    while (bone_chunk.Get()) {
        CCountedPointer<C3DBone2> bone;
        bone.Reset(new C3DBone2(bone_chunk->GetBone()));
        bone->OriginalNoChildren = 0;
        if (bone->Parent != -1)
            ++bones[bone->Parent]->OriginalNoChildren;
        bones.push_back(bone);
        bone_names.push_back(bone_chunk->GetName());
        bone_chunk = sub_mesh.FindNextBone(bone_chunk);
    }

    CArray<bool> used(bones.size(), false);
    GetUsedBones(used);
    for (long i = used.size() - 1; i >= 0; --i)
        if (used[i] && bones[i]->Parent != -1)
            used[bones[i]->Parent] = true;

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
