// C3DAnimationBuilder::BuildMeshAnimation, retail 00AB2520.
// Leaf: ?BuildMeshAnimation@C3DAnimationBuilder@@QAEKAAVCOutputAnimation@N3DAnimationBuilder@@PBVC3DAnimationSequence@@KABVC3DMesh2@@_NAAVCSimpleMemoryPool@@@Z
// Flags: /Gr
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_MESH_ANIMATION_TREE_PASS30
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_ANIMATION_BUILDER_LONG_DISTANCE_PASS30
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DETERMINANT
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/C3DAnimationBuilder.h"
#include "engine/N3DAnimationBuilder_COutputAnimation.h"

unsigned long C3DAnimationBuilder::BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation& dest_anim,
    const C3DAnimationSequence* src_anim, unsigned long src_size, const C3DMesh2& mesh, bool delta, CSimpleMemoryPool& alloc)
{
    dest_anim.GeneratedBoneCount = 0;
    CArray<CCountedPointer<CBoneInfo> > mesh_bone_list;
    CCountedPointer<CBoneInfo> mesh_tree = BuildBoneInfoTree(mesh, mesh_bone_list);
    CCountedPointer<CBoneInfo> anim_tree = BuildBoneInfoTree(src_anim, src_size);
    if (mesh_tree.IsNull() || anim_tree.IsNull())
        return 0;
    unsigned long size = mesh_bone_list.size();
    unsigned long matches = AssignAnimation(*mesh_tree, anim_tree, true);
    dest_anim.Sequences.resize(size);
    for (unsigned long c_bone = 0; c_bone < size; ++c_bone) {
        const CBoneInfo& mesh_bone = *mesh_bone_list[c_bone];
        const C3DBone2& bone = *mesh.GetBone(c_bone);
        N3DAnimationBuilder::CSequenceInfo& output_seq = dest_anim.Sequences[c_bone];
        if (mesh_bone.AnimSource.Get()) {
            output_seq.OriginalIndex = mesh_bone.AnimSource->Index;
        } else {
            if (mesh_bone.ExtensionChild || delta) {
                output_seq.GeneratedTransform.SetIdentity();
            } else {
                output_seq.GeneratedTransform.SetInverse(bone.PeekLocalizationMatrix());
                long parent_index = bone.GetParent();
                if (parent_index != -1)
                    output_seq.GeneratedTransform.PostMultiply(mesh.GetBone(parent_index)->PeekLocalizationMatrix());
            }
            output_seq.OriginalIndex = -1;
            ++dest_anim.GeneratedBoneCount;
        }
    }
    return matches;
}