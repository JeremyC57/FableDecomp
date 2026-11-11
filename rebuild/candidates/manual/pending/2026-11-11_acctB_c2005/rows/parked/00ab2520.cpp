// C3DAnimationBuilder::BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation& output, const C3DAnimationSequence* sequences, unsigned long sequence_count, const C3DMesh2& mesh, bool identity_generated_bones, CSimpleMemoryPool& memory_pool) -- map the animation's bones onto the mesh skeleton: one output sequence per mesh bone, either the matched animation sequence or a generated bone whose transform is the inverse of its localization matrix (times its parent's)
// Grade: DIFFER(1265v1265) -- x87 operand/term order only (SetInverse E22 at 0x00AB2698, E41 sum, PostMultiply term order 0x00AB2773..0x00AB28C2); all integer code, frame and calls match
// Leaf: ?BuildMeshAnimation@C3DAnimationBuilder@@QAEKAAVCOutputAnimation@N3DAnimationBuilder@@PBVC3DAnimationSequence@@KABVC3DMesh2@@_NAAVCSimpleMemoryPool@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#include "engine/C3DAnimationBuilder.h"
#include "engine/N3DAnimationBuilder_COutputAnimation.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"

// Same-TU body of the mesh overload (retail 0x00AB1CC0, compiled in this TU ahead of its caller): VC7.1 then knows it does not
// keep the address of the result it builds, so the caller's mesh_root stays out of memory as in retail (0x00AB294A `mov esi,[esp+24h]`
// ... `dec dword ptr [esi] ; jne`, no zeroing stores).
#ifdef _MSC_VER
__declspec(noinline)
#endif
CCountedPointer<C3DAnimationBuilder::CBoneInfo> C3DAnimationBuilder::BuildBoneInfoTree(const C3DMesh2& mesh, CArray<CCountedPointer<CBoneInfo> >& bones)
{
    CArray<CCountedPointer<CBoneInfo> > roots;
    bones.clear();
    for (unsigned long i = 0; i < mesh.BoneCount; ++i)
    {
        CCountedPointer<CBoneInfo> bone(new CBoneInfo);
        const C3DBone2& mesh_bone = mesh.Bones[i];
        bone->Name = static_cast<const CCharString&>(mesh.BoneNameData + mesh.BoneNameIndices[i]);
        bone->Index = i;
        bone->OriginalNoChildren = mesh_bone.OriginalNoChildren;
        long parent = mesh_bone.Parent;
        if (parent >= 0)
            bones[parent]->Children.push_back(bone);
        else
            roots.push_back(bone);
        bones.push_back(bone);
    }
    if (roots.size() == 0)
        return CCountedPointer<CBoneInfo>();
    return roots[0];
}

unsigned long C3DAnimationBuilder::BuildMeshAnimation(N3DAnimationBuilder::COutputAnimation& output, const C3DAnimationSequence* sequences,
                                                      unsigned long sequence_count, const C3DMesh2& mesh, bool identity_generated_bones,
                                                      CSimpleMemoryPool& memory_pool)
{
    output.GeneratedBoneCount = 0;
    CArray<CCountedPointer<CBoneInfo> > mesh_bones;
    CCountedPointer<CBoneInfo> mesh_root = BuildBoneInfoTree(mesh, mesh_bones);
    CCountedPointer<CBoneInfo> anim_root = BuildBoneInfoTree(sequences, sequence_count);
    if (mesh_root.Get() == 0 || anim_root.Get() == 0)
        return 0;
    unsigned long bone_count = mesh_bones.size();
    unsigned long result = AssignAnimation(*mesh_root, anim_root, true);
    output.Sequences.resize(bone_count);
    for (unsigned long i = 0; i < bone_count; ++i)
    {
        CBoneInfo* bone_info = mesh_bones[i].Get();
        const C3DBone2& bone = mesh.Bones[i];
        N3DAnimationBuilder::CSequenceInfo& sequence = output.Sequences[i];
        if (bone_info->AnimSource.Get())
        {
            sequence.OriginalIndex = bone_info->AnimSource->Index;
        }
        else
        {
            if (bone_info->ExtensionChild || identity_generated_bones)
            {
                sequence.GeneratedTransform.SetIdentity();
            }
            else
            {
                sequence.GeneratedTransform.SetInverse(bone.LocalizationMatrix);
                if (bone.Parent != -1)
                    sequence.GeneratedTransform.PostMultiply(mesh.Bones[bone.Parent].LocalizationMatrix);
            }
            sequence.OriginalIndex = -1;
            ++output.GeneratedBoneCount;
        }
    }
    return result;
}
