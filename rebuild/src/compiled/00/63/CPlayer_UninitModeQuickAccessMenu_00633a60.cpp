// CPlayer::UninitModeQuickAccessMenu(EPlayerMode, CProcessedInput*) -- leaving the quick-access menu mode detaches every input process the mode attached
// Leaf: UninitModeQuickAccessMenu
// Retail: Fable.exe (TLC, VC7.1) @ 0x00633A60, 172 bytes, `ret 8` (both hook arguments unused).
// Identity: the catalog label "~CSpecialAbilitiesDivineWrathDef" is propagated (a destructor never takes two stack
//   arguments). The player-mode hook table filled at 0x0121B600.. stores {Init, Uninit} pointers per mode with a
//   0x10 stride: 0x00633FF0 (InitModeQuickAccessMenu, landed manifest name) at 0x013B9108 and this body at 0x013B9118,
//   and Ego_r.pdb carries `?UninitModeQuickAccessMenu@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z`.
// Shape: the mirror of InitModeQuickAccessMenu, which adds the same fifteen processes in the same member order;
//   each call is the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on the scoped_ptr's raw pointer
//   (engine/CPlayer.h offsets match retail here, as in the landed UninitModeBerserk 0x006339B0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; struct CProcessedInput;
struct CAInputProcess { void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeQuickAccessMenu(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeQuickAccessMenu(EPlayerMode mode, CProcessedInput* input)
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
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessMenu_ptr)->RemoveFromInterface();
}