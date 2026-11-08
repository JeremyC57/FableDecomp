// CPlayer::InitInterfaces() -- builds the player's 66 input processes, each from one shared CInputProcessGameBaseInit, into its scoped_ptr member
// Retail: Fable.exe (TLC, VC7.1) @ 0x00488D10, 4142 bytes (+ int3 padding); private (Ego_r.pdb / FableWin.pdb ?InitInterfaces@CPlayer@@AAEXXZ);
// one caller, 0x0048A38C.
// Leaf: ?InitInterfaces@CPlayer@@AAEXXZ
// Shape: the init block (World, this, DefinitionManager, Component) is a 16-byte local filled once; then, per member in retail's
// statement order (the inventory Stats/Magic pair and FirstPerson/YesNoQuestion run out of member order), `scoped_ptr::reset(new T(init))`:
// operator new 0x00BFEA1A (`push size`), a null test, the out-of-line CInputProcessGameBase constructor 0x00687A30 and the inline
// derived constructor (vtable store + members), the old pointer read, the new one stored, and the old process deleted through slot 0.
// The block is filled in World, DefinitionManager, MyPlayer, Component order (the reverse pair order gives a scheduling swap).
// Out-of-line callees: CInputProcessGameBase::CInputProcessGameBase 0x00687A30, CBaseIntelligentPointer::CBaseIntelligentPointer
// 0x00A01B10 (ZTarget, CreatureMovement, DebugControls), CJoystickPositionTracker::CJoystickPositionTracker 0x00A0E0A0 (Combat),
// CInputProcessControlSpirit::CInputProcessControlSpirit 0x006A9140, malloc 0x00BFEA0E (JumpingAndRolling's two STLport list sentinels).
// Models: engine/CPlayer.h (FABLE_RETAIL_PLAYER + _INPUTPROCESSES), the CInputProcess family under FABLE_RETAIL_INPUTPROCESS
// (one engine/<Class>.h per class), boost/scoped_ptr.h (FABLE_RETAIL_SCOPED_PTR_RESET).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessBButtonExitMode.h"
#include "engine/CInputProcessBetting.h"
#include "engine/CInputProcessBlock.h"
#include "engine/CInputProcessBoastUI.h"
#include "engine/CInputProcessCameraLookAround.h"
#include "engine/CInputProcessClickPastText.h"
#include "engine/CInputProcessCombat.h"
#include "engine/CInputProcessConsole.h"
#include "engine/CInputProcessControlCreature.h"
#include "engine/CInputProcessControlCreatureActivateZTarget.h"
#include "engine/CInputProcessControlCreatureActivateZTargetOnPress.h"
#include "engine/CInputProcessControlCreatureRightStick.h"
#include "engine/CInputProcessControlFreeCamera.h"
#include "engine/CInputProcessControlSpirit.h"
#include "engine/CInputProcessCreatureMovement.h"
#include "engine/CInputProcessCreatureMovementWatchForControlAngleChange.h"
#include "engine/CInputProcessCreditsUI.h"
#include "engine/CInputProcessCutScene.h"
#include "engine/CInputProcessCycleSpecialCameraModes.h"
#include "engine/CInputProcessDead.h"
#include "engine/CInputProcessDebugControls.h"
#include "engine/CInputProcessDigging.h"
#include "engine/CInputProcessFireheartMinigame.h"
#include "engine/CInputProcessFirstPerson.h"
#include "engine/CInputProcessFirstPersonLookAround.h"
#include "engine/CInputProcessFirstPersonTargeting.h"
#include "engine/CInputProcessFishing.h"
#include "engine/CInputProcessFreezeControls.h"
#include "engine/CInputProcessHeroAbilitiesScreen.h"
#include "engine/CInputProcessHeroInformationScreens.h"
#include "engine/CInputProcessInGameMenu.h"
#include "engine/CInputProcessInventory.h"
#include "engine/CInputProcessInventoryClothing.h"
#include "engine/CInputProcessInventoryExperienceScreen.h"
#include "engine/CInputProcessInventoryMagicScreen.h"
#include "engine/CInputProcessInventoryMapScreen.h"
#include "engine/CInputProcessInventoryQuestsScreen.h"
#include "engine/CInputProcessInventoryStatsScreen.h"
#include "engine/CInputProcessInventoryTradeScreen.h"
#include "engine/CInputProcessInventoryWeapons.h"
#include "engine/CInputProcessJumpingAndRolling.h"
#include "engine/CInputProcessLightning.h"
#include "engine/CInputProcessMain.h"
#include "engine/CInputProcessOracleMinigame.h"
#include "engine/CInputProcessParalysed.h"
#include "engine/CInputProcessPhotojournalCapture.h"
#include "engine/CInputProcessProjectileTargetingAnalogueZoom.h"
#include "engine/CInputProcessQuestCompletionUI.h"
#include "engine/CInputProcessQuickAccessItems.h"
#include "engine/CInputProcessQuickAccessMenu.h"
#include "engine/CInputProcessRebootGame.h"
#include "engine/CInputProcessSetRangedWeaponMode.h"
#include "engine/CInputProcessSetRangedWeaponThirdPersonMode.h"
#include "engine/CInputProcessSpecialAbilities.h"
#include "engine/CInputProcessStrafe.h"
#include "engine/CInputProcessTargetLockCycleTargets.h"
#include "engine/CInputProcessTargetLockRightStickTargetSelect.h"
#include "engine/CInputProcessTavernGame.h"
#include "engine/CInputProcessToggleViewHeroMode.h"
#include "engine/CInputProcessUseEnvironment.h"
#include "engine/CInputProcessUseRangedWeapon.h"
#include "engine/CInputProcessUseRangedWeaponZLock.h"
#include "engine/CInputProcessWatchForRangedWeaponThirdPersonModeTermination.h"
#include "engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h"
#include "engine/CInputProcessYesNoQuestion.h"
#include "engine/CInputProcessZTarget.h"

