// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CPlayer::UninitModeControlCreature(EPlayerMode, CProcessedInput*) -- remove all control-mode input processes from the interface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00633420, 208 bytes. The eighteen calls are the non-virtual
// CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (manifest), in the retail member order.
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members); CPlayer_Methods is the lane's method-carrier
// convention for generated data-only headers (see the landed CPlayer::InitModeControlFreeCamera 0x006335E0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeControlCreature(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeControlCreature(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseEnvironment_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponThirdPerson_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessMenu_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessToggleViewHeroMode_ptr)->RemoveFromInterface();
}