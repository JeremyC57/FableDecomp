// CPlayer::UninitModeQuickAccessMenu(EPlayerMode, CProcessedInput*) -- leaving the quick-access menu mode detaches every input process the mode attached
// Leaf: ?UninitModeQuickAccessMenu@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00633A60, 172 bytes, `ret 8` (both hook arguments unused).
// Identity: the catalog label "~CSpecialAbilitiesDivineWrathDef" is propagated (a destructor never takes two stack
//   arguments). The player-mode hook table filled at 0x0121B600.. stores {Init, Uninit} pointers per mode with a
//   0x10 stride: 0x00633FF0 (InitModeQuickAccessMenu, landed manifest name) at 0x013B9108 and this body at 0x013B9118,
//   and Ego_r.pdb carries `?UninitModeQuickAccessMenu@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z`.
// Shape: the mirror of InitModeQuickAccessMenu, which adds the same fifteen processes in the same member order;
//   each call is the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on the scoped_ptr's raw pointer
//   (engine/CPlayer.h offsets match retail here, as in the landed UninitModeBerserk 0x006339B0).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessControlCreature.h"
#include "engine/CInputProcessControlCreatureActivateZTarget.h"
#include "engine/CInputProcessControlCreatureRightStick.h"
#include "engine/CInputProcessCameraLookAround.h"
#include "engine/CInputProcessCycleSpecialCameraModes.h"
#include "engine/CInputProcessCreatureMovement.h"
#include "engine/CInputProcessUseEnvironment.h"
#include "engine/CInputProcessBlock.h"
#include "engine/CInputProcessDebugControls.h"
#include "engine/CInputProcessCombat.h"
#include "engine/CInputProcessUseRangedWeapon.h"
#include "engine/CInputProcessSpecialAbilities.h"
#include "engine/CInputProcessJumpingAndRolling.h"
#include "engine/CInputProcessSetRangedWeaponMode.h"
#include "engine/CInputProcessQuickAccessMenu.h"

void CPlayer::UninitModeQuickAccessMenu(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature->RemoveFromInterface();
    InputProcessControlCreatureActivateZTarget->RemoveFromInterface();
    InputProcessControlCreatureRightStick->RemoveFromInterface();
    InputProcessRightStickLookAround->RemoveFromInterface();
    InputProcessCycleSpecialCameraModes->RemoveFromInterface();
    InputProcessCreatureMovement->RemoveFromInterface();
    InputProcessUseEnvironment->RemoveFromInterface();
    InputProcessBlock->RemoveFromInterface();
    InputProcessDebugControls->RemoveFromInterface();
    InputProcessCombat->RemoveFromInterface();
    InputProcessUseRangedWeapon->RemoveFromInterface();
    InputProcessSpecialAbilities->RemoveFromInterface();
    InputProcessJumpingAndRolling->RemoveFromInterface();
    InputProcessSetRangedWeaponMode->RemoveFromInterface();
    InputProcessQuickAccessMenu->RemoveFromInterface();
}
