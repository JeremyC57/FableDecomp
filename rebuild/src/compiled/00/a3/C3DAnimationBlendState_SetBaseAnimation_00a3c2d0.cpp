// C3DAnimationBlendState::SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>& state) -- unshare the blend data, then make its base animation the given counted state (release the old one, add-ref the new one)
// Leaf: ?SetBaseAnimation@C3DAnimationBlendState@@QAEXABV?$CIVCountedPointer@VC3DAnimationStateBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3C2D0, 64 bytes, thiscall, `ret 4`.
// Identity (agentFAM06): Ego_r.pdb and FableWin.pdb ?SetBaseAnimation@C3DAnimationBlendState@@QAEXABV?$CIVCountedPointer@VC3DAnimationStateBase@@@@@Z
//   (the CIVCountedPointer overload; the C3DAnimationState& overload is 0x00A3C260, both calling the private MakeOwnCopyOfData
//   0x00A3C1A0 first); callers CAnimateThing::CalcInterpolatedMeshAnim 0x0070D940 (calls 0x0070DE21 / 0x0070DE6F) pass a
//   CIVCountedPointer<C3DAnimationStateBase>. The catalog label CIVCountedPointer_Assign is a placeholder.
// Callees: C3DAnimationBlendState::MakeOwnCopyOfData 0x00A3C1A0; the old state's Delete (CIVCountedPointeeBase slot 1).
// Body: shared C3DAnimationBlendState.h (TYPED_DATA + the new nested opt-in SHAREDDATA: Ego_r CSharedData, BaseAnimation +8) and
//   CIVCountedPointer.h (retail operator=: compare, release with its `mov [p],0`, re-read the reference, add-ref).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SHAREDDATA
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
#include "engine/C3DAnimationBlendState.h"

void C3DAnimationBlendState::SetBaseAnimation(const CIVCountedPointer<C3DAnimationStateBase>& state)
{
    MakeOwnCopyOfData();
    Data->BaseAnimation = state;
}
