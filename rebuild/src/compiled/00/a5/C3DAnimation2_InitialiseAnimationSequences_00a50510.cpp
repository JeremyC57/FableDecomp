// C3DAnimation2::InitialiseAnimationSequences(C3DAnimation2& shared_animation, const C3DMesh2& mesh, bool create_rigid_bones, unsigned long& build_result) -- build an animation instance's sequences for a mesh from the shared animation: the builder's output gives the sequence list (shared or generated-constant data per bone), then per bone the inverse localization matrix (times the parent's) gives the rigid-bone flags and positions, and the bone mask is copied through the original sequence index
// Leaf: ?InitialiseAnimationSequences@C3DAnimation2@@AAE_NAAV1@ABVC3DMesh2@@_NAAK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A50510, 1748 bytes, thiscall, `ret 10h`, frame 0xB0, no EH frame. Sole caller
//   C3DAnimation2::InitialiseAnimationInstance 0x00A50D1A.
// Identity: Ego_r.pdb / FableWin.pdb ?InitialiseAnimationSequences@C3DAnimation2@@AAE_NAAV1@ABVC3DMesh2@@_NAAK@Z (private; the four
//   pushed arguments and the bool result `mov al,1`). Parameter names are descriptive (the PDB name sets carry no names).
// Callees: C3DAnimationBuilder::C3DAnimationBuilder 0x00AB1660, C3DAnimationBuilder::BuildMeshAnimation 0x00AB2520,
//   CSimpleMemoryPool::New<C3DAnimationSequence> 0x00A533F0 / New<C3DAnimationSequenceData> 0x00A53440, CSimpleMemoryPool::Allocate
//   0x00A5C910 (inside the inline AllocateType<bool> / <C3DVector>), C3DAnimationSequenceData::InitialiseAsConstant 0x00A4FD70, malloc
//   0x00BFEA0E / free 0x00BFEA14 (STLport allocator of the local std::vector<bool>; free also for the output's vector), ~CCharString
//   0x0099EAE0 (the builder's AddedBonePrefixes), ~CBaseClassNonCopyable 0x0099A300 (the builder's base; implicit dtor, no vtable store).
// Shape (each piece measured against retail, chunk 2014 NOTES.md):
//   - The accessors are the ones whose COMDATs this TU keeps in its inline run: GetType 0x00A4BB40, GetFrameCount 0x00A4BB50,
//     GetSamplesPerSecond 0x00A4BB60, SetParentIndex 0x00A4BC60, C3DBone2::GetParent / GetOriginalNoChildren / PeekLocalizationMatrix
//     0x00A4BD30 / 0x00A4BD40 / 0x00A4BD50, C3DMesh2::GetBoneCount / GetBone 0x00A4BD60 / 0x00A4BD70; C3DAnimationSequence::InitialiseShared
//     (COMDAT 0x00A4C140) is a header inline, C3DAnimationSequence::InitialiseAsConstant is an ordinary member of this TU (out-of-line
//     copy 0x00A4FE30, defined below as in the retail TU and expanded here by /O2).
//   - The rigid flags are a local std::vector<bool> (malloc / guarded `rep stosd ; rep stosb` fill / free, 0x00A50733..0x00A50774).
//   - The bone-mask copy is an if / else of two stores (a value ternary gives one store and moves the frequencies: loop heads then
//     get the alignment padding retail lacks).
//   - The x87 operand order of the SetInverse translation row and of the PostMultiply column-1 sums is c2's per-function order
//     (VC7.1 reassociates each sum by candidate order; MATCHING_PLAYBOOK): the named loop-1 bone, the separate `++data_index`, the
//     named `original_index` in loop 1, std::vector<bool> and the explicit C3DVector translation are the spellings that give retail's
//     order (each measured with the free c2 read-order metric, tools/e41fit.py / pmfit.py, then graded).
#include "fable_compiler.h"
#include <stddef.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_NEW
#define FABLE_RETAIL_C3DANIMATION2_INITSEQUENCES
#define FABLE_RETAIL_C3DANIMATIONSTATS_GETTYPE
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_INITIALISE
#define FABLE_RETAIL_C3DANIMATIONBUILDER_BASE
#define FABLE_RETAIL_COUTPUTANIMATION_CTOR
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#include "engine/C3DAnimation2.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/C3DAnimationBuilder.h"
#include "engine/N3DAnimationBuilder_COutputAnimation.h"

