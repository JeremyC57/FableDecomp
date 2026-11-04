// C3DAnimationTransitionState::FindFirstAnimation() const -- the first plain animation state of a transition: that of its start state
// Leaf: ?FindFirstAnimation@C3DAnimationTransitionState@@UBEABVC3DAnimationState@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00710530, 8 bytes: `mov ecx,[ecx+8] ; mov eax,[ecx] ; jmp [eax+0Ch]` (StartState's slot 3, tail call).
// Identity (two sources): C3DAnimationTransitionState vtable 0x01265144 slot 3, between GetType 0x00710520 and FindDominantAnimation
//   0x00710540 (which picks StartState or EndState by Progress and calls the same slot 3 on it); Ego_r.pdb
//   ?FindFirstAnimation@C3DAnimationTransitionState@@UBEABVC3DAnimationState@@XZ 0x00578CB0, followed by FindDominantAnimation 0x00578CC0
//   (FableWin.pdb has the same spellings); Ego_r StartState +0x08. The catalog's ctor label was a propagated placeholder.
// Callees: virtual C3DAnimationStateBase::FindFirstAnimation (slot 3).
// Header: engine/C3DAnimationTransitionState.h (FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE), engine/C3DAnimationState.h
//   (FABLE_RETAIL_C3DANIMATIONSTATEBASE_VIRTUALS).
#define FABLE_RETAIL_C3DANIMATIONTRANSITIONSTATE
#include "engine/C3DAnimationTransitionState.h"
const C3DAnimationState& C3DAnimationTransitionState::FindFirstAnimation() const
{
    return StartState->FindFirstAnimation();
}