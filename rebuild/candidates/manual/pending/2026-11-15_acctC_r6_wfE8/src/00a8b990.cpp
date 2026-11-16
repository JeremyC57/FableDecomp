// C3DMesh2::InitialiseBones(const C3DMeshFileSubMeshChunk& sub_mesh) -- read the sub-mesh's bone chunks into counted bones (counting each parent's children) and names, keep only the bones the mesh uses (and their ancestors), pack them into the mesh's bone array with their names, remap every bone index, then build the bone keyframes; false when the names do not pack
// Same-TU callees visible: PackBoneNames 0x00A8B6C0 and InitialiseBoneNamePointers 0x00A892E0 defined above the row with their landed texts (each RELOCATION_MATCHes its own retail address in this TU); c2 then sees the used-names array is not captured -- this closed the whole bone-loop register residue of chunk 864 / wfE8 (EVIDENCED: retail holds the array begin in edi across PackBoneNames, 0x00A8BCCF..0x00A8BE0D).
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
#define FABLE_RETAIL_C3DMESH2_BONENAMEPOINTERS_DECL
#define FABLE_RETAIL_C3DMESHSTATS_PROTECTED_POOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CMemoryDataOutputStream.h"
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)

template <class T> inline void FABLE_FASTCALL GFExplicitNew(T* data, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&data[i]) T;
}

// Same-TU callees (landed 0x00A892E0 C3DMesh2::InitialiseBoneNamePointers and 0x00A8B6C0 C3DMesh2::PackBoneNames, their texts on the
// shared headers), defined above the row as in retail's TU (Ego_r donor order): c2 sees that PackBoneNames does not capture the
// used-names array, so the row keeps its begin in a CPU register across the remap loop and the PackBoneNames call (retail edi,
// 0x00A8BCCF .. 0x00A8BE0D, never reloaded after the calls) and strength-reduces &used_bone_names[new_index] (0x00A8BD0F).
void C3DMesh2::InitialiseBoneNamePointers(unsigned long size)
{
    if (size) {
        unsigned short* indices = (unsigned short*)MemoryPool->GetPointerFromHandle(BoneNameMemoryHandle);
        BoneNameIndices = indices;
        BoneNameData = (char*)(indices + BoneCount);
    }
}

bool C3DMesh2::PackBoneNames(const CArray<CCharString>& names)
{
    unsigned long count = names.size();
    if (count == 0)
        return true;
    CArray<unsigned short> indices(count);
    CMemoryDataOutputStream stream;
    for (unsigned long i = 0; i < count; ++i) {
        indices[i] = (unsigned short)stream.GetPosition();
        stream.WriteNullTerminatedString(names[i]);
    }
    unsigned long name_bytes = stream.GetLength();
    unsigned long index_bytes = indices.size() * sizeof(unsigned short);
    BoneNameDataSize = name_bytes;
    if (!MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, index_bytes + name_bytes, 0))
        return false;
    InitialiseBoneNamePointers(index_bytes + name_bytes);
    memcpy(BoneNameIndices, indices.GetAsCArray(), index_bytes);
    memcpy(BoneNameData, stream.PeekData(), BoneNameDataSize);
    return true;
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