void C3DAnimationSequence::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned long frame_count, float samples_per_second, C3DAnimationSequenceData* data, CSimpleMemoryPool& memory_pool)
{
    Data = data;
    Shared = false;
    Data->InitialiseAsConstant(matrix, frame_count, samples_per_second, memory_pool);
}

bool C3DAnimation2::InitialiseAnimationSequences(C3DAnimation2& shared_animation, const C3DMesh2& mesh, bool create_rigid_bones, unsigned long& build_result)
{
    bool relative = (GetType() & ANIMTYPE_BASE_RELATIVE) != 0;
    C3DAnimationBuilder builder;
    N3DAnimationBuilder::COutputAnimation output;
    if (mesh.GetBoneCount() > 0)
        build_result = builder.BuildMeshAnimation(output, shared_animation.Sequences, shared_animation.SequenceCount, mesh, relative, *Allocator);
    SequenceCount = output.Sequences.size();
    Sequences = Allocator->New<C3DAnimationSequence>(SequenceCount);
    SequenceDataBufferSize = output.GeneratedBoneCount;
    SequenceDataBuffer = Allocator->New<C3DAnimationSequenceData>(SequenceDataBufferSize);
    unsigned long data_index = 0;
    for (unsigned long i = 0; i < SequenceCount; ++i)
    {
        const N3DAnimationBuilder::CSequenceInfo& info = output.Sequences[i];
        C3DAnimationSequence& sequence = Sequences[i];
        const C3DBone2* bone = mesh.GetBone(i);
        sequence.SetParentIndex(bone->GetParent());
        long original_index = info.OriginalIndex;
        if (original_index != -1)
            sequence.InitialiseShared(shared_animation.Sequences[original_index]);
        else
        {
            sequence.InitialiseAsConstant(info.GeneratedTransform, shared_animation.GetFrameCount(), shared_animation.GetSamplesPerSecond(), &SequenceDataBuffer[data_index], *Allocator);
            ++data_index;
        }
    }
    if (shared_animation.BoneMask)
        BoneMask = Allocator->AllocateType<bool>(SequenceCount);
    if (create_rigid_bones && !relative)
    {
        RigidBoneMask = Allocator->AllocateType<bool>(SequenceCount);
        RigidBonePositions = Allocator->AllocateType<C3DVector>(SequenceCount);
    }
    std::vector<bool> rigid(SequenceCount, false);
    for (unsigned long i = 0; i < SequenceCount; ++i)
    {
        const C3DBone2* bone = mesh.GetBone(i);
        CMatrix3x4 local;
        local.SetInverse(bone->PeekLocalizationMatrix());
        if (bone->GetParent() != -1)
        {
            const C3DBone2* parent = mesh.GetBone(bone->GetParent());
            local.PostMultiply(parent->PeekLocalizationMatrix());
            rigid[i] = rigid[bone->GetParent()] || parent->GetOriginalNoChildren() > 1;
        }
        if (RigidBoneMask)
        {
            RigidBoneMask[i] = rigid[i];
            RigidBonePositions[i] = C3DVector(local.E41, local.E42, local.E43);
        }
        if (BoneMask)
        {
            long original_index = output.Sequences[i].OriginalIndex;
            if (original_index != -1)
                BoneMask[i] = shared_animation.BoneMask[original_index];
            else
                BoneMask[i] = false;
        }
    }
    return true;
}