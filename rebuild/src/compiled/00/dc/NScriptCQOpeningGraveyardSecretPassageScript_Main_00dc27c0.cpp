// NScript::CQ_OpeningGraveyardSecretPassageScript::Main() -- the graveyard secret passage quest main: reset the variables, bind the fourteen entity scripts, spawn WatchForTermination; unless the demon door was already opened, set objective 1, wait for GraveYard_1, activate Q_UndeadRising, kick off the quest start screen once, wait for the gravekeeper cutscene trigger, play CS_OPENGRAVE_INTRO with the hero, keeper and friend, have the keeper speak TEXT_QST_049_GRAVEKEEPER_UNDEAD after the undead cutscene, wait for the skeleton visit, spawn WatchForArmour, wait for finished_quest and remove the skeleton's minimap marker; then mark the door opened, wait for GraveyardPath, spawn CheckDoor, wait for GraveYard_2, set objective 5 and wait for Q_UndeadRising to end
// Leaf: ?Main@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC27C0, 4134 bytes, thiscall, plain `ret`, no EH frame; three destructor cascades (the inner scope
//   before and after the unused CScriptThing local, and its normal end), outer exits are a bare return.
// Identity (proven): slot 2 (Main) of the quest-script vtable; Ego_r.pdb ?Main@CQ_OpeningGraveyardSecretPassageScript@NScript@@UAEXXZ. Bound
//   Allocs are the landed CQ_OpeningGraveyardSecretPassageScript::C*::Alloc rows (binding literal = class name less its C) except
//   CSecretPassageSkeleton::Alloc 0x00DC3D70 (not landed; LABELS.tsv); threads are the landed WatchForTermination 0x00DC3BD0, WatchForArmour
//   0x00DC37F0 and CheckDoor 0x00DC8C80; InitializeVariables 0x00DC3CF0 is landed.
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 / IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase 0x00CDD450,
//   CScriptThing 0x006E7B40 / IsAlive 0x004AB130 / ~ 0x004AA840, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator=
//   0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770, movie ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D.
// Layout: engine/CQ_OpeningGraveyardSecretPassageScript.h (repo, unchanged): finished_quest +0x48, skeleton_visited +0x4B,
//   GravekeeperIntroduced +0x4D, GravekeeperCutsceneStart +0x4E, UndeadCutscenePlayed +0x4F, UndeadCutsceneStarted +0x50, MissionSucceeded
//   +0x55, QuestStartScreenShown +0x71, HaveOpenedDemonDoor +0x72.
// Shape: the landed quest-Main binding model (CQ_RansomVictimScript::Main 0x00DE0850). The quest-active wait is spelled `!= false`
//   (retail test al,al / setne bl; a plain bool condition gives mov bl,al), as in CQGuildTrainingDepartureScript::Main 0x00D50800.
//   The CScriptThing local `skeleton` is constructed (0x006E7B40) and never used, as in retail.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();

void NScript::CQ_OpeningGraveyardSecretPassageScript::Main()
{
    InitializeVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SpeakingMarker", this, &CSpeakingMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("UndeadLieutenant", this, &CUndeadLieutenant::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageSkeleton", this, &CSecretPassageSkeleton::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageFirstGate", this, &CSecretPassageFirstGate::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageGate", this, &CSecretPassageGate::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageGraveyardKeeper", this, &CSecretPassageGraveyardKeeper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageSarcophagos", this, &CSecretPassageSarcophagos::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageMan", this, &CSecretPassageMan::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageWoman", this, &CSecretPassageWoman::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("SecretPassageKeeperFriend", this, &CSecretPassageKeeperFriend::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("StartGravekeeperCutsceneMarker", this, &CStartGravekeeperCutsceneMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("UndeadSpawnCutscene", this, &CUndeadSpawnCutscene::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("GraveyardDemon", this, &CGraveyardDemon::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_OpeningGraveyardSecretPassageScript>("CeremonialEffect", this, &CCeremonialEffect::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_OpeningGraveyardSecretPassageScript>("WatchForTermination", 0, this, &CQ_OpeningGraveyardSecretPassageScript::WatchForTermination), CCharString(""));
    if (!HaveOpenedDemonDoor) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_01", "Graveyard", "Graveyard");
        while (!Game->IsLevelLoaded("GraveYard_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("Q_UndeadRising");
        if (!QuestStartScreenShown) {
            if (IsActiveThreadTerminating())
                return;
            Game->KickOffQuestStartScreen("Q_OpeningGraveyardSecretPassage", true, false);
            QuestStartScreenShown = true;
        }
        while (!GravekeeperCutsceneStart) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase seh_keeper;
        CScriptGameResourceObjectScriptedThingBase seh_friend;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptThing keeper = Game->GetThingWithScriptName("SecretPassageGraveyardKeeper");
        CScriptThing keeper_friend = Game->GetThingWithScriptName("SecretPassageKeeperFriend");
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(keeper_friend, seh_friend, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(keeper, seh_keeper, SCRIPT_AI_PRIORITY_HIGHEST);
        {
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["KEEPER"] = seh_keeper;
            mymap["FRIEND"] = seh_friend;
            Game->FixMovieSequenceCamera(true);
            CScriptGameResourceObjectMovieBase intro_movie;
            Game->StartMovieSequence("", intro_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_OPENGRAVE_INTRO", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            seh_hero.Release();
        }
        GravekeeperIntroduced = true;
        if (seh_friend.IsValid()) seh_friend.Release();
        if (seh_keeper.IsValid()) seh_keeper.Release();
        while (!UndeadCutsceneStarted) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing skeleton;
        if (keeper.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_keeper.IsValid()) seh_keeper.Release();
            while (!Game->StartScriptingEntity(keeper, seh_keeper, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(keeper, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_049_GRAVEKEEPER_UNDEAD", false, keeper, Game->GetHero());
        }
        UndeadCutscenePlayed = true;
        if (keeper.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_keeper.IsValid()) seh_keeper.Release();
        }
        while (!skeleton_visited) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        AddSpawnedFunction(new CSpawnedFunc<CQ_OpeningGraveyardSecretPassageScript>("WatchForArmour", 0, this, &CQ_OpeningGraveyardSecretPassageScript::WatchForArmour), CCharString(""));
        while (!finished_quest) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->MiniMapRemoveMarker(Game->GetThingWithScriptName("SecretPassageSkeleton"));
    }
    HaveOpenedDemonDoor = true;
    while (!Game->IsLevelLoaded("GraveyardPath")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_OpeningGraveyardSecretPassageScript>("CheckDoor", 0, this, &CQ_OpeningGraveyardSecretPassageScript::CheckDoor), CCharString(""));
    while (!Game->IsLevelLoaded("GraveYard_2")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_05", "", "Graveyard");
    while (Game->IsQuestActive("Q_UndeadRising") != false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    MissionSucceeded = true;
}