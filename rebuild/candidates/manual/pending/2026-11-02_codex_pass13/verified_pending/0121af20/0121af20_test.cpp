#include <cassert>
#include <cstdio>
enum EPlayerMode { PLAYER_MODE_NULL=0, PLAYER_MODE_CONTROL_CREATURE=1, PLAYER_MODE_Z_TARGET=2, PLAYER_MODE_DEAD=3, PLAYER_MODE_NAVIGATE_INVENTORY=4, PLAYER_MODE_NAVIGATE_INVENTORY_CLOTHING=5, PLAYER_MODE_NAVIGATE_INVENTORY_WEAPONS=6, PLAYER_MODE_NAVIGATE_INVENTORY_ABILITIES_SCREEN=7, PLAYER_MODE_NAVIGATE_INVENTORY_MAP_SCREEN=8, PLAYER_MODE_NAVIGATE_INVENTORY_MAGIC_SCREEN=9, PLAYER_MODE_NAVIGATE_INVENTORY_STATS_SCREEN=10, PLAYER_MODE_NAVIGATE_INVENTORY_EXPERIENCE_SCREEN=11, PLAYER_MODE_NAVIGATE_INVENTORY_TRADE_SCREEN=12, PLAYER_MODE_NAVIGATE_INVENTORY_QUESTS_SCREEN=13, PLAYER_MODE_CLICK_PAST_TEXT=14, PLAYER_MODE_YES_NO_QUESTION=15, PLAYER_MODE_FIRST_PERSON=16, PLAYER_MODE_FREEZE_CONTROLS=17, PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS=18, PLAYER_MODE_CONTROL_CAMERA=19, PLAYER_MODE_LOOK_THROUGH_WINDOW=20, PLAYER_MODE_REBOOT_GAME=21, PLAYER_MODE_FIRST_PERSON_TARGETING=22, PLAYER_MODE_NAVIGATE_IN_GAME_MENU=23, PLAYER_MODE_CONTROL_SPIRIT=24, PLAYER_MODE_USE_PROJECTILE_WEAPON=25, PLAYER_MODE_TAVERN_GAME=26, PLAYER_MODE_CUT_SCENE=27, PLAYER_MODE_CHARGE_QUICK_ACCESS=28, PLAYER_MODE_FISHING=29, PLAYER_MODE_DIGGING=30, PLAYER_MODE_PARALYSED=31, PLAYER_MODE_BOAST_UI=32, PLAYER_MODE_BERSERK=33, PLAYER_MODE_USE_PROJECTILE_WEAPON_THIRD_PERSON=34, PLAYER_MODE_CHARGE_UP_WILL_SPELL=35, PLAYER_MODE_QUICK_ACCESS_MENU=36, PLAYER_MODE_QUEST_COMPLETION_UI=37, PLAYER_MODE_CREDITS_UI=38, PLAYER_MODE_BETTING=39, PLAYER_MODE_LIGHTNING=40, PLAYER_MODE_ORACLE_MINIGAME=41, PLAYER_MODE_FIREHEART_MINIGAME=42, PLAYER_MODE_LIVE_GUI=43, PLAYER_MODE_CONSOLE=44, PLAYER_MODE_TAKE_SCREENSHOT_FOR_PHOTOJOURNAL=45, PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING=46, PLAYER_MODE_VIEW_HERO=47, MAX_NO_PLAYER_MODES=48 };

