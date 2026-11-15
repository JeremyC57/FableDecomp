// Behaviour test for 0x0121B9B0 CMainGameComponent::`dynamic initializer for 'EventDispatch'': the 87-entry game-event dispatch
// table. Every slot's action / local-action handler is called through the table on a CWorld and must be the handler of that
// event type (expected codes listed independently, per event); unset slots must be null pointers ({0,0,0,-1} in VC7.1's
// 16-byte unspecified-inheritance form, the words the retail initializer stores) and the display-engine pointers are all null.
#include <cstdio>
#include <cstring>
class CGameEvent { public: int Type; };
class CWorld; class CDisplayEngine;
class CGameEventDispatch { public:
    void (CWorld::*ActionFunction)(const CGameEvent* event);
    void (CWorld::*LocalActionFunction)(const CGameEvent* event);
    void (CDisplayEngine::*FeedbackFunction)(const CGameEvent* event);
    void (CDisplayEngine::*ConfirmFunction)(const CGameEvent* event);
};
class CWorld { public:
    int LastCode, LastType, Calls; CWorld() : LastCode(0), LastType(-1), Calls(0) {}
    void Record(int code, const CGameEvent* event) { LastCode = code; LastType = event->Type; ++Calls; }
    void EAAnswerQuestion(const CGameEvent* event) { Record(1, event); }
    void EAApplyAllScriptedMapBrushes(const CGameEvent* event) { Record(2, event); }
    void EACentreCamera(const CGameEvent* event) { Record(3, event); }
    void EAChargeQuickAccessItem(const CGameEvent* event) { Record(4, event); }
    void EAChargeUpWillSpell(const CGameEvent* event) { Record(5, event); }
    void EAClickPastText(const CGameEvent* event) { Record(6, event); }
    void EACloseHeroCentreDoor(const CGameEvent* event) { Record(7, event); }
    void EACloseHeroInformationScreen(const CGameEvent* event) { Record(8, event); }
    void EACloseInGameMenu(const CGameEvent* event) { Record(9, event); }
    void EACloseLiveGUI(const CGameEvent* event) { Record(10, event); }
    void EAClosePhotoCapture(const CGameEvent* event) { Record(11, event); }
    void EAConsoleToggleState(const CGameEvent* event) { Record(12, event); }
    void EAControlledCreatureBlockEnd(const CGameEvent* event) { Record(13, event); }
    void EAControlledCreatureDeactivateLightning(const CGameEvent* event) { Record(14, event); }
    void EAControlledCreatureDeactivateZTarget(const CGameEvent* event) { Record(15, event); }
    void EAControlledCreatureDefault(const CGameEvent* event) { Record(16, event); }
    void EAControlledCreatureDropObject(const CGameEvent* event) { Record(17, event); }
    void EAControlledCreatureEndThrowObject(const CGameEvent* event) { Record(18, event); }
    void EAControlledCreatureGiveItemToTarget(const CGameEvent* event) { Record(19, event); }
    void EAControlledCreatureJump(const CGameEvent* event) { Record(20, event); }
    void EAControlledCreatureLearnExpression(const CGameEvent* event) { Record(21, event); }
    void EAControlledCreatureLightning(const CGameEvent* event) { Record(22, event); }
    void EAControlledCreaturePlayerInteraction(const CGameEvent* event) { Record(23, event); }
    void EAControlledCreatureRoll(const CGameEvent* event) { Record(24, event); }
    void EAControlledCreatureSheatheWeapon(const CGameEvent* event) { Record(25, event); }
    void EAControlledCreatureStartSneaking(const CGameEvent* event) { Record(26, event); }
    void EAControlledCreatureStartThrowObject(const CGameEvent* event) { Record(27, event); }
    void EAControlledCreatureStopSneaking(const CGameEvent* event) { Record(28, event); }
    void EAControlledCreatureStrafe(const CGameEvent* event) { Record(29, event); }
    void EAControlledCreatureTalk(const CGameEvent* event) { Record(30, event); }
    void EAControlledCreatureUnfreezeControls(const CGameEvent* event) { Record(31, event); }
    void EAControlledCreatureUnsheatheMeleeWeapon(const CGameEvent* event) { Record(32, event); }
    void EAControlledCreatureUnsheatheRangedWeapon(const CGameEvent* event) { Record(33, event); }
    void EAControlledCreatureUseAbility(const CGameEvent* event) { Record(34, event); }
    void EAControlledCreatureZTarget(const CGameEvent* event) { Record(35, event); }
    void EACreatureBlock(const CGameEvent* event) { Record(36, event); }
    void EACreatureCheat(const CGameEvent* event) { Record(37, event); }
    void EACreatureSprint(const CGameEvent* event) { Record(38, event); }
    void EACreatureUseObject(const CGameEvent* event) { Record(39, event); }
    void EADeleteMenuComponents(const CGameEvent* event) { Record(40, event); }
    void EADeleteTradeActiveMenu(const CGameEvent* event) { Record(41, event); }
    void EAExpressionFollow(const CGameEvent* event) { Record(42, event); }
    void EAExpressionWait(const CGameEvent* event) { Record(43, event); }
    void EAFirstPersonTargeting(const CGameEvent* event) { Record(44, event); }
    void EAFirstPersonViewStart(const CGameEvent* event) { Record(45, event); }
    void EALoadGameFromInGameMenu(const CGameEvent* event) { Record(46, event); }
    void EAMoveCreature(const CGameEvent* event) { Record(47, event); }
    void EAMoveHeroToRegion(const CGameEvent* event) { Record(48, event); }
    void EAMoveSpirit(const CGameEvent* event) { Record(49, event); }
    void EAOpenHeroCentreDoor(const CGameEvent* event) { Record(50, event); }
    void EAOpenHeroInformationScreen(const CGameEvent* event) { Record(51, event); }
    void EAOpenPCBuyTradingMenu(const CGameEvent* event) { Record(52, event); }
    void EAOpenPCInventoryMenu(const CGameEvent* event) { Record(53, event); }
    void EAOpenPCMSNChatMenu(const CGameEvent* event) { Record(54, event); }
    void EAOpenPCMapMenu(const CGameEvent* event) { Record(55, event); }
    void EAOpenPCOptionsMenu(const CGameEvent* event) { Record(56, event); }
    void EAOpenPCPhotoJournalMenu(const CGameEvent* event) { Record(57, event); }
    void EAOpenPCScoreboard(const CGameEvent* event) { Record(58, event); }
    void EAOpenPCSellTradingMenu(const CGameEvent* event) { Record(59, event); }
    void EAOpenPCSkillsMenu(const CGameEvent* event) { Record(60, event); }
    void EAOpenPCStatusMenu(const CGameEvent* event) { Record(61, event); }
    void EAOpenPCWantedTradingMenu(const CGameEvent* event) { Record(62, event); }
    void EAPlayerRespawn(const CGameEvent* event) { Record(63, event); }
    void EAPutAway(const CGameEvent* event) { Record(64, event); }
    void EARemoveCurrentMode(const CGameEvent* event) { Record(65, event); }
    void EASetFreeCameraMode(const CGameEvent* event) { Record(66, event); }
    void EASetPauseMode(const CGameEvent* event) { Record(67, event); }
    void EASetSlowMotion(const CGameEvent* event) { Record(68, event); }
    void EASkipCutScene(const CGameEvent* event) { Record(69, event); }
    void EASpiritAttack(const CGameEvent* event) { Record(70, event); }
    void EASpiritReturnToHero(const CGameEvent* event) { Record(71, event); }
    void EATakeTheBloodyScreenshot(const CGameEvent* event) { Record(72, event); }
    void EATempWorldEventForWhateverYouWant(const CGameEvent* event) { Record(73, event); }
    void EAToggleKillEverythingMode(const CGameEvent* event) { Record(74, event); }
    void EATogglePassiveAggressiveMode(const CGameEvent* event) { Record(75, event); }
    void EAToggleViewHeroMode(const CGameEvent* event) { Record(76, event); }
    void EAUpdateFrame(const CGameEvent* event) { Record(77, event); }
    void EAUseFreeCamera(const CGameEvent* event) { Record(78, event); }
    void EAUseProjectileWeapon(const CGameEvent* event) { Record(79, event); }
    void EAUseProjectileWeaponThirdPerson(const CGameEvent* event) { Record(80, event); }
    void EAUseQuickAccessItem(const CGameEvent* event) { Record(81, event); }
    void EAUseQuickAccessItemInCutscene(const CGameEvent* event) { Record(82, event); }
    void LEASetDisplayMode(const CGameEvent* event) { Record(83, event); }
    void LEASetEditorMode(const CGameEvent* event) { Record(84, event); }
    void LEASetExclusiveMode(const CGameEvent* event) { Record(85, event); }
};
class CDisplayEngine { public: int Unused; };
class CMainGameComponent { friend int main(); static const CGameEventDispatch EventDispatch[87]; };
const CGameEventDispatch CMainGameComponent::EventDispatch[87] = {
    { NULL, NULL, NULL, NULL },
    { &CWorld::EAUpdateFrame, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL },
    { NULL, &CWorld::LEASetExclusiveMode, NULL, NULL },
    { NULL, &CWorld::LEASetDisplayMode, NULL, NULL },
    { NULL, &CWorld::LEASetEditorMode, NULL, NULL },
    { &CWorld::EAFirstPersonViewStart, NULL, NULL, NULL },
    { &CWorld::EACreatureCheat, NULL, NULL, NULL },
    { &CWorld::EAPlayerRespawn, NULL, NULL, NULL },
    { &CWorld::EASetPauseMode, NULL, NULL, NULL },
    { &CWorld::EASetSlowMotion, NULL, NULL, NULL },
    { &CWorld::EASetFreeCameraMode, NULL, NULL, NULL },
    { &CWorld::EAUseFreeCamera, NULL, NULL, NULL },
    { &CWorld::EACreatureBlock, NULL, NULL, NULL },
    { &CWorld::EATempWorldEventForWhateverYouWant, NULL, NULL, NULL },
    { &CWorld::EAApplyAllScriptedMapBrushes, NULL, NULL, NULL },
    { &CWorld::EAMoveCreature, NULL, NULL, NULL },
    { &CWorld::EACreatureUseObject, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureTalk, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureLearnExpression, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureDropObject, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureZTarget, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureUseAbility, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureStartSneaking, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureStopSneaking, NULL, NULL, NULL },
    { &CWorld::EAClickPastText, NULL, NULL, NULL },
    { &CWorld::EAOpenHeroInformationScreen, NULL, NULL, NULL },
    { &CWorld::EACloseHeroInformationScreen, NULL, NULL, NULL },
    { &CWorld::EACloseInGameMenu, NULL, NULL, NULL },
    { &CWorld::EAAnswerQuestion, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureStartThrowObject, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureEndThrowObject, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureSheatheWeapon, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureUnsheatheMeleeWeapon, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureUnsheatheRangedWeapon, NULL, NULL, NULL },
    { &CWorld::EAControlledCreaturePlayerInteraction, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureGiveItemToTarget, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureUnfreezeControls, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureRoll, NULL, NULL, NULL },
    { &CWorld::EAUseQuickAccessItem, NULL, NULL, NULL },
    { &CWorld::EAUseQuickAccessItemInCutscene, NULL, NULL, NULL },
    { &CWorld::EAChargeQuickAccessItem, NULL, NULL, NULL },
    { &CWorld::EAPutAway, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureStrafe, NULL, NULL, NULL },
    { &CWorld::EAMoveHeroToRegion, NULL, NULL, NULL },
    { &CWorld::EAFirstPersonTargeting, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureDefault, NULL, NULL, NULL },
    { &CWorld::EAExpressionFollow, NULL, NULL, NULL },
    { &CWorld::EAExpressionWait, NULL, NULL, NULL },
    { &CWorld::EAUseProjectileWeapon, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureJump, NULL, NULL, NULL },
    { &CWorld::EAMoveSpirit, NULL, NULL, NULL },
    { &CWorld::EAOpenHeroCentreDoor, NULL, NULL, NULL },
    { &CWorld::EACloseHeroCentreDoor, NULL, NULL, NULL },
    { &CWorld::EASpiritAttack, NULL, NULL, NULL },
    { &CWorld::EACreatureSprint, NULL, NULL, NULL },
    { &CWorld::EASpiritReturnToHero, NULL, NULL, NULL },
    { &CWorld::EASkipCutScene, NULL, NULL, NULL },
    { &CWorld::EAUseProjectileWeaponThirdPerson, NULL, NULL, NULL },
    { &CWorld::EAChargeUpWillSpell, NULL, NULL, NULL },
    { &CWorld::EALoadGameFromInGameMenu, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureBlockEnd, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureDeactivateZTarget, NULL, NULL, NULL },
    { &CWorld::EARemoveCurrentMode, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureLightning, NULL, NULL, NULL },
    { &CWorld::EAControlledCreatureDeactivateLightning, NULL, NULL, NULL },
    { &CWorld::EACloseLiveGUI, NULL, NULL, NULL },
    { &CWorld::EAClosePhotoCapture, NULL, NULL, NULL },
    { &CWorld::EATakeTheBloodyScreenshot, NULL, NULL, NULL },
    { &CWorld::EAConsoleToggleState, NULL, NULL, NULL },
    { &CWorld::EAOpenPCSkillsMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCMSNChatMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCInventoryMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCOptionsMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCPhotoJournalMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCMapMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCBuyTradingMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCSellTradingMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCWantedTradingMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCStatusMenu, NULL, NULL, NULL },
    { &CWorld::EAOpenPCScoreboard, NULL, NULL, NULL },
    { &CWorld::EADeleteMenuComponents, NULL, NULL, NULL },
    { &CWorld::EATogglePassiveAggressiveMode, NULL, NULL, NULL },
    { &CWorld::EADeleteTradeActiveMenu, NULL, NULL, NULL },
    { &CWorld::EAToggleKillEverythingMode, NULL, NULL, NULL },
    { &CWorld::EAToggleViewHeroMode, NULL, NULL, NULL },
    { &CWorld::EACentreCamera, NULL, NULL, NULL },
};
static const int ExpectedAction[87] = { 0, 77, 0, 0, 0, 0, 45, 37, 63, 67, 68, 66, 78, 36, 73, 2, 47, 39, 30, 21, 17, 35, 34, 26, 28, 6, 51, 8, 9, 1, 27, 18, 25, 32, 33, 23, 19, 31, 24, 81, 82, 4, 64, 29, 48, 44, 16, 42, 43, 79, 20, 49, 50, 7, 70, 38, 71, 69, 80, 5, 46, 13, 15, 65, 22, 14, 10, 11, 72, 12, 60, 54, 53, 56, 57, 55, 52, 59, 62, 61, 58, 40, 75, 41, 74, 76, 3 };
static const int ExpectedLocal[87] = { 0, 0, 0, 85, 83, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int main() {
    if (sizeof(CGameEventDispatch) != 0x40) { std::puts("0121b9b0_TEST FAIL size"); return 1; }
    CWorld world; int set = 0, local = 0;
    for (int type = 0; type < 87; ++type) {
        const CGameEventDispatch& d = CMainGameComponent::EventDispatch[type];
        const unsigned long* w = (const unsigned long*)&d;
        CGameEvent event; event.Type = type;
        for (int k = 0; k < 2; ++k) {
            void (CWorld::*f)(const CGameEvent*) = k ? d.LocalActionFunction : d.ActionFunction;
            int expected = k ? ExpectedLocal[type] : ExpectedAction[type];
            const unsigned long* pw = w + 4 * k;
            if (!expected) {
                if (f || pw[0] != 0 || pw[1] != 0 || pw[2] != 0 || pw[3] != 0xFFFFFFFFul) { std::printf("0121b9b0_TEST FAIL null %d/%d\n", type, k); return 1; }
                continue;
            }
            if (!f || pw[1] != 0 || pw[2] != 0 || pw[3] != 0) { std::printf("0121b9b0_TEST FAIL set %d/%d\n", type, k); return 1; }
            int calls = world.Calls; (world.*f)(&event);
            if (world.LastCode != expected || world.LastType != type || world.Calls != calls + 1) { std::printf("0121b9b0_TEST FAIL call %d/%d\n", type, k); return 1; }
            if (k) ++local; else ++set;
        }
        if (d.FeedbackFunction || d.ConfirmFunction || w[11] != 0xFFFFFFFFul || w[15] != 0xFFFFFFFFul) { std::printf("0121b9b0_TEST FAIL display %d\n", type); return 1; }
        for (int other = 0; other < type; ++other)
            if (ExpectedAction[type] && CMainGameComponent::EventDispatch[other].ActionFunction == d.ActionFunction) { std::printf("0121b9b0_TEST FAIL dup %d\n", type); return 1; }
    }
    if (set != 82 || local != 3) { std::puts("0121b9b0_TEST FAIL count"); return 1; }
    std::puts("0121b9b0_TEST PASS");
    return 0;
}