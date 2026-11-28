// 00aa0c50 C3DAnimationManager::CombineBaseAnimationsBoneList
// Leaf: ?CombineBaseAnimationsBoneList@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@PAVC3DKeyframe@@KKABVC3DMesh2@@ABV?$CArray@K@@@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_ANIMATION_BONELIST_TRANSITION_PASS30
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
#define FABLE_RETAIL_C3DANIMATION2_CALCLOCALBONE
#define FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE
#define FABLE_RETAIL_C3DANIMATIONSTATE_ACCESSORS
#define FABLE_RETAIL_C3DANIMATIONSTATE_ORIGINAL_CTORS_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM
#define FABLE_RETAIL_C3DANIMATIONINFO_GETANIMATION
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#include "engine/C3DKeyframe.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DAnimationTransitionState.h"
#include "engine/C3DAnimationManager.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CMeshDataBank.h"

void C3DAnimationManager::CombineBaseAnimationsBoneList(const C3DAnimationStateBase& state, C3DKeyframe* out, unsigned long count,
                                                        unsigned long depth, const C3DMesh2& mesh, const CArray<unsigned long>& bones)
{
    long type = state.GetType();
    if (type == 0) {
        const C3DAnimationState& single = static_cast<const C3DAnimationState&>(state);
        long index = single.GetAnimationIndex();
        const C3DAnimationInfo* info = MeshBank->PeekAnimation(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        float time = single.GetAnimationTime();
        if (animation->IsRelative()) {
            for (unsigned long i = 0; i < count; ++i)
                animation->CalcLocalBoneTransformationRelative(out[i], bones[i], time, mesh);
        }
        else {
            for (unsigned long i = 0; i < count; ++i)
                animation->CalcLocalBoneTransformation(out[i], bones[i], time);
        }
    }
    else if (type == 2) {
        const C3DAnimationTransitionState& transition = static_cast<const C3DAnimationTransitionState&>(state);
        unsigned long childDepth = depth + 1;
        CombineBaseAnimationsBoneList(transition.PeekStartState(), out, count, childDepth, mesh, bones);
        if (childDepth < 5) {
            C3DKeyframe* scratch = out + count;
            CombineBaseAnimationsBoneList(transition.PeekEndState(), scratch, count, childDepth, mesh, bones);
            BlendAnimations(out, scratch, count, transition.GetProgress());
        }
    }
}