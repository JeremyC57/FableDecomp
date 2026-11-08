// Behaviour test for 0x00488D10 CPlayer::InitInterfaces: every one of the 66 scoped_ptr members receives a freshly built process of
// its own class, constructed from the shared init block (World, the player, DefinitionManager, Component), and a second call deletes
// each previous process exactly once (scoped_ptr::reset semantics, the virtual destructor through the base pointer).
#include <cstdio>
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CPlayer;
struct CInputProcessGameBaseInit { CWorld* World; CPlayer* MyPlayer; CGameDefinitionManager* DefinitionManager; CMainGameComponent* Component; };
static int g_live = 0, g_deleted = 0;
class CInputProcessGameBase {
public:
    CInputProcessGameBase(const CInputProcessGameBaseInit& init, int kind)
        : World(init.World), MyPlayer(init.MyPlayer), DefinitionManager(init.DefinitionManager), Component(init.Component), Kind(kind) { ++g_live; }
    virtual ~CInputProcessGameBase() { --g_live; ++g_deleted; }
    CWorld* World; CPlayer* MyPlayer; CGameDefinitionManager* DefinitionManager; CMainGameComponent* Component; int Kind;
};
template<class T> class scoped_ptr {
public:
    scoped_ptr() : ptr(0) { }
    ~scoped_ptr() { delete ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }   // boost 1.30: scoped_ptr(p).swap(*this), then the temporary's checked_delete
    T* get() const { return ptr; }
private:
    T* ptr;
};
#define INPUT_PROCESS(Name, Kind) class Name : public CInputProcessGameBase { public: Name(const CInputProcessGameBaseInit& i) : CInputProcessGameBase(i, Kind) { } };
INPUT_PROCESS(CInputProcessMain, 0)
INPUT_PROCESS(CInputProcessControlCreature, 1)
INPUT_PROCESS(CInputProcessZTarget, 2)
INPUT_PROCESS(CInputProcessDead, 3)
INPUT_PROCESS(CInputProcessInventory, 4)
INPUT_PROCESS(CInputProcessInventoryClothing, 5)
INPUT_PROCESS(CInputProcessInventoryWeapons, 6)
INPUT_PROCESS(CInputProcessHeroAbilitiesScreen, 7)
INPUT_PROCESS(CInputProcessInventoryMapScreen, 8)
INPUT_PROCESS(CInputProcessInventoryMagicScreen, 9)
INPUT_PROCESS(CInputProcessInventoryStatsScreen, 10)
INPUT_PROCESS(CInputProcessInventoryExperienceScreen, 11)
INPUT_PROCESS(CInputProcessInventoryTradeScreen, 12)
INPUT_PROCESS(CInputProcessInventoryQuestsScreen, 13)
INPUT_PROCESS(CInputProcessHeroInformationScreens, 14)
INPUT_PROCESS(CInputProcessClickPastText, 15)
INPUT_PROCESS(CInputProcessYesNoQuestion, 16)
INPUT_PROCESS(CInputProcessFirstPerson, 17)
INPUT_PROCESS(CInputProcessFreezeControls, 18)
INPUT_PROCESS(CInputProcessControlFreeCamera, 19)
INPUT_PROCESS(CInputProcessCreatureMovement, 20)
INPUT_PROCESS(CInputProcessUseEnvironment, 21)
INPUT_PROCESS(CInputProcessBlock, 22)
INPUT_PROCESS(CInputProcessDebugControls, 23)
INPUT_PROCESS(CInputProcessQuickAccessItems, 24)
INPUT_PROCESS(CInputProcessCombat, 25)
INPUT_PROCESS(CInputProcessSpecialAbilities, 26)
INPUT_PROCESS(CInputProcessControlCreatureRightStick, 27)
INPUT_PROCESS(CInputProcessCameraLookAround, 28)
INPUT_PROCESS(CInputProcessCycleSpecialCameraModes, 29)
INPUT_PROCESS(CInputProcessRebootGame, 30)
INPUT_PROCESS(CInputProcessJumpingAndRolling, 31)
INPUT_PROCESS(CInputProcessFirstPersonTargeting, 32)
INPUT_PROCESS(CInputProcessUseRangedWeapon, 33)
INPUT_PROCESS(CInputProcessControlCreatureActivateZTarget, 34)
INPUT_PROCESS(CInputProcessControlCreatureActivateZTargetOnPress, 35)
INPUT_PROCESS(CInputProcessFirstPersonLookAround, 36)
INPUT_PROCESS(CInputProcessInGameMenu, 37)
INPUT_PROCESS(CInputProcessControlSpirit, 38)
INPUT_PROCESS(CInputProcessTavernGame, 39)
INPUT_PROCESS(CInputProcessSetRangedWeaponMode, 40)
INPUT_PROCESS(CInputProcessCutScene, 41)
INPUT_PROCESS(CInputProcessFishing, 42)
INPUT_PROCESS(CInputProcessDigging, 43)
INPUT_PROCESS(CInputProcessParalysed, 44)
INPUT_PROCESS(CInputProcessBoastUI, 45)
INPUT_PROCESS(CInputProcessSetRangedWeaponThirdPersonMode, 46)
INPUT_PROCESS(CInputProcessUseRangedWeaponZLock, 47)
INPUT_PROCESS(CInputProcessWatchForRangedWeaponThirdPersonModeTermination, 48)
INPUT_PROCESS(CInputProcessWatchForWillChargeUpThirdPersonModeTermination, 49)
INPUT_PROCESS(CInputProcessTargetLockCycleTargets, 50)
INPUT_PROCESS(CInputProcessQuickAccessMenu, 51)
INPUT_PROCESS(CInputProcessQuestCompletionUI, 52)
INPUT_PROCESS(CInputProcessTargetLockRightStickTargetSelect, 53)
INPUT_PROCESS(CInputProcessBButtonExitMode, 54)
INPUT_PROCESS(CInputProcessCreditsUI, 55)
INPUT_PROCESS(CInputProcessBetting, 56)
INPUT_PROCESS(CInputProcessLightning, 57)
INPUT_PROCESS(CInputProcessCreatureMovementWatchForControlAngleChange, 58)
INPUT_PROCESS(CInputProcessOracleMinigame, 59)
INPUT_PROCESS(CInputProcessFireheartMinigame, 60)
INPUT_PROCESS(CInputProcessStrafe, 61)
INPUT_PROCESS(CInputProcessConsole, 62)
INPUT_PROCESS(CInputProcessProjectileTargetingAnalogueZoom, 63)
INPUT_PROCESS(CInputProcessPhotojournalCapture, 64)
INPUT_PROCESS(CInputProcessToggleViewHeroMode, 65)
class CPlayer {
public:
    CMainGameComponent* Component; CGameDefinitionManager* DefinitionManager; CWorld* World;
    scoped_ptr<CInputProcessMain> InputProcessMain;
    scoped_ptr<CInputProcessControlCreature> InputProcessControlCreature;
    scoped_ptr<CInputProcessZTarget> InputProcessZTarget;
    scoped_ptr<CInputProcessDead> InputProcessDead;
    scoped_ptr<CInputProcessInventory> InputProcessInventory;
    scoped_ptr<CInputProcessInventoryClothing> InputProcessInventoryClothing;
    scoped_ptr<CInputProcessInventoryWeapons> InputProcessInventoryWeapons;
    scoped_ptr<CInputProcessHeroAbilitiesScreen> InputProcessHeroAbilitiesScreen;
    scoped_ptr<CInputProcessInventoryMapScreen> InputProcessInventoryMapScreen;
    scoped_ptr<CInputProcessInventoryMagicScreen> InputProcessInventoryMagicScreen;
    scoped_ptr<CInputProcessInventoryStatsScreen> InputProcessInventoryStatsScreen;
    scoped_ptr<CInputProcessInventoryExperienceScreen> InputProcessInventoryExperienceScreen;
    scoped_ptr<CInputProcessInventoryTradeScreen> InputProcessInventoryTradeScreen;
    scoped_ptr<CInputProcessInventoryQuestsScreen> InputProcessInventoryQuestsScreen;
    scoped_ptr<CInputProcessHeroInformationScreens> InputProcessHeroInformationScreens;
    scoped_ptr<CInputProcessClickPastText> InputProcessClickPastText;
    scoped_ptr<CInputProcessYesNoQuestion> InputProcessYesNoQuestion;
    scoped_ptr<CInputProcessFirstPerson> InputProcessFirstPerson;
    scoped_ptr<CInputProcessFreezeControls> InputProcessFreezeControls;
    scoped_ptr<CInputProcessControlFreeCamera> InputProcessControlFreeCamera;
    scoped_ptr<CInputProcessCreatureMovement> InputProcessCreatureMovement;
    scoped_ptr<CInputProcessUseEnvironment> InputProcessUseEnvironment;
    scoped_ptr<CInputProcessBlock> InputProcessBlock;
    scoped_ptr<CInputProcessDebugControls> InputProcessDebugControls;
    scoped_ptr<CInputProcessQuickAccessItems> InputProcessQuickAccessItems;
    scoped_ptr<CInputProcessCombat> InputProcessCombat;
    scoped_ptr<CInputProcessSpecialAbilities> InputProcessSpecialAbilities;
    scoped_ptr<CInputProcessControlCreatureRightStick> InputProcessControlCreatureRightStick;
    scoped_ptr<CInputProcessCameraLookAround> InputProcessRightStickLookAround;
    scoped_ptr<CInputProcessCycleSpecialCameraModes> InputProcessCycleSpecialCameraModes;
    scoped_ptr<CInputProcessRebootGame> InputProcessRebootGame;
    scoped_ptr<CInputProcessJumpingAndRolling> InputProcessJumpingAndRolling;
    scoped_ptr<CInputProcessFirstPersonTargeting> InputProcessFirstPersonTargeting;
    scoped_ptr<CInputProcessUseRangedWeapon> InputProcessUseRangedWeapon;
    scoped_ptr<CInputProcessControlCreatureActivateZTarget> InputProcessControlCreatureActivateZTarget;
    scoped_ptr<CInputProcessControlCreatureActivateZTargetOnPress> InputProcessControlCreatureActivateZTargetOnPress;
    scoped_ptr<CInputProcessFirstPersonLookAround> InputProcessLeftStickLookAround;
    scoped_ptr<CInputProcessInGameMenu> InputProcessInGameMenu;
    scoped_ptr<CInputProcessControlSpirit> InputProcessControlSpirit;
    scoped_ptr<CInputProcessTavernGame> InputProcessTavernGame;
    scoped_ptr<CInputProcessSetRangedWeaponMode> InputProcessSetRangedWeaponMode;
    scoped_ptr<CInputProcessCutScene> InputProcessCutScene;
    scoped_ptr<CInputProcessFishing> InputProcessFishing;
    scoped_ptr<CInputProcessDigging> InputProcessDigging;
    scoped_ptr<CInputProcessParalysed> InputProcessParalysed;
    scoped_ptr<CInputProcessBoastUI> InputProcessBoastUI;
    scoped_ptr<CInputProcessSetRangedWeaponThirdPersonMode> InputProcessSetRangedWeaponThirdPerson;
    scoped_ptr<CInputProcessUseRangedWeaponZLock> InputProcessUseRangedWeaponZLock;
    scoped_ptr<CInputProcessWatchForRangedWeaponThirdPersonModeTermination> InputProcessWatchForRangedWeaponThirdPersonModeTermination;
    scoped_ptr<CInputProcessWatchForWillChargeUpThirdPersonModeTermination> InputProcessWatchForWillChargeUpThirdPersonModeTermination;
    scoped_ptr<CInputProcessTargetLockCycleTargets> InputProcessTargetLockCycleTargets;
    scoped_ptr<CInputProcessQuickAccessMenu> InputProcessQuickAccessMenu;
    scoped_ptr<CInputProcessQuestCompletionUI> InputProcessQuestCompletionUI;
    scoped_ptr<CInputProcessTargetLockRightStickTargetSelect> InputProcessTargetLockRightStickTargetSelect;
    scoped_ptr<CInputProcessBButtonExitMode> InputProcessBButtonExitMode;
    scoped_ptr<CInputProcessCreditsUI> InputProcessCreditsUI;
    scoped_ptr<CInputProcessBetting> InputProcessBetting;
    scoped_ptr<CInputProcessLightning> InputProcessLightning;
    scoped_ptr<CInputProcessCreatureMovementWatchForControlAngleChange> InputProcessWatchForControlAngleChange;
    scoped_ptr<CInputProcessOracleMinigame> InputProcessOracleMinigame;
    scoped_ptr<CInputProcessFireheartMinigame> InputProcessFireheartMinigame;
    scoped_ptr<CInputProcessStrafe> InputProcessStrafe;
    scoped_ptr<CInputProcessConsole> InputProcessConsole;
    scoped_ptr<CInputProcessProjectileTargetingAnalogueZoom> InputProcessProjectileTargetingAnalogueZoom;
    scoped_ptr<CInputProcessPhotojournalCapture> InputProcessPhotojournalCapture;
    scoped_ptr<CInputProcessToggleViewHeroMode> InputProcessToggleViewHeroMode;
    void InitInterfaces();
    CInputProcessGameBase* Slot(int i);
};
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
CInputProcessGameBase* CPlayer::Slot(int i)
{
    switch (i) {
    case 0: return InputProcessMain.get();
    case 1: return InputProcessControlCreature.get();
    case 2: return InputProcessZTarget.get();
    case 3: return InputProcessDead.get();
    case 4: return InputProcessInventory.get();
    case 5: return InputProcessInventoryClothing.get();
    case 6: return InputProcessInventoryWeapons.get();
    case 7: return InputProcessHeroAbilitiesScreen.get();
    case 8: return InputProcessInventoryMapScreen.get();
    case 9: return InputProcessInventoryMagicScreen.get();
    case 10: return InputProcessInventoryStatsScreen.get();
    case 11: return InputProcessInventoryExperienceScreen.get();
    case 12: return InputProcessInventoryTradeScreen.get();
    case 13: return InputProcessInventoryQuestsScreen.get();
    case 14: return InputProcessHeroInformationScreens.get();
    case 15: return InputProcessClickPastText.get();
    case 16: return InputProcessYesNoQuestion.get();
    case 17: return InputProcessFirstPerson.get();
    case 18: return InputProcessFreezeControls.get();
    case 19: return InputProcessControlFreeCamera.get();
    case 20: return InputProcessCreatureMovement.get();
    case 21: return InputProcessUseEnvironment.get();
    case 22: return InputProcessBlock.get();
    case 23: return InputProcessDebugControls.get();
    case 24: return InputProcessQuickAccessItems.get();
    case 25: return InputProcessCombat.get();
    case 26: return InputProcessSpecialAbilities.get();
    case 27: return InputProcessControlCreatureRightStick.get();
    case 28: return InputProcessRightStickLookAround.get();
    case 29: return InputProcessCycleSpecialCameraModes.get();
    case 30: return InputProcessRebootGame.get();
    case 31: return InputProcessJumpingAndRolling.get();
    case 32: return InputProcessFirstPersonTargeting.get();
    case 33: return InputProcessUseRangedWeapon.get();
    case 34: return InputProcessControlCreatureActivateZTarget.get();
    case 35: return InputProcessControlCreatureActivateZTargetOnPress.get();
    case 36: return InputProcessLeftStickLookAround.get();
    case 37: return InputProcessInGameMenu.get();
    case 38: return InputProcessControlSpirit.get();
    case 39: return InputProcessTavernGame.get();
    case 40: return InputProcessSetRangedWeaponMode.get();
    case 41: return InputProcessCutScene.get();
    case 42: return InputProcessFishing.get();
    case 43: return InputProcessDigging.get();
    case 44: return InputProcessParalysed.get();
    case 45: return InputProcessBoastUI.get();
    case 46: return InputProcessSetRangedWeaponThirdPerson.get();
    case 47: return InputProcessUseRangedWeaponZLock.get();
    case 48: return InputProcessWatchForRangedWeaponThirdPersonModeTermination.get();
    case 49: return InputProcessWatchForWillChargeUpThirdPersonModeTermination.get();
    case 50: return InputProcessTargetLockCycleTargets.get();
    case 51: return InputProcessQuickAccessMenu.get();
    case 52: return InputProcessQuestCompletionUI.get();
    case 53: return InputProcessTargetLockRightStickTargetSelect.get();
    case 54: return InputProcessBButtonExitMode.get();
    case 55: return InputProcessCreditsUI.get();
    case 56: return InputProcessBetting.get();
    case 57: return InputProcessLightning.get();
    case 58: return InputProcessWatchForControlAngleChange.get();
    case 59: return InputProcessOracleMinigame.get();
    case 60: return InputProcessFireheartMinigame.get();
    case 61: return InputProcessStrafe.get();
    case 62: return InputProcessConsole.get();
    case 63: return InputProcessProjectileTargetingAnalogueZoom.get();
    case 64: return InputProcessPhotojournalCapture.get();
    case 65: return InputProcessToggleViewHeroMode.get();
    }
    return 0;
}
int main()
{
    CPlayer* player = new CPlayer;
    player->World = (CWorld*)0x1000; player->DefinitionManager = (CGameDefinitionManager*)0x2000; player->Component = (CMainGameComponent*)0x3000;
    player->InitInterfaces();
    int bad = 0;
    for (int i = 0; i < 66; ++i) {
        CInputProcessGameBase* p = player->Slot(i);
        if (!p || p->Kind != i || p->World != player->World || p->MyPlayer != player || p->DefinitionManager != player->DefinitionManager || p->Component != player->Component) ++bad;
    }
    CInputProcessGameBase* first = player->Slot(0);
    if (bad || g_live != 66 || g_deleted != 0) { std::printf("00488d10_TEST FAIL\n"); return 1; }
    player->InitInterfaces();
    if (g_live != 66 || g_deleted != 66 || player->Slot(0) == 0 || player->Slot(0)->Kind != 0) { std::printf("00488d10_TEST FAIL\n"); return 1; }
    (void)first;
    delete player;
    if (g_live != 0) { std::printf("00488d10_TEST FAIL\n"); return 1; }
    std::printf("00488d10_TEST PASS\n");
    return 0;
}