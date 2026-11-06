// NScript::CQ_UndeadRisingScript::Main() -- the undead rising quest main: bind the five entity scripts, wait for GraveYard_2, spawn WatchForTermination and LampChecker, light the big-rock sparkles and the portal; until the hero reaches the centre keep both effects alive (re-creating them after a graveyard reload); then set objective 6, play the intro cutscene, collect the activation / boss points, wait for every zombie token, kill the attackers, play the bosses cutscene with the three zombie bosses, wait for every boss kill, keep the big rock persistent and play the exit cutscene
// Leaf: ?Main@CQ_UndeadRisingScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0B160, length 4584 bytes.
// Name: slot 2 of the level-script vtable 0x012DFD3C (1 RegisterMain 0x00E0B0A0, 3 Init 0x00E0B020) that the landed ctor
//   0x00E0AFE0 stores; Ego_r.pdb and FableWin.pdb both hold ?Main@CQ_UndeadRisingScript@NScript@@UAEXXZ.
// Callees: AddEntityScriptBinding 0x00CB8230 with CEntityScriptBinding<CQ_UndeadRisingScript> (vtable 0x012DFED0) and the five
//   entity Allocs (CUndeadRisingFirefly 0x00E0C970, ZombieAttacker 0x00E0CF20, ZombieAttackerBoss 0x00E0D590, Boss 0x00E0D130,
//   Centre 0x00E0D760), PostAddScriptedEntities 0x00CB8930, AddSpawnedFunction 0x00CB7E50 with CSpawnedFunc<...> (vtable
//   0x012DFD50, CSpawnedFuncBase ctor 0x00CDD450) for WatchForTermination 0x00E0C350 and LampChecker 0x00E0C4C0,
//   IsActiveThreadTerminating 0x00CB7940, KillAllThingsInVector 0x00CBED82, RunCutsceneMacro 0x00CBFB7D, cast map ctor
//   0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, scripted-thing ctor 0x007E72A0 / operator= 0x008ABD10 / ~ 0x007E74D0,
//   MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, ~vector<CScriptThing> 0x008AC970, ~CScriptThing 0x004AA840, ~CBaseClass
//   0x0099A2E0, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, operator new 0x00BFEA1A / delete 0x00BFE9BC, free
//   0x00BFEA14; interface slots through Game.
// Data: engine/CQ_UndeadRisingScript.h (new, behind FABLE_RETAIL_NSCRIPT): token/boss counters +0x48/+0x4C, HeroReachedCentre
//   +0x50, BossCutscenePlayed +0x51, MissionSucceeded +0x52, quest_status +0x5C, activation_points +0x60, boss_points +0x6C.
// Shape: a generated level-script Main with three `{ }` cutscene blocks (each block's map, movie and pause object die at its
//   close, as retail's dtor order shows). The row-local inline CScriptThing::IsAlive body (null PImp test, then the slot-75
//   virtual) is what retail expands at the two effect re-creation tests; the COMDAT 0x004AB130 call is one more frame-shifted
//   diff (a1, 51 lines). The spawn name is an explicit CCharString("") so it is built after the new expression.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_UndeadRisingScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade);   // 0x00CBED82
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
namespace NScript {
#define FABLE_UNDEADRISING_ENTITY(Name) \
    class CQ_UndeadRisingScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_UndeadRisingScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_UNDEADRISING_ENTITY(CUndeadRisingFirefly)              // Alloc 0x00E0C970
FABLE_UNDEADRISING_ENTITY(CUndeadRisingZombieAttacker)       // Alloc 0x00E0CF20
FABLE_UNDEADRISING_ENTITY(CUndeadRisingZombieAttackerBoss)   // Alloc 0x00E0D590
FABLE_UNDEADRISING_ENTITY(CUndeadRisingBoss)                 // Alloc 0x00E0D130
FABLE_UNDEADRISING_ENTITY(CUndeadRisingCentre)               // Alloc 0x00E0D760
#undef FABLE_UNDEADRISING_ENTITY
}

