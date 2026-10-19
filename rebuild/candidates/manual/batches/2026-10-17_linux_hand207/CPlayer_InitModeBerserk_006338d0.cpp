// CPlayer::InitModeBerserk(EPlayerMode, CProcessedInput*) -- attach the berserk mode's input processes to the player interface
// Retail: Fable.exe @ 0x006338d0, 224 bytes. Each process has a virtual
// AddToInterface call (vtable slot 1), with PlayerInterface passed as its argument.
#include "engine/CPlayer.h"
typedef long EPlayerMode; struct CProcessedInput;
struct CInputProcessVirtuals { virtual void v00(); virtual void AddToInterface(CGamePlayerInterface*); };
struct CPlayer_Methods : CPlayer { void InitModeBerserk(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::InitModeBerserk(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessControlCreatureRightStick_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessCombat_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CInputProcessVirtuals*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
}
