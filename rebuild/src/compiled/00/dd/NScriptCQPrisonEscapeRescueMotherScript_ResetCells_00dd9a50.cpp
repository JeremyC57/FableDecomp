// NScript::CQ_PrisonEscapeRescueMotherScript::ResetCells() -- put the hero back in the cells after a race or an escape attempt: either the torture cut-scenes (guard + guard mate, a year passes, a new hairstyle, gear confiscated) or, with the key stolen, a teleport to PrisonHSP; then the guard / accomplice scene that depends on how the race went
// Leaf: ?ResetCells@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD9A50, 4696 bytes incl. the 5-entry switch table at 0x00DDAC94, thiscall, plain `ret`, no EH.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb both name ?ResetCells@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ;
//   FableWin's address order ProcessRaces -> ResetCells matches retail (0x00DD9900, whose body activates Q_PrisonRace and calls this
//   row on `this`, then 0x00DD9A50); every member it touches is this class's (YearsPassedInCell +0x6c, HeroHasFoundWeapons +0x65,
//   PrisonCutsceneFinished +0x6a) and CQ_SunnyvaleMasterData's (PrisonKeyStolenByHero +0xee, PrisonTorturedBefore +0xf2,
//   PrisonRaceWonByHero +0xec, PrisonRaceNumber +0xe8); literals "HeroTortureHSP" .. "TEXT_QST_055_PRISON_PRISONER_HINT_02".
// Callees: IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, CBaseObject()
//   0x0099A380, scripted handle ctor 0x007E72A0 (the later sites; VC7.1 expands the first four) / dtor 0x007E74D0 / operator=
//   0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   CGameScriptInterfaceBase slots 7, 8, 11, 70, 72, 108, 131, 137, 138, 170, 171, 191, 192, 296, 364-366, 368-371, 376, 379,
//   417, 472, 474, 475, 655; CScriptThing slots 6 GetPos, 10 GetAngleXY.
// Shape: the landed same-TU PrisonLifeIntro 0x00DD3B80 (thread check at the head of every if / else arm, WAIT loops) plus the
//   FirstFleeCutscene 0x00E9A970 cut-scene block (CWideScreenMagicPauseEntities keeps Game in edi; its forced-inline ctor, opt-in
//   FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR, takes the pause object out of the frame: without it 0xd0 v 0xcc).
//   Called MovieBase pair and handle dtor / operator= (the OUTOFLINE opt-ins). RELOCATION_MATCH on the second grade.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"

