// CPlayer::UninitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input) -- leaving the charge/quick-access mode detaches the five input processes InitModeChargeQuickAccess attached, then removes player mode 0x1C
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634470, length 71 bytes.
// Identity: catalog label CPlayer::UninitModeChargeQuickAccess (the Init twin is Ego_r.pdb
// `?InitModeChargeQuickAccess@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z`, 0x00633700 here).
// Callees: CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (non-virtual, five times),
// CPlayer::RemoveMode(EPlayerMode) @ 0x00634190.
// Shape: engine/CPlayer.h members (InputProcessQuickAccessItems_ptr +0x160,
// InputProcessControlCreatureRightStick_ptr +0x16C, InputProcessRightStickLookAround_ptr +0x170,
// InputProcessCycleSpecialCameraModes_ptr +0x174, InputProcessDebugControls_ptr +0x15C) in the
// Init order; the final RemoveMode is a plain call (`push 1Ch ; mov ecx,esi ; call ; ret 8`).
// agentNF16: the shared-header retail CPlayer (FABLE_RETAIL_PLAYER: : CInitBaseClass, same offsets) declares the private
//   ?UninitModeChargeQuickAccess@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z (Ego_r / FableWin) and, under
//   FABLE_RETAIL_PLAYER_REMOVEMODE, ?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z (0x00634190). Was the derived stand-in CPlayer_Methods
//   with a long-typed RemoveMode.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
class CProcessedInput;

struct CAInputProcessCalls {
    void RemoveFromInterface();                       // 0x00A0D2D0 (non-virtual)
};

void CPlayer::UninitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(this->InputProcessDebugControls_ptr)->RemoveFromInterface();
    RemoveMode(PLAYER_MODE_CHARGE_QUICK_ACCESS);
}