void NScript::CQ_UndeadRisingScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_UndeadRisingScript>("UndeadRisingFirefly", this, &CUndeadRisingFirefly::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_UndeadRisingScript>("UndeadRisingZombieAttacker", this, &CUndeadRisingZombieAttacker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_UndeadRisingScript>("UndeadRisingZombieAttackerBoss", this, &CUndeadRisingZombieAttackerBoss::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_UndeadRisingScript>("UndeadRisingBoss", this, &CUndeadRisingBoss::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_UndeadRisingScript>("UndeadRisingCentre", this, &CUndeadRisingCentre::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("GraveYard_2")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_UndeadRisingScript>("WatchForTermination", 0, this, &CQ_UndeadRisingScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_UndeadRisingScript>("LampChecker", 0, this, &CQ_UndeadRisingScript::LampChecker), CCharString(""));
    CScriptThing rock_sparkles = Game->CreateEffect("GRAVEYARD_SPARKLES_DOOR", Game->GetThingWithScriptName("UndeadRisingBigRock").GetPos(), "", Game->GetThingWithScriptName("UndeadRisingBigRock").GetAngleXY(), false, false);
    CScriptThing portal = Game->CreateEffect("GRAVEYARD_PORTAL_IDLE", Game->GetThingWithScriptName("UndeadRisingCentre").GetPos(), "", 0.0f, false, false);
    std::vector<CScriptThing> zombies;
    while (!HeroReachedCentre) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsLevelLoaded("GraveYard_2")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsLevelLoaded("GraveYard_2")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (!rock_sparkles.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            rock_sparkles = Game->CreateEffect("GRAVEYARD_SPARKLES_DOOR", Game->GetThingWithScriptName("UndeadRisingBigRock").GetPos(), "", Game->GetThingWithScriptName("UndeadRisingBigRock").GetAngleXY(), false, false);
        }
        if (!portal.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            portal = Game->CreateEffect("GRAVEYARD_PORTAL_IDLE", Game->GetThingWithScriptName("UndeadRisingCentre").GetPos(), "", 0.0f, false, false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_OpeningGraveyardSecretPassage", "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_06", "", "Graveyard");
    quest_status = QUEST_STATUS_COLLECTING_TOKENS;
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->RemoveThing(portal, false, true);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        CScriptGameResourceObjectMovieBase intro_movie;
        Game->StartMovieSequence("", intro_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_UNDEADRISING_INTRO", mymap, 0, 0, false, true);
    }
    Game->GetAllThingsWithScriptName("UndeadRisingActivationPoint", activation_points);
    Game->GetAllThingsWithScriptName("UndeadRisingBoss", boss_points);
    while (number_of_zombie_tokens_in_right_area < activation_points.size()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    quest_status = QUEST_STATUS_FIGHTING_BOSSES;
    Game->GetAllThingsWithScriptName("UndeadRisingZombieAttacker", zombies);
    KillAllThingsInVector(*Game, zombies, false);
    Game->Pause(0.1f);
    {
        std::vector<CScriptThing> bosses;
        Game->GetAllThingsWithScriptName("UndeadRisingZombieAttackerBoss", bosses);
        CScriptGameResourceObjectScriptedThingBase seh_boss0;
        Game->StartScriptingEntity(bosses[0], seh_boss0, SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_boss1;
        Game->StartScriptingEntity(bosses[1], seh_boss1, SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_boss2;
        Game->StartScriptingEntity(bosses[2], seh_boss2, SCRIPT_AI_PRIORITY_HIGHEST);
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["BOSS0"] = seh_boss0;
            mymap["BOSS1"] = seh_boss1;
            mymap["BOSS2"] = seh_boss2;
            CScriptGameResourceObjectMovieBase bosses_movie;
            Game->StartMovieSequence("", bosses_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_UNDEADRISING_BOSSES", mymap, 0, 0, false, true);
        }
        Game->Pause(0.5f);
    }
    BossCutscenePlayed = true;
    while (number_of_boss_kills_in_right_area < boss_points.size()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Game->GetThingWithScriptName("UndeadRisingBigRock"), true);
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        CScriptGameResourceObjectMovieBase exit_movie;
        Game->StartMovieSequence("", exit_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_UNDEADRISING_EXIT", mymap, 0, 0, false, true);
        Game->RemoveThing(rock_sparkles, false, true);
        quest_status = QUEST_STATUS_COMPLETED;
        MissionSucceeded = true;
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}