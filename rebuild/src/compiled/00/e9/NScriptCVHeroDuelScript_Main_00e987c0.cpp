// NScript::CV_HeroDuelScript::Main() -- the Hero Duel quest main: finish a duel already decided, bind the Thunder / Spectator entity scripts, retire the Hanging Tree cards and set the Mayor's Invitation objective, wait in BowerstoneJail until no competing quest runs, then spawn the WatchForLeaving / ShowEnemyHealth / WatchForHeroEnteringCave / DoMission threads
// Leaf: ?Main@CV_HeroDuelScript@NScript@@UAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E987C0, 2015 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 2 of the CV_HeroDuelScript vtable 0x012ED0D4 (slot 0 scalar deleting dtor 0x00E9C340, slot 1
//   RegisterMain 0x00E98700, slot 3 Init 0x00E986D0; CScriptBase slot 2 is Main); Ego_r.pdb name set
//   ?Main@CV_HeroDuelScript@NScript@@UAEXXZ; the body spawns this class's own WatchForLeaving / ShowEnemyHealth /
//   WatchForHeroEnteringCave / DoMission (name literals 0x012D19EC / 0x012ED120 / 0x012ED104 / 0x012CB73C stored with
//   their member addresses 0x00E99260 / 0x00E99470 / 0x00E9A270 / 0x00E9B690) and binds CThunder::Alloc 0x00E996B0 /
//   CSpectator::Alloc 0x00E997E0 under "Thunder" / "Spectator".
// Callees: IsActiveThreadTerminating 0x00CB7940, CleanUpHeroDuel 0x00E98FB0, operator new 0x00BFEA1A, CCharString
//   0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, AddEntityScriptBinding 0x00CB8230 (binding vtable 0x012ED15C),
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc<CV_HeroDuelScript> vtable 0x012ED0E8),
//   AddSpawnedFunction 0x00CB7E50; CGameScriptInterfaceBase slots 7 NewScriptFrame, 11 IsLevelLoaded, 64
//   PostAddScriptedEntities, 284 IsQuestActive, 293 RemoveQuestCardFromGuild, 296 SetQuestCardObjective, 565
//   SetTeleportingAsActive.
// Layout: CQ_SunnyvaleMasterData DefeatedThunder +0x61 / LostToThunder +0x62 (engine header, = Ego_r.pdb).
// Shape: the BowerstoneJail wait is the landed DoMission 0x00E9B690 loop (same TU) verbatim; the binding / spawn
//   statements are the landed quest-Main model (CV_MayorsInvitationScript::Main 0x00EB6D70): one ebx flag word guards
//   every name temporary that exists only when its operator new succeeded. Matched on the first grade.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CV_HeroDuelScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
class CEntityScriptBase;
namespace NScript {
class CV_HeroDuelScript::CThunder {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HeroDuelScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E996B0
};
class CV_HeroDuelScript::CSpectator {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_HeroDuelScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E997E0
};
}
void NScript::CV_HeroDuelScript::Main()
{
    if (MasterScriptData->DefeatedThunder || MasterScriptData->LostToThunder) {
        if (IsActiveThreadTerminating()) return;
        Game->SetTeleportingAsActive(false);
        CleanUpHeroDuel();
    }
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HeroDuelScript>("Thunder", this, &CThunder::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_HeroDuelScript>("Spectator", this, &CSpectator::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->RemoveQuestCardFromGuild("Q_HangingTreeEvil");
    Game->RemoveQuestCardFromGuild("Q_HangingTreeGood");
    Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_07", "BowerstoneJail", "");
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->IsQuestActive("Q_AmbushTraders") || Game->IsQuestActive("Q_HangingTreeEvil") ||
            Game->IsQuestActive("Q_HangingTreeGood") || Game->IsQuestActive("Q_EndGameFocalSites") ||
            Game->IsQuestActive("Q_MinionClifftopChase")) {
            if (IsActiveThreadTerminating()) return;
            while (Game->IsQuestActive("Q_AmbushTraders") || Game->IsQuestActive("Q_HangingTreeEvil") ||
                   Game->IsQuestActive("Q_HangingTreeGood") || Game->IsQuestActive("Q_EndGameFocalSites") ||
                   Game->IsQuestActive("Q_MinionClifftopChase")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            while (Game->IsLevelLoaded("BowerstoneJail") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    AddSpawnedFunction(new CSpawnedFunc<CV_HeroDuelScript>("WatchForLeaving", 0, this, &CV_HeroDuelScript::WatchForLeaving), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_HeroDuelScript>("ShowEnemyHealth", 0, this, &CV_HeroDuelScript::ShowEnemyHealth), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_HeroDuelScript>("WatchForHeroEnteringCave", 0, this, &CV_HeroDuelScript::WatchForHeroEnteringCave), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_HeroDuelScript>("DoMission", 0, this, &CV_HeroDuelScript::DoMission), CCharString(""));
}