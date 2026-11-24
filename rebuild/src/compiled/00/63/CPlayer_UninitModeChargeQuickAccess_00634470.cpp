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
// NF295 (2026-11-23): re-ported onto the landed siblings' shape (0x00633A60 / 0x00633880 UninitMode<X>): FABLE_RETAIL_PLAYER_INPUTPROCESSES
//   gives the scoped_ptr members and FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE the Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ
//   (0x00A0D2D0) on the per-process headers' bases; was the reinterpret_cast stand-in CAInputProcessCalls.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/EPlayerMode.h"
#include "engine/CInputProcessQuickAccessItems.h"
#include "engine/CInputProcessControlCreatureRightStick.h"
#include "engine/CInputProcessCameraLookAround.h"
#include "engine/CInputProcessCycleSpecialCameraModes.h"
#include "engine/CInputProcessDebugControls.h"

void CPlayer::UninitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessQuickAccessItems->RemoveFromInterface();
    InputProcessControlCreatureRightStick->RemoveFromInterface();
    InputProcessRightStickLookAround->RemoveFromInterface();
    InputProcessCycleSpecialCameraModes->RemoveFromInterface();
    InputProcessDebugControls->RemoveFromInterface();
    RemoveMode(PLAYER_MODE_CHARGE_QUICK_ACCESS);
}
