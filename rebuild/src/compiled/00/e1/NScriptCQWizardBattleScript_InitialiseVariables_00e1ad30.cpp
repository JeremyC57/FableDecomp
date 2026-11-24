// NScript::CQ_WizardBattleScript::InitialiseVariables() -- reset the quest's script variables: the eight flags +0x48..+0x4F cleared,
// MazePhase -1, NumberOfScreamersAlive 0
// Leaf: ?InitialiseVariables@CQ_WizardBattleScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1AD30, 37 bytes, thiscall, plain `ret`.
// Name (NF348, 2026-11-24): FableWin.pdb ?InitialiseVariables@CQ_WizardBattleScript@NScript@@QAEXXZ (absent from the Ego_r.pdb name
//   set); FableWin's Main calls it first, as retail Main 0x00E1A990 calls 0x00E1AD30 first (landed binder, respelled at nf345 b2);
//   FableWin object order Init < Main < InitialiseVariables = retail 0x00E1A870 < 0x00E1A990 < 0x00E1AD30. Was the stand-in
//   ?Init@C@@QAEXXZ (catalog label NScript::CV_ChickenKickingScript::Init, propagated).
// Layout: the shared engine/CQ_WizardBattleScript.h (VillagersTurnOnHero +0x48 .. MazeDefeated +0x4F, MazePhase +0x50,
//   NumberOfScreamersAlive +0x54) with the add-only declaration opt-in FABLE_RETAIL_CQ_WIZARDBATTLESCRIPT_INITIALISEVARIABLES.
//   Store order as retail: +0x48..+0x4E, MazePhase, NumberOfScreamersAlive, then MazeDefeated +0x4F.
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQ_WIZARDBATTLESCRIPT_INITIALISEVARIABLES
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CQ_WizardBattleScript.h"

void NScript::CQ_WizardBattleScript::InitialiseVariables()
{
    VillagersTurnOnHero = false;
    VillagersChased = false;
    FightersStart = false;
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    QuestStartScreenShown = false;
    MazePhase = -1;
    NumberOfScreamersAlive = 0;
    MazeDefeated = false;
}
