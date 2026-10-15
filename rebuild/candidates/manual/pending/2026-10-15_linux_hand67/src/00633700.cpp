// CPlayer::InitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input) -- entering the charge/quick-access mode attaches the quick-access, right-stick creature, right-stick look-around, special-camera-cycle and debug input processes to the player's interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633700, length 82 bytes.
// Identity: Ego_r.pdb `?InitModeChargeQuickAccess@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z`.
// Callees: each input process's virtual AddToInterface(CGamePlayerInterface*) (slot 1, `call [eax+4]`).
// Shape: engine/CPlayer.h members (PlayerInterface +0x0C, InputProcessDebugControls_ptr +0x15C,
// InputProcessQuickAccessItems_ptr +0x160, InputProcessControlCreatureRightStick_ptr +0x16C,
// InputProcessRightStickLookAround_ptr +0x170, InputProcessCycleSpecialCameraModes_ptr +0x174), the
// landed CPlayer::InitModeQuestCompletionUI (0x00633B10) pattern: the scoped_ptr's object goes to
// ecx first, PlayerInterface is re-read for every call, both hook arguments are unused (`ret 8`).
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CPlayer_Methods : CPlayer {
    void InitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeChargeQuickAccess(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessQuickAccessItems_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessControlCreatureRightStick_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessRightStickLookAround_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessDebugControls_ptr)->AddToInterface(this->PlayerInterface);
}