void CPlayer::InitInterfaces()
{
    CInputProcessGameBaseInit init;
    init.World = World;
    init.DefinitionManager = DefinitionManager;
    init.MyPlayer = this;
    init.Component = Component;

    InputProcessMain.reset(new CInputProcessMain(init));
    InputProcessControlCreature.reset(new CInputProcessControlCreature(init));
    InputProcessZTarget.reset(new CInputProcessZTarget(init));
    InputProcessDead.reset(new CInputProcessDead(init));
    InputProcessInventory.reset(new CInputProcessInventory(init));
    InputProcessInventoryClothing.reset(new CInputProcessInventoryClothing(init));
    InputProcessInventoryWeapons.reset(new CInputProcessInventoryWeapons(init));
    InputProcessHeroAbilitiesScreen.reset(new CInputProcessHeroAbilitiesScreen(init));
    InputProcessInventoryMapScreen.reset(new CInputProcessInventoryMapScreen(init));
    InputProcessInventoryMagicScreen.reset(new CInputProcessInventoryMagicScreen(init));
    InputProcessInventoryStatsScreen.reset(new CInputProcessInventoryStatsScreen(init));
    InputProcessInventoryExperienceScreen.reset(new CInputProcessInventoryExperienceScreen(init));
    InputProcessInventoryTradeScreen.reset(new CInputProcessInventoryTradeScreen(init));
    InputProcessInventoryQuestsScreen.reset(new CInputProcessInventoryQuestsScreen(init));
    InputProcessHeroInformationScreens.reset(new CInputProcessHeroInformationScreens(init));
    InputProcessClickPastText.reset(new CInputProcessClickPastText(init));
    InputProcessYesNoQuestion.reset(new CInputProcessYesNoQuestion(init));
    InputProcessFirstPerson.reset(new CInputProcessFirstPerson(init));
    InputProcessFreezeControls.reset(new CInputProcessFreezeControls(init));
    InputProcessControlFreeCamera.reset(new CInputProcessControlFreeCamera(init));
    InputProcessCreatureMovement.reset(new CInputProcessCreatureMovement(init));
    InputProcessUseEnvironment.reset(new CInputProcessUseEnvironment(init));
    InputProcessBlock.reset(new CInputProcessBlock(init));
    InputProcessDebugControls.reset(new CInputProcessDebugControls(init));
    InputProcessQuickAccessItems.reset(new CInputProcessQuickAccessItems(init));
    InputProcessCombat.reset(new CInputProcessCombat(init));
    InputProcessSpecialAbilities.reset(new CInputProcessSpecialAbilities(init));
    InputProcessControlCreatureRightStick.reset(new CInputProcessControlCreatureRightStick(init));
    InputProcessRightStickLookAround.reset(new CInputProcessCameraLookAround(init));
    InputProcessCycleSpecialCameraModes.reset(new CInputProcessCycleSpecialCameraModes(init));
    InputProcessRebootGame.reset(new CInputProcessRebootGame(init));
    InputProcessJumpingAndRolling.reset(new CInputProcessJumpingAndRolling(init));
    InputProcessFirstPersonTargeting.reset(new CInputProcessFirstPersonTargeting(init));
    InputProcessUseRangedWeapon.reset(new CInputProcessUseRangedWeapon(init));
    InputProcessControlCreatureActivateZTarget.reset(new CInputProcessControlCreatureActivateZTarget(init));
    InputProcessControlCreatureActivateZTargetOnPress.reset(new CInputProcessControlCreatureActivateZTargetOnPress(init));
    InputProcessLeftStickLookAround.reset(new CInputProcessFirstPersonLookAround(init));
    InputProcessInGameMenu.reset(new CInputProcessInGameMenu(init));
    InputProcessControlSpirit.reset(new CInputProcessControlSpirit(init));
    InputProcessTavernGame.reset(new CInputProcessTavernGame(init));
    InputProcessSetRangedWeaponMode.reset(new CInputProcessSetRangedWeaponMode(init));
    InputProcessCutScene.reset(new CInputProcessCutScene(init));
    InputProcessFishing.reset(new CInputProcessFishing(init));
    InputProcessDigging.reset(new CInputProcessDigging(init));
    InputProcessParalysed.reset(new CInputProcessParalysed(init));
    InputProcessBoastUI.reset(new CInputProcessBoastUI(init));
    InputProcessSetRangedWeaponThirdPerson.reset(new CInputProcessSetRangedWeaponThirdPersonMode(init));
    InputProcessUseRangedWeaponZLock.reset(new CInputProcessUseRangedWeaponZLock(init));
    InputProcessWatchForRangedWeaponThirdPersonModeTermination.reset(new CInputProcessWatchForRangedWeaponThirdPersonModeTermination(init));
    InputProcessWatchForWillChargeUpThirdPersonModeTermination.reset(new CInputProcessWatchForWillChargeUpThirdPersonModeTermination(init));
    InputProcessTargetLockCycleTargets.reset(new CInputProcessTargetLockCycleTargets(init));
    InputProcessQuickAccessMenu.reset(new CInputProcessQuickAccessMenu(init));
    InputProcessQuestCompletionUI.reset(new CInputProcessQuestCompletionUI(init));
    InputProcessTargetLockRightStickTargetSelect.reset(new CInputProcessTargetLockRightStickTargetSelect(init));
    InputProcessBButtonExitMode.reset(new CInputProcessBButtonExitMode(init));
    InputProcessCreditsUI.reset(new CInputProcessCreditsUI(init));
    InputProcessBetting.reset(new CInputProcessBetting(init));
    InputProcessLightning.reset(new CInputProcessLightning(init));
    InputProcessWatchForControlAngleChange.reset(new CInputProcessCreatureMovementWatchForControlAngleChange(init));
    InputProcessOracleMinigame.reset(new CInputProcessOracleMinigame(init));
    InputProcessFireheartMinigame.reset(new CInputProcessFireheartMinigame(init));
    InputProcessStrafe.reset(new CInputProcessStrafe(init));
    InputProcessConsole.reset(new CInputProcessConsole(init));
    InputProcessProjectileTargetingAnalogueZoom.reset(new CInputProcessProjectileTargetingAnalogueZoom(init));
    InputProcessPhotojournalCapture.reset(new CInputProcessPhotojournalCapture(init));
    InputProcessToggleViewHeroMode.reset(new CInputProcessToggleViewHeroMode(init));
}