class CProcessedInput {public:int marker;};
class CPlayer;
class CPlayerModeInfo {public:void(CPlayer::*InitFunc)(EPlayerMode,CProcessedInput*);void(CPlayer::*UninitFunc)(EPlayerMode,CProcessedInput*);};
class CPlayer {friend int main();
private:
static const CPlayerModeInfo PlayerModeInfo[MAX_NO_PLAYER_MODES];
public:int calls,last_id;EPlayerMode last_mode;CProcessedInput*last_input;CPlayer():calls(0),last_id(-1),last_mode(PLAYER_MODE_NULL),last_input(0){}
private:
void Record(int id,EPlayerMode mode,CProcessedInput*input){++calls;last_id=id;last_mode=mode;last_input=input;}
void InitModeControlCreature(EPlayerMode mode,CProcessedInput*input){Record(2,mode,input);}
void UninitModeControlCreature(EPlayerMode mode,CProcessedInput*input){Record(3,mode,input);}
void InitModeZTarget(EPlayerMode mode,CProcessedInput*input){Record(4,mode,input);}
void UninitModeZTarget(EPlayerMode mode,CProcessedInput*input){Record(5,mode,input);}
void InitModeDead(EPlayerMode mode,CProcessedInput*input){Record(6,mode,input);}
void UninitModeDead(EPlayerMode mode,CProcessedInput*input){Record(7,mode,input);}
void InitModeNavigateInventory(EPlayerMode mode,CProcessedInput*input){Record(8,mode,input);}
void UninitModeNavigateInventory(EPlayerMode mode,CProcessedInput*input){Record(9,mode,input);}
void InitModeNavigateInventoryClothing(EPlayerMode mode,CProcessedInput*input){Record(10,mode,input);}
void UninitModeNavigateInventoryClothing(EPlayerMode mode,CProcessedInput*input){Record(11,mode,input);}
void InitModeNavigateInventoryWeapons(EPlayerMode mode,CProcessedInput*input){Record(12,mode,input);}
void UninitModeNavigateInventoryWeapons(EPlayerMode mode,CProcessedInput*input){Record(13,mode,input);}
void InitModeNavigateInventoryAbilitiesScreen(EPlayerMode mode,CProcessedInput*input){Record(14,mode,input);}
void UninitModeNavigateInventoryAbilitiesScreen(EPlayerMode mode,CProcessedInput*input){Record(15,mode,input);}
void InitModeNavigateInventoryMapScreen(EPlayerMode mode,CProcessedInput*input){Record(16,mode,input);}
void UninitModeNavigateInventoryMapScreen(EPlayerMode mode,CProcessedInput*input){Record(17,mode,input);}
void InitModeNavigateInventoryMagicScreen(EPlayerMode mode,CProcessedInput*input){Record(18,mode,input);}
void UninitModeNavigateInventoryMagicScreen(EPlayerMode mode,CProcessedInput*input){Record(19,mode,input);}
void InitModeNavigateInventoryStatsScreen(EPlayerMode mode,CProcessedInput*input){Record(20,mode,input);}
void UninitModeNavigateInventoryStatsScreen(EPlayerMode mode,CProcessedInput*input){Record(21,mode,input);}
void InitModeNavigateInventoryExperienceScreen(EPlayerMode mode,CProcessedInput*input){Record(22,mode,input);}
void UninitModeNavigateInventoryExperienceScreen(EPlayerMode mode,CProcessedInput*input){Record(23,mode,input);}
void InitModeNavigateInventoryTradeScreen(EPlayerMode mode,CProcessedInput*input){Record(24,mode,input);}
void UninitModeNavigateInventoryTradeScreen(EPlayerMode mode,CProcessedInput*input){Record(25,mode,input);}
void InitModeNavigateInventoryQuestsScreen(EPlayerMode mode,CProcessedInput*input){Record(26,mode,input);}
void UninitModeNavigateInventoryQuestsScreen(EPlayerMode mode,CProcessedInput*input){Record(27,mode,input);}
void InitModeClickPastText(EPlayerMode mode,CProcessedInput*input){Record(28,mode,input);}
void UninitModeClickPastText(EPlayerMode mode,CProcessedInput*input){Record(29,mode,input);}
void InitModeYesNoQuestion(EPlayerMode mode,CProcessedInput*input){Record(30,mode,input);}
void UninitModeYesNoQuestion(EPlayerMode mode,CProcessedInput*input){Record(31,mode,input);}
void InitModeFirstPerson(EPlayerMode mode,CProcessedInput*input){Record(32,mode,input);}
void UninitModeFirstPerson(EPlayerMode mode,CProcessedInput*input){Record(33,mode,input);}
void InitModeFreezeControls(EPlayerMode mode,CProcessedInput*input){Record(34,mode,input);}
void UninitModeFreezeControls(EPlayerMode mode,CProcessedInput*input){Record(35,mode,input);}
void InitModeSpecialAbilityFreezeControls(EPlayerMode mode,CProcessedInput*input){Record(36,mode,input);}
void UninitModeSpecialAbilityFreezeControls(EPlayerMode mode,CProcessedInput*input){Record(37,mode,input);}
void InitModeControlFreeCamera(EPlayerMode mode,CProcessedInput*input){Record(38,mode,input);}
void UninitModeControlFreeCamera(EPlayerMode mode,CProcessedInput*input){Record(39,mode,input);}
void InitModeLookThroughWindow(EPlayerMode mode,CProcessedInput*input){Record(40,mode,input);}
void UninitModeLookThroughWindow(EPlayerMode mode,CProcessedInput*input){Record(41,mode,input);}
void InitModeRebootGame(EPlayerMode mode,CProcessedInput*input){Record(42,mode,input);}
void UninitModeRebootGame(EPlayerMode mode,CProcessedInput*input){Record(43,mode,input);}
void InitModeFirstPersonTargeting(EPlayerMode mode,CProcessedInput*input){Record(44,mode,input);}
void UninitModeFirstPersonTargeting(EPlayerMode mode,CProcessedInput*input){Record(45,mode,input);}
void InitModeNavigateInGameMenu(EPlayerMode mode,CProcessedInput*input){Record(46,mode,input);}
void UninitModeNavigateInGameMenu(EPlayerMode mode,CProcessedInput*input){Record(47,mode,input);}
void InitModeControlSpirit(EPlayerMode mode,CProcessedInput*input){Record(48,mode,input);}
void UninitModeControlSpirit(EPlayerMode mode,CProcessedInput*input){Record(49,mode,input);}
void InitModeUseProjectileWeapon(EPlayerMode mode,CProcessedInput*input){Record(50,mode,input);}
void UninitModeUseProjectileWeapon(EPlayerMode mode,CProcessedInput*input){Record(51,mode,input);}
void InitModeTavernGame(EPlayerMode mode,CProcessedInput*input){Record(52,mode,input);}
void UninitModeTavernGame(EPlayerMode mode,CProcessedInput*input){Record(53,mode,input);}
void InitModeCutScene(EPlayerMode mode,CProcessedInput*input){Record(54,mode,input);}
void UninitModeCutScene(EPlayerMode mode,CProcessedInput*input){Record(55,mode,input);}
void InitModeChargeQuickAccess(EPlayerMode mode,CProcessedInput*input){Record(56,mode,input);}
void UninitModeChargeQuickAccess(EPlayerMode mode,CProcessedInput*input){Record(57,mode,input);}
void InitModeFishing(EPlayerMode mode,CProcessedInput*input){Record(58,mode,input);}
void UninitModeFishing(EPlayerMode mode,CProcessedInput*input){Record(59,mode,input);}
void InitModeDigging(EPlayerMode mode,CProcessedInput*input){Record(60,mode,input);}
void UninitModeDigging(EPlayerMode mode,CProcessedInput*input){Record(61,mode,input);}
void InitModeParalysed(EPlayerMode mode,CProcessedInput*input){Record(62,mode,input);}
void UninitModeParalysed(EPlayerMode mode,CProcessedInput*input){Record(63,mode,input);}
void InitModeBoastUI(EPlayerMode mode,CProcessedInput*input){Record(64,mode,input);}
void UninitModeBoastUI(EPlayerMode mode,CProcessedInput*input){Record(65,mode,input);}
void InitModeBerserk(EPlayerMode mode,CProcessedInput*input){Record(66,mode,input);}
void UninitModeBerserk(EPlayerMode mode,CProcessedInput*input){Record(67,mode,input);}
void InitModeUseProjectileWeaponThirdPerson(EPlayerMode mode,CProcessedInput*input){Record(68,mode,input);}
void UninitModeUseProjectileWeaponThirdPerson(EPlayerMode mode,CProcessedInput*input){Record(69,mode,input);}
void InitModeChargeUpWillSpell(EPlayerMode mode,CProcessedInput*input){Record(70,mode,input);}
void UninitModeChargeUpWillSpell(EPlayerMode mode,CProcessedInput*input){Record(71,mode,input);}
void InitModeQuickAccessMenu(EPlayerMode mode,CProcessedInput*input){Record(72,mode,input);}
void UninitModeQuickAccessMenu(EPlayerMode mode,CProcessedInput*input){Record(73,mode,input);}
void InitModeQuestCompletionUI(EPlayerMode mode,CProcessedInput*input){Record(74,mode,input);}
void UninitModeQuestCompletionUI(EPlayerMode mode,CProcessedInput*input){Record(75,mode,input);}
void InitModeCreditsUI(EPlayerMode mode,CProcessedInput*input){Record(76,mode,input);}
void UninitModeCreditsUI(EPlayerMode mode,CProcessedInput*input){Record(77,mode,input);}
void InitModeBetting(EPlayerMode mode,CProcessedInput*input){Record(78,mode,input);}
void UninitModeBetting(EPlayerMode mode,CProcessedInput*input){Record(79,mode,input);}
void InitModeLightning(EPlayerMode mode,CProcessedInput*input){Record(80,mode,input);}
void UninitModeLightning(EPlayerMode mode,CProcessedInput*input){Record(81,mode,input);}
void InitModeOracleMinigame(EPlayerMode mode,CProcessedInput*input){Record(82,mode,input);}
void UninitModeOracleMinigame(EPlayerMode mode,CProcessedInput*input){Record(83,mode,input);}
void InitModeFireheartMinigame(EPlayerMode mode,CProcessedInput*input){Record(84,mode,input);}
void UninitModeFireheartMinigame(EPlayerMode mode,CProcessedInput*input){Record(85,mode,input);}
void InitModeLiveGUI(EPlayerMode mode,CProcessedInput*input){Record(86,mode,input);}
void UninitModeLiveGUI(EPlayerMode mode,CProcessedInput*input){Record(87,mode,input);}
void InitModeConsole(EPlayerMode mode,CProcessedInput*input){Record(88,mode,input);}
void UninitModeConsole(EPlayerMode mode,CProcessedInput*input){Record(89,mode,input);}
void InitModeTakePhotojournalScreenshot(EPlayerMode mode,CProcessedInput*input){Record(90,mode,input);}
void UninitModeTakePhotojournalScreenshot(EPlayerMode mode,CProcessedInput*input){Record(91,mode,input);}
void InitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode,CProcessedInput*input){Record(92,mode,input);}
void UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode,CProcessedInput*input){Record(93,mode,input);}
void InitModeViewHero(EPlayerMode mode,CProcessedInput*input){Record(94,mode,input);}
void UninitModeViewHero(EPlayerMode mode,CProcessedInput*input){Record(95,mode,input);}
};

