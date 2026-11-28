// C3DAnimationBuilder::BuildBoneInfoTree, sequence array overload, retail 00AB1990 (812 bytes).
// Inline-budget: T72 retains the child and root growth calls and expands bone_list growth at 00AB1B86.
// Leaf: ?BuildBoneInfoTree@C3DAnimationBuilder@@AAE?AV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PBVC3DAnimationSequence@@K@Z
// Flags: /Gr /d2inlT72
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_ANIMATION_BONE_TREE_PASS30
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "engine/C3DAnimationBuilder.h"
#include "engine/C3DAnimationSequence.h"
#include "fable_assert.h"

CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const C3DAnimationSequence* anim, unsigned long size)
{
    CArray<CCountedPointer<CBoneInfo> > root_bones;
    CArray<CCountedPointer<CBoneInfo> > bone_list;
    for (unsigned long c_bone = 0; c_bone < size; ++c_bone) {
        CCountedPointer<CBoneInfo> bone_info(new CBoneInfo);
        const C3DAnimationSequence& bone_anim = anim[c_bone];
        bone_info->Name = bone_anim.GetBoneName();
        bone_info->Index = c_bone;
        bone_info->OriginalNoChildren = 0;
        long parent_index = bone_anim.GetParentIndex();
        if (parent_index != -1) {
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