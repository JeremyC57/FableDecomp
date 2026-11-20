// NUISystem::CChangingStateComponent::IsPositionIndependent() const -- independent when the CComponent base says so or when the
//   target state's change flags carry bit 0x20; with no target state the base answer is returned as is
// Leaf: ?IsPositionIndependent@CChangingStateComponent@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C870, 56 bytes (virtual; reached only through the vtables).
// Identity: Ego_r.pdb S_PUB32 + FableWin.pdb ?IsPositionIndependent@CChangingStateComponent@NUISystem@@UBE_NXZ (engine/
//   NUISystem_CChangingStateComponent.h); the base it calls non-virtually is ?IsPositionIndependent@CComponent@NUISystem@@UBE_NXZ
//   0x0052F1A0 (the landed self row; its body calls slot 101, IsIndependent), the state is the const GetState(unsigned long) (slot
//   134, +0x218) of TargetState (+0x148), and the bit is tested in CUIState::StateChangeFlag (+0).
// Naming (NF156, 2026-11-20, q5084): moved off the row-local CComponent / CChangingStateComponent / SStateInfo models (the global
//   non-virtual ?IsPositionIndependent@CChangingStateComponent@@QAE_NXZ, binding 0x0052F1A0 as
//   ?IsIndependentByDefault@CChangingStateComponent@@QAE_NXZ) onto the shared headers.
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIState.h"
namespace NUISystem {
bool CChangingStateComponent::IsPositionIndependent() const
{
    bool independent = CComponent::IsPositionIndependent();
    const CUIState* state = GetState(TargetState);
    if (state != 0) {
        if (independent || (state->StateChangeFlag & 0x20))
            return true;
        return false;
    }
    return independent;
}
}