const CPlayerModeInfo CPlayer::PlayerModeInfo[MAX_NO_PLAYER_MODES] = {
    {0, 0},
    {&CPlayer::InitModeControlCreature, &CPlayer::UninitModeControlCreature},
    {&CPlayer::InitModeZTarget, &CPlayer::UninitModeZTarget},
    {&CPlayer::InitModeDead, &CPlayer::UninitModeDead},
    {&CPlayer::InitModeNavigateInventory, &CPlayer::UninitModeNavigateInventory},
    {&CPlayer::InitModeNavigateInventoryClothing, &CPlayer::UninitModeNavigateInventoryClothing},
    {&CPlayer::InitModeNavigateInventoryWeapons, &CPlayer::UninitModeNavigateInventoryWeapons},
    {&CPlayer::InitModeNavigateInventoryAbilitiesScreen, &CPlayer::UninitModeNavigateInventoryAbilitiesScreen},
    {&CPlayer::InitModeNavigateInventoryMapScreen, &CPlayer::UninitModeNavigateInventoryMapScreen},
    {&CPlayer::InitModeNavigateInventoryMagicScreen, &CPlayer::UninitModeNavigateInventoryMagicScreen},
    {&CPlayer::InitModeNavigateInventoryStatsScreen, &CPlayer::UninitModeNavigateInventoryStatsScreen},
    {&CPlayer::InitModeNavigateInventoryExperienceScreen, &CPlayer::UninitModeNavigateInventoryExperienceScreen},
    {&CPlayer::InitModeNavigateInventoryTradeScreen, &CPlayer::UninitModeNavigateInventoryTradeScreen},
    {&CPlayer::InitModeNavigateInventoryQuestsScreen, &CPlayer::UninitModeNavigateInventoryQuestsScreen},
    {&CPlayer::InitModeClickPastText, &CPlayer::UninitModeClickPastText},
    {&CPlayer::InitModeYesNoQuestion, &CPlayer::UninitModeYesNoQuestion},
    {&CPlayer::InitModeFirstPerson, &CPlayer::UninitModeFirstPerson},
    {&CPlayer::InitModeFreezeControls, &CPlayer::UninitModeFreezeControls},
    {&CPlayer::InitModeSpecialAbilityFreezeControls, &CPlayer::UninitModeSpecialAbilityFreezeControls},
    {&CPlayer::InitModeControlFreeCamera, &CPlayer::UninitModeControlFreeCamera},
    {&CPlayer::InitModeLookThroughWindow, &CPlayer::UninitModeLookThroughWindow},
    {&CPlayer::InitModeRebootGame, &CPlayer::UninitModeRebootGame},
    {&CPlayer::InitModeFirstPersonTargeting, &CPlayer::UninitModeFirstPersonTargeting},
    {&CPlayer::InitModeNavigateInGameMenu, &CPlayer::UninitModeNavigateInGameMenu},
    {&CPlayer::InitModeControlSpirit, &CPlayer::UninitModeControlSpirit},
    {&CPlayer::InitModeUseProjectileWeapon, &CPlayer::UninitModeUseProjectileWeapon},
    {&CPlayer::InitModeTavernGame, &CPlayer::UninitModeTavernGame},
    {&CPlayer::InitModeCutScene, &CPlayer::UninitModeCutScene},
    {&CPlayer::InitModeChargeQuickAccess, &CPlayer::UninitModeChargeQuickAccess},
    {&CPlayer::InitModeFishing, &CPlayer::UninitModeFishing},
    {&CPlayer::InitModeDigging, &CPlayer::UninitModeDigging},
    {&CPlayer::InitModeParalysed, &CPlayer::UninitModeParalysed},
    {&CPlayer::InitModeBoastUI, &CPlayer::UninitModeBoastUI},
    {&CPlayer::InitModeBerserk, &CPlayer::UninitModeBerserk},
    {&CPlayer::InitModeUseProjectileWeaponThirdPerson, &CPlayer::UninitModeUseProjectileWeaponThirdPerson},
    {&CPlayer::InitModeChargeUpWillSpell, &CPlayer::UninitModeChargeUpWillSpell},
    {&CPlayer::InitModeQuickAccessMenu, &CPlayer::UninitModeQuickAccessMenu},
    {&CPlayer::InitModeQuestCompletionUI, &CPlayer::UninitModeQuestCompletionUI},
    {&CPlayer::InitModeCreditsUI, &CPlayer::UninitModeCreditsUI},
    {&CPlayer::InitModeBetting, &CPlayer::UninitModeBetting},
    {&CPlayer::InitModeLightning, &CPlayer::UninitModeLightning},
    {&CPlayer::InitModeOracleMinigame, &CPlayer::UninitModeOracleMinigame},
    {&CPlayer::InitModeFireheartMinigame, &CPlayer::UninitModeFireheartMinigame},
    {&CPlayer::InitModeLiveGUI, &CPlayer::UninitModeLiveGUI},
    {&CPlayer::InitModeConsole, &CPlayer::UninitModeConsole},
    {&CPlayer::InitModeTakePhotojournalScreenshot, &CPlayer::UninitModeTakePhotojournalScreenshot},
    {&CPlayer::InitModePCProjectileWeaponThirdPersonAiming, &CPlayer::UninitModePCProjectileWeaponThirdPersonAiming},
    {&CPlayer::InitModeViewHero, &CPlayer::UninitModeViewHero},
};

