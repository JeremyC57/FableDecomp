// Leaf: ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@ABV?$CArray@V?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@@@AAV?$CArray@V?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@@@Z
// Flags: /Gr /d2inlT64
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_ANIMATION_BUILDER_LONG_DISTANCE_PASS30
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "engine/C3DAnimationBuilder.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "fable_assert.h"

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& anim, CArray<CCountedPointer<CBoneInfo> >& bone_list)
{
    CArray<CCountedPointer<CBoneInfo> > root_bones;
    bone_list.clear();
    for (unsigned long c_bone = 0; c_bone < anim.size(); ++c_bone) {
        CCountedPointer<CBoneInfo> bone_info(new CBoneInfo);
        const C3DAnimFileSequenceChunk& bone_anim = *anim[c_bone];
        bone_info->Name = bone_anim.GetBoneName();
        bone_info->Index = c_bone;
        bone_info->OriginalNoChildren = 0;
        long parent_index = bone_anim.GetParentIndex();
        if (parent_index >= 0) {
            FABLE_RETAIL_ASSERT(parent_index < bone_list.size());
            CBoneInfo& parent = *bone_list[parent_index];
            parent.Children.push_back(bone_info);
            ++parent.OriginalNoChildren;
        } else {
            root_bones.push_back(bone_info);
        }
        bone_list.push_back(bone_info);
    }
    FABLE_RETAIL_ASSERT(root_bones.size() == 1);
    return root_bones[0];
}
