// 00ab2160 C3DAnimationBuilder::GetIntersection
// Leaf: ?GetIntersection@C3DAnimationBuilder@@QAEKAAV?$CArray@V?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@@@0@Z
// Flags: /O2 /Oy /Gr
#define FABLE_RETAIL_BUILDER_INTERSECTION_PASS29
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CARRAY_FOLD_ORIGINAL_AT_PASS27
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#include "fable_assert.h"
#include "engine/C3DAnimationBuilder.h"
#include "engine/C3DAnimFileSequenceChunk.h"

inline CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(
    const CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& sequences, CArray<CCountedPointer<CBoneInfo> >& bones)
{
    CArray<CCountedPointer<CBoneInfo> > roots;
    bones.clear();
    for (unsigned long i = 0; i < sequences.size(); ++i) {
        CCountedPointer<CBoneInfo> bone(new CBoneInfo);
        const C3DAnimFileSequenceChunk* sequence = sequences[i].Get();
        bone->Name = sequence->GetBoneName();
        bone->Index = i;
        bone->OriginalNoChildren = 0;
        long parent = sequence->GetParentIndex();
        if (parent >= 0) {
            CBoneInfo* parentBone = bones[parent].Get();
            parentBone->Children.push_back(bone);
            parentBone->OriginalNoChildren++;
        }
        else
            roots.push_back(bone);
        bones.push_back(bone);
    }
    return roots[0];
}

unsigned long C3DAnimationBuilder::GetIntersection(CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& anim1,CArray<CCountedPointer<C3DAnimFileSequenceChunk> >& anim2)
{
    CArray<CCountedPointer<C3DAnimFileSequenceChunk> > new_anim1,new_anim2;
    CArray<CCountedPointer<CBoneInfo> > bone_list1,bone_list2;
    CCountedPointer<CBoneInfo> tree1=BuildBoneInfoTree(anim1,bone_list1);
    CCountedPointer<CBoneInfo> tree2=BuildBoneInfoTree(anim2,bone_list2);
    unsigned long matches=AssignAnimation(*tree1,tree2,true);
    CArray<long> new_indices(anim1.size(),-1);
    for(unsigned long c_bone=0;c_bone<anim1.size();++c_bone) {
        CBoneInfo& bone1=*bone_list1.At(c_bone);
        bool extension=bone1.ExtensionChild||bone1.ExtensionParent;
        if(bone1.AnimSource.Get()||extension) {
            CCountedPointer<C3DAnimFileSequenceChunk> sequence1=anim1[c_bone];
            long parent_index=sequence1->GetParentIndex();
            if(parent_index!=-1) {
                parent_index=new_indices.At(parent_index);
                FABLE_RETAIL_ASSERT(parent_index>=0);
                sequence1->SetParentIndex(parent_index);
            }
            sequence1->SetParentIndex(parent_index);
            new_indices[c_bone]=new_anim1.size();
            new_anim1.push_back(sequence1);
            if(extension) {
                CCountedPointer<C3DAnimFileSequenceChunk> null_sequence;
                new_anim2.push_back(null_sequence);
            } else {
                CBoneInfo& bone2=*bone1.AnimSource;
                CCountedPointer<C3DAnimFileSequenceChunk> sequence2=anim2[bone2.Index];
                new_anim2.push_back(sequence2);
            }
        }
    }
    anim1=new_anim1;
    anim2=new_anim2;
    return matches;
}