// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CPlayer::UninitModeBerserk(EPlayerMode, CProcessedInput*) -- remove each Berserk-mode input process from the player interface
// Retail: Fable.exe (TLC, VC7.1) @ 0x006339B0, 176 bytes. Each of the fourteen calls is the non-virtual
// CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (manifest), in the same member order InitModeBerserk 0x006338D0 adds them.
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members); CPlayer_Methods is the lane's method-carrier
// convention for generated data-only headers (see the landed CPlayer::InitModeControlFreeCamera 0x006335E0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeBerserk(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeBerserk(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
}