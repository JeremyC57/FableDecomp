// Leaf: ?InitialiseFromMeshFileData@CAnimatedBlock@N3DPrimitive@@QAEXABVC3DMeshFileXAnimatedPrimitiveBlockChunk@@_N@Z
// Flags:
// Inline-budget: header opt-in FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27 (stlport/fable_containers.h _MSC_VER-guarded __forceinline __copy_trivial and
//   __copy_ptrs<__true_type>) -- the in-TU vector<unsigned long>::erase copy (owner 0x0046496F, 52 B) expands STLport's POD copy; with plain inline
//   it is a 34-B copy unequal to retail. Flags tried (landing p27): /d2inlT100, /d2inlT300, /Ob2 keep the 34-B copy; /d2inlS50 / S150 / S300
//   give the 52-B erase but then clear 0x00451046, resize 0x00464931 and ~CVertexGroupBlock 0x00A96DA0 differ (S100 breaks the leaf).
#define FABLE_RETAIL_ANIMATEDPRIMITIVEBLOCK_ACCESSORS_PASS27
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CANIMATEDBLOCK_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ORIGINAL_CASTTOBASE_PASS22
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DINDEXEDBONEWEIGHT_CTOR_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27
#include <string.h>
#include "fable_assert.h"
#include "engine/EVertexType.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveBlockChunk.h"

// 00A96DA0 is the emitted copy of the debug donor's inline group-block destructor.
inline CVertexGroupBlock::~CVertexGroupBlock() {}

void N3DPrimitive::CAnimatedBlock::InitialiseFromMeshFileData(const C3DMeshFileXAnimatedPrimitiveBlockChunk& file_data, bool vb_built)
{
    CVertexGroupBlock group_block = file_data.GetGroupBlock();
    GroupCount = (unsigned char)group_block.GetGroupCount();
    FABLE_RETAIL_ASSERT(GroupCount <= 18);
    memset(Groups, 0, sizeof(Groups));
    for (unsigned long c_group = 0; c_group < GroupCount; ++c_group)
        Groups[c_group] = (unsigned char)group_block.GetGroup(c_group);

    VertexCount = file_data.GetVertexCount();
    BonesPerVertex = (unsigned short)file_data.GetBonesPerVertex();
    PalettedFlag = true;

    if (!vb_built)
    {
        Data.Reset(new CDeviceIndependent);
        Data->VertexList.resize(VertexCount);
        for (unsigned long c_vertex = 0; c_vertex < VertexCount; ++c_vertex)
            Data->VertexList[c_vertex] = file_data.GetVertex(c_vertex);

        Data->BoneWeights.resize(VertexCount * BonesPerVertex);
        for (unsigned long c_vertex = 0; c_vertex < VertexCount; ++c_vertex)
            for (unsigned long c_bone = 0; c_bone < BonesPerVertex; ++c_bone)
                Data->BoneWeights[BonesPerVertex * c_vertex + c_bone] =
                    file_data.GetBoneWeight(c_vertex, c_bone);
    }
}

// VC7.1 emits the pending STLport copies after the leaf. The owner bodies of
// _M_erase (004507A1), clear (00451046), erase (0046496F) and resize
// (00464931) use size optimization. This late setting preserves the leaf's
// /O2 schedule and matches those four owners. The erase owner expands both
// levels of the POD copy dispatch, enabled above before the container header.
#pragma optimize("s", on)