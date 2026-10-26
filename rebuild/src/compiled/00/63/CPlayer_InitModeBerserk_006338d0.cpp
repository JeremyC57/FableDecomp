// Grade: MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CPlayer::InitModeBerserk(EPlayerMode, CProcessedInput*) -- attach the berserk mode's input processes to the player interface
// Retail: Fable.exe (TLC, VC7.1) @ 0x006338D0, 224 bytes. Each process gets the virtual
// CAInputProcess::AddToInterface(CGamePlayerInterface*) call (vtable slot 1), with PlayerInterface passed as its argument.
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C); CPlayer_Methods is the lane's
// method-carrier convention for generated data-only headers (see the landed CPlayer::InitModeControlFreeCamera 0x006335E0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: virtual void VFunc00(); virtual bool AddToInterface(CGamePlayerInterface*); };   // slot 1 (+0x04)
struct CPlayer_Methods : CPlayer { void InitModeBerserk(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::InitModeBerserk(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
}