void NScript::CQ_PrisonEscapeRescueMotherScript::ResetCells()
{
    if (!MasterScriptData->PrisonKeyStolenByHero) {
        if (IsActiveThreadTerminating()) return;
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("HeroTortureHSP"), false);
        Game->EntitySetFacingAngle(Game->GetHero(), Game->GetThingWithScriptName("HeroTortureHSP").GetAngleXY(), true);
        while (!Game->IsLevelLoaded("PrisonCells_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing guard = Game->GetThingWithScriptName("HeroTortureGuard");
        CScriptGameResourceObjectScriptedThingBase guard_res;
        if (guard_res.IsValid()) guard_res.Release();
        while (!Game->StartScriptingEntity(guard, guard_res, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing mate = Game->GetThingWithScriptName("HeroTortureGuardMate");
        CScriptGameResourceObjectScriptedThingBase mate_res;
        if (mate_res.IsValid()) mate_res.Release();
        while (!Game->StartScriptingEntity(mate, mate_res, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptGameResourceObjectScriptedThingBase hero_res;
        Game->StartScriptingEntity(Game->GetHero(), hero_res, (EScriptAIPriority)4);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> actors;
        actors["HERO"] = hero_res;
        actors["MATE"] = mate_res;
        actors["GUARD"] = guard_res;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        if (MasterScriptData->PrisonTorturedBefore) {
            if (IsActiveThreadTerminating()) return;
            RunCutsceneMacro("CS_PRISON_TORTURE_1_SHORT", actors, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating()) return;
            RunCutsceneMacro("CS_PRISON_TORTURE_1", actors, 0, 0, false, true);
        }
        Game->PlayAVIMovie("Data\\Video\\7_prison_1_comp.xmv");
        if (++YearsPassedInCell <= 5)
            Game->SetHeroAge(Game->GetHeroAge() + 1.0f);
        else
            YearsPassedInCell = 5;
        switch (YearsPassedInCell) {
        case 0:
            Game->RemoveHeroHairstyle();
            break;
        case 1:
            Game->ChangeHeroHairstyle("OBJECT_HERO_HAIR_YOUNG_01");
            Game->ChangeHeroHairstyle("OBJECT_HERO_BEARD_CHIN_01");
            break;
        case 2:
            Game->ChangeHeroHairstyle("OBJECT_HERO_HAIR_BASIN_01");
            Game->ChangeHeroHairstyle("OBJECT_HERO_BEARD_02");
            Game->ChangeHeroHairstyle("OBJECT_HERO_TASHSMITH_01");
            break;
        case 3:
            Game->ChangeHeroHairstyle("OBJECT_HERO_HAIR");
            Game->ChangeHeroHairstyle("OBJECT_HERO_BEARD_03");
            break;
        case 4:
            Game->ChangeHeroHairstyle("OBJECT_HERO_HAIR_PONYTAIL_01");
            Game->ChangeHeroHairstyle("OBJECT_HERO_BEARD_TRAMP_01");
            break;
        }
        HeroHasFoundWeapons = false;
        Game->SetHeroWillAsUsable(false);
        Game->ConfiscateAllHeroItems();
        Game->ConfiscateAllHeroWeapons();
        Game->SetHeroWillAsUsable(false);
        if (MasterScriptData->PrisonTorturedBefore) {
            if (IsActiveThreadTerminating()) return;
            RunCutsceneMacro("CS_PRISON_TORTURE_2_SHORT", actors, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating()) return;
            RunCutsceneMacro("CS_PRISON_TORTURE_2", actors, 0, 0, false, true);
            MasterScriptData->PrisonTorturedBefore = true;
        }
        Game->FixMovieSequenceCamera(false);
        Game->CameraResetToViewBehindHero(0.1f);
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("PrisonHSP"), false);
        Game->EntitySetFacingAngle(Game->GetHero(), Game->GetThingWithScriptName("PrisonHSP").GetAngleXY(), true);
        while (!Game->IsLevelLoaded("PrisonCells_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    CScriptGameResourceObjectScriptedThingBase guard_res;
    CScriptGameResourceObjectScriptedThingBase accomplice_res;
    CScriptThing guard = Game->GetThingWithScriptName("PrisonGuard");
    CScriptThing accomplice = Game->GetThingWithScriptName("PrisonerAccomplice");
    Game->EntitySetFacingAngleTowardsThing(accomplice, Game->GetHero(), false);
    if (guard_res.IsValid()) guard_res.Release();
    while (!Game->StartScriptingEntity(guard, guard_res, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (accomplice_res.IsValid()) accomplice_res.Release();
    while (!Game->StartScriptingEntity(accomplice, accomplice_res, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (MasterScriptData->PrisonKeyStolenByHero) {
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase hero_res;
            Game->StartScriptingEntity(Game->GetHero(), hero_res, (EScriptAIPriority)4);
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> actors;
            actors["HERO"] = hero_res;
            actors["GUARD"] = guard_res;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_PRISON_ESCAPE_LOSTKEY", actors, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
        }
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_04", "PrisonTortureChamber", "");
        Game->RemoveThing(guard, false, true);
    } else if (MasterScriptData->PrisonRaceWonByHero) {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(guard, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_GUARD_BACKSOON_01", false, guard, Game->GetHero());
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        guard_res.MoveToPosition(Game->GetThingWithScriptName("GuardPatrolExit").GetPos(), 2.0f, (EScriptEntityMoveType)0, false, true);
        Game->Pause(5.0f);
        conversation = Game->AddNewConversation(accomplice, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_SURVIVED_01", false, accomplice, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_HINT_01", false, accomplice, Game->GetHero());
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    } else if (MasterScriptData->PrisonRaceNumber > 1) {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(guard, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_GUARD_BACKSOON_02", false, guard, Game->GetHero());
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        guard_res.MoveToPosition(Game->GetThingWithScriptName("GuardPatrolExit").GetPos(), 2.0f, (EScriptEntityMoveType)0, false, true);
        Game->Pause(5.0f);
    } else {
        if (IsActiveThreadTerminating()) return;
        long conversation = Game->AddNewConversation(guard, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_GUARD_BACKSOON_03", false, guard, Game->GetHero());
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        guard_res.MoveToPosition(Game->GetThingWithScriptName("GuardPatrolExit").GetPos(), 2.0f, (EScriptEntityMoveType)0, false, true);
        Game->Pause(5.0f);
        conversation = Game->AddNewConversation(accomplice, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_055_PRISON_PRISONER_HINT_02", false, accomplice, Game->GetHero());
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    PrisonCutsceneFinished = true;
}