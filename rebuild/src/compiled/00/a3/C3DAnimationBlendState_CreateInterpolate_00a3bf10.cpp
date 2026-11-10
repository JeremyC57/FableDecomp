// C3DAnimationBlendState::CreateInterpolate(const C3DAnimationStateBase& a, const C3DAnimationStateBase& b, float t) -- build a new animation state between a and b at t: a transition state recursively interpolates its start and end states and its progress; a plain state copies the animation index and lerps the time when both indices agree
// Leaf: ?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@ABVC3DAnimationStateBase@@0M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BF10, 432 bytes, private static /Gr __fastcall (hidden return in ecx,
//   a in edx; b and t on the stack), `ret 8`.
// Identity (proven): Ego_r.pdb ?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@
//   ABVC3DAnimationStateBase@@0M@Z (0x00B96750, next to SetBaseAnimation / AddCombinationBlend as retail 0x00A3C260..);
//   callers are itself and C3DAnimationBlendState::SetInterpolate 0x00A3C3B0; the two arms build C3DAnimationTransitionState
//   (vtable 0x01265144, 0x14 bytes: StartState +8, EndState +0xC, Progress +0x10) and C3DAnimationState (vtable 0x01249F50,
//   0x10 bytes: AnimationIndex +8 = -1, AnimationTime +0xC), the Ego_r layouts.
// Callees: operator new 0x00BFEA1A, the state's slot 2 (GetType; 2 = transition), itself, the pointee's slot 1 (Delete).
// Layout: shared engine/CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE polymorphic base, Delete slot 1), CIVCountedPointer.h
//   (retail branch: adopting raw ctor, copy ctor, operator=, RELEASE_INNER; new FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT: the
//   converting ctor from CIVCountedPointer<U>, InternalGetObject, operator*), C3DAnimationState.h (GetType slot 2; new _DEFAULT ctor:
//   AnimationIndex -1), C3DAnimationTransitionState.h (new _INTERPOLATE: inline default ctor + friend C3DAnimationBlendState) and
//   C3DAnimationBlendState.h (new _CREATEINTERPOLATE declaration).
// Shape: the lerp keeps its start on the x87 stack (`fld a ; fld b ; fsub st(1) ; fmul t ; fadd st(1)`); the whole transition lerp
//   RHS is parenthesised (`(start + (tb.Progress - start) * t)`), a PAREN no-code tuple that moves the scheduling region cut so the
//   kept progress pops (`fstp st(0)`) right after `mov ecx,[esi+4]` as retail.
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
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE_INTERPOLATE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_CREATEINTERPOLATE
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/C3DAnimationState.h"
#include "engine/C3DAnimationTransitionState.h"
#include "engine/C3DAnimationBlendState.h"

CIVCountedPointer<C3DAnimationStateBase> __fastcall C3DAnimationBlendState::CreateInterpolate(const C3DAnimationStateBase& a,
                                                                                              const C3DAnimationStateBase& b,
                                                                                              float t)
{
    if (a.GetType() == 2) {
        const C3DAnimationTransitionState& ta = static_cast<const C3DAnimationTransitionState&>(a);
        const C3DAnimationTransitionState& tb = static_cast<const C3DAnimationTransitionState&>(b);
        CIVCountedPointer<C3DAnimationTransitionState> trans(new C3DAnimationTransitionState);
        trans->StartState = CreateInterpolate(*ta.StartState, *tb.StartState, t);
        trans->EndState = CreateInterpolate(*ta.EndState, *tb.EndState, t);
        const float start = ta.Progress;
        trans->Progress = (start + (tb.Progress - start) * t);
        return trans;
    }
    const C3DAnimationState& sa = static_cast<const C3DAnimationState&>(a);
    const C3DAnimationState& sb = static_cast<const C3DAnimationState&>(b);
    CIVCountedPointer<C3DAnimationState> state(new C3DAnimationState);
    if (sa.AnimationIndex == sb.AnimationIndex) {
        state->AnimationIndex = sa.AnimationIndex;
        const float start = sa.AnimationTime;
        state->AnimationTime = start + (sb.AnimationTime - start) * t;
    }
    return state;
}