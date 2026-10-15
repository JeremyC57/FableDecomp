// CPlayer::InitModeParalysed(EPlayerMode mode, CProcessedInput* input) -- entering the paralysed mode attaches the paralysed, right-stick creature, right-stick look-around, special-camera-cycle, creature-movement and debug input processes to the player's interface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006337C0, length 97 bytes.
// Callees: each input process's virtual AddToInterface(CGamePlayerInterface*) (slot 1, `call [eax+4]`).
// Shape: engine/CPlayer.h members (PlayerInterface +0x0C, InputProcessCreatureMovement_ptr +0x150,
// InputProcessDebugControls_ptr +0x15C, InputProcessControlCreatureRightStick_ptr +0x16C,
// InputProcessRightStickLookAround_ptr +0x170, InputProcessCycleSpecialCameraModes_ptr +0x174,
// InputProcessParalysed_ptr +0x1B0), the landed CPlayer::InitModeChargeQuickAccess (0x00633700) pattern: the
// scoped_ptr's object goes to ecx first, PlayerInterface is re-read for every call, both hook arguments are unused.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CPlayer_Methods : CPlayer {
    void InitModeParalysed(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeParalysed(EPlayerMode mode, CProcessedInput* input)
{
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessParalysed_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessControlCreatureRightStick_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessRightStickLookAround_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessCreatureMovement_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessDebugControls_ptr)->AddToInterface(this->PlayerInterface);
}