int main(){
 assert(MAX_NO_PLAYER_MODES==48);
 assert(CPlayer::PlayerModeInfo[0].InitFunc==0&&CPlayer::PlayerModeInfo[0].UninitFunc==0);
 CPlayer players[4];CProcessedInput inputs[4];for(int p=0;p<4;++p)inputs[p].marker=p*17;
 int invocations=0;
 for(int entry=1;entry<48;++entry){
  const CPlayerModeInfo&info=CPlayer::PlayerModeInfo[entry];assert(info.InitFunc!=0&&info.UninitFunc!=0&&info.InitFunc!=info.UninitFunc);
  for(int earlier=1;earlier<entry;++earlier){const CPlayerModeInfo&other=CPlayer::PlayerModeInfo[earlier];assert(info.InitFunc!=other.InitFunc&&info.InitFunc!=other.UninitFunc&&info.UninitFunc!=other.InitFunc&&info.UninitFunc!=other.UninitFunc);}
  for(int p=0;p<4;++p)for(int mode=0;mode<48;++mode)for(int null_input=0;null_input<2;++null_input){
   CProcessedInput*input=null_input?0:&inputs[p];int before[4];for(int q=0;q<4;++q)before[q]=players[q].calls;
   (players[p].*info.InitFunc)((EPlayerMode)mode,input);assert(players[p].last_id==entry*2&&players[p].last_mode==mode&&players[p].last_input==input);
   (players[p].*info.UninitFunc)((EPlayerMode)mode,input);assert(players[p].last_id==entry*2+1&&players[p].last_mode==mode&&players[p].last_input==input);
   for(int q=0;q<4;++q)assert(players[q].calls==before[q]+(p==q?2:0));
   assert(inputs[p].marker==p*17);invocations+=2;
  }
 }
 assert(invocations==36096);puts("0121af20_TEST PASS");
}
