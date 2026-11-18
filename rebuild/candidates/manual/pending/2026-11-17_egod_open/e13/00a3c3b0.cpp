// C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState& anim1, const C3DAnimationBlendState& anim2, float weight2) -- make this blend state the interpolation of two others: own copy of the shared data, the interpolated base animation, the combination blends resized to anim1's count with each one's animation index copied from anim1 and its time and alpha weight lerped by weight2, then anim1's skeletal morph if it has one
// Grade: DIFFER(777v777) -- e13: 9 hunks, x87-order: in the 4x-unrolled blend loop four integer loads are scheduled before the lerp's `fstp st(0)` (retail after: 0x00A3C4FC mov esi,[esi+4]; 0x00A3C54F mov ecx,[ecx+0Ch]; 0x00A3C583 add eax,50h; 0x00A3C5A5 mov ecx,[ebx+4])
// Leaf: ?SetInterpolate@C3DAnimationBlendState@@QAEXABV1@0M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3C3B0, 777 bytes, thiscall `ret 0Ch`.
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (retail calls vector<C3DAnimationStateAlpha>::resize 0x00A3D500, landed).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_C3DANIMATIONSTATE
#define FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS
#define FABLE_RETAIL_C3DANIMATIONSTATE_DEFAULT
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA
#define FABLE_RETAIL_C3DANIMATIONSTATEALPHA_CTOR
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CREATEINTERPOLATE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_ACCESSORS
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SKELETALMORPH
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SETINTERPOLATE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DAnimationStateAlpha.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/GFGetInterpolatedValue.h"

void C3DAnimationBlendState::SetInterpolate(const C3DAnimationBlendState& anim1, const C3DAnimationBlendState& anim2, float weight2)
{
    MakeOwnCopyOfData();
    Data->BaseAnimation = CreateInterpolate(anim1.PeekBaseAnimation(), anim2.PeekBaseAnimation(), weight2);
    unsigned long no_blends = anim1.GetCombinationBlendCount();
    Data->CombinationBlends.resize(no_blends, C3DAnimationStateAlpha());
    for (unsigned long c_blend = 0; c_blend < no_blends; c_blend++) {
        C3DAnimationStateAlpha& dest = Data->CombinationBlends[c_blend];
        const C3DAnimationStateAlpha& blend1 = anim1.PeekCombinationBlend(c_blend);
        const C3DAnimationStateAlpha& blend2 = anim2.PeekCombinationBlend(c_blend);
        dest.AnimationIndex = blend1.AnimationIndex;
        dest.AnimationTime = GFGetInterpolatedValue(blend1.AnimationTime, blend2.AnimationTime, weight2);
        dest.AlphaWeight = GFGetInterpolatedValue(blend1.AlphaWeight, blend2.AlphaWeight, weight2);
    }
    if (!anim1.Data->SkeletalMorph.IsNull())
        Data->SkeletalMorph = anim1.Data->SkeletalMorph;
}
