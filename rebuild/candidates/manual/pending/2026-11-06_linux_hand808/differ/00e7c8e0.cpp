// NScript::CV_FishermanScript::CFisherman::Main() -- the fisherman: until the hero has saved him from the wasps play the intro cut-scene with three cut-scene wasps, run to his hiding spot, call for help at the window while wasps live, then thank the hero (quest card, objective, fishing-spot game info); afterwards open the hut door, run the fishing tutorial (or the knows-fishing line), play the caught-fish cut-scene and complete the quest, then idle-chat looking at the river
// Grade: DIFFER(5358v5358) -- SDIFF 38: helpTimer.TimerIndex cached in esi across the window loop in retail (reloaded per use here); the thanks-scope T exit block kept at the first T (inside !QuestCardGiven) in retail, after the IsXbox branch here; FishermanTasks=2 through eax vs ecx; RelationCounters[2] / RelationSuffixes reloc addends
// Next idea: try the window loop with the timer read into a local before the loop, or the thanks scope with the game-info pair in its own block, to move the shared T exit; frame and size already match
// Leaf: ?Main@CFisherman@CV_FishermanScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7C8E0, 5358 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CFisherman@CV_FishermanScript@NScript@@EAEXXZ; the
//   address has one .rdata reference, slot 1 of the entity vtable 0x012EA0B4 that the landed CFisherman::Alloc 0x00E7B7B0 stores
//   (reference 0x00E7B7E5).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_FishermanScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "engine/CScriptDef.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
namespace NScript {
class CIsVariableFalse : public CPredicateBase {           // vtable 0x012EA43C
public:
    const bool* Variable;                                  // +0x04
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual bool operator()() const;                       // slot 0, 0x00E7DDD0
    virtual CPredicateBase* Clone() const;                 // slot 1, 0x00E7DDE0
    virtual ~CIsVariableFalse() { }                        // slot 2, 0x00E7DE10
};
}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
CCharString FABLE_FASTCALL operator+(const char*, const CCharString&);                                // 0x0099F690

void NScript::CV_FishermanScript::CFisherman::Main()
{
    SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass->FishermanMustRunAway));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (!ParentClass->HeroHasSavedFishermanFromWasps) {
        if (IsActiveThreadTerminating()) return;
        ParentClass->FishermanTasks = 1;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        {
            CScriptThing wasp1 = Game->CreateCreature("CREATURE_HORNET_01", Game->GetThingWithScriptName("MK_INTRO_WASP1").GetPos(), "CutsceneWasp", false);
            CScriptThing wasp2 = Game->CreateCreature("CREATURE_HORNET_01", Game->GetThingWithScriptName("MK_INTRO_WASP2").GetPos(), "CutsceneWasp", false);
            CScriptThing wasp3 = Game->CreateCreature("CREATURE_HORNET_01", Game->GetThingWithScriptName("MK_INTRO_WASP3").GetPos(), "CutsceneWasp", false);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_wasp1;
            CScriptGameResourceObjectScriptedThingBase seh_wasp2;
            CScriptGameResourceObjectScriptedThingBase seh_wasp3;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(wasp1, seh_wasp1, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(wasp2, seh_wasp2, SCRIPT_AI_PRIORITY_HIGHEST);
            Game->StartScriptingEntity(wasp3, seh_wasp3, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            mymap["WASP1"] = seh_wasp1;
            mymap["WASP2"] = seh_wasp2;
            mymap["WASP3"] = seh_wasp3;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_INTRO", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            ParentClass->IntroPlayed = true;
        }
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        CScriptThing hidingSpot = Game->GetThingWithScriptName("M_FishermanHiding");
        {
            C3DVector hidingPos = hidingSpot.GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), hidingPos, 3.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                seh_me.MoveToPosition(hidingPos, 1.0f, ENTITY_MOVE_RUN, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        CScriptThing windowPos = Game->GetThingWithScriptName("M_FishermanWindowPos");
        CTimer helpTimer;
        helpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
        while (ParentClass->WaspsAlive > 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (helpTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(windowPos, Game->GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating()) return;
                helpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_023_HELP", false, Me, Game->GetHero());
            }
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetAsScared(Me, false);
        ParentClass->HeroHasSavedFishermanFromWasps = true;
        Game->GiveHeroMorality(pGlobalScriptDef->FishermanSaveHimMorality);
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_THANKS", mymap, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            if (!ParentClass->QuestCardGiven) {
                if (IsActiveThreadTerminating()) return;
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FISHERMAN", Game->GetActiveQuestName(), false);
                ParentClass->QuestCardGiven = true;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FISHERMAN_OBJECTIVE_01", "", "");
            }
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating()) return;
                Game->DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO_PC");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
            }
            if (IsActiveThreadTerminating()) return;
        }
        ParentClass->FishermanTasks = 2;
    }
    Game->OpenDoor(Game->GetThingWithScriptName("FishermanHutDoor"));
    Game->EntitySetAllStategroupsEnabled(Me, false);
    Game->EntitySetOpinionReactionsEnabled(Me, true);
    Game->EntitySetDeedReactionsEnabled(Me, true);
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
    if (ParentClass->FishermanTasks == 2) {
        if (IsActiveThreadTerminating()) return;
        Game->SetThingHasInformation(Me, false, true, false);
        CScriptThing tutorial = Game->GetNearestWithScriptName(Me, "FishermanTutorialMain");
        if (tutorial.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
            while (Game->IsFishingSpotEnabled(tutorial)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                IdleChatter();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_023_TUTORIAL_10", false, Me, Game->GetHero());
                }
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass->FishermanTasks = 3;
        } else {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_023_KNOWS_FISHING" + CQ_SunnyvaleMasterData::RelationSuffixes[CQ_SunnyvaleMasterData::RelationCounters[2]], false, Me, Game->GetHero());
            seh_me.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass->FishermanTasks = 4;
        }
    }
    Game->ClearThingHasInformation(Me);
    if (ParentClass->FishermanTasks == 3) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->FixMovieSequenceCamera(true);
            if (CQ_SunnyvaleMasterData::RelationSuffixes[CQ_SunnyvaleMasterData::RelationCounters[2]] == "HATE") {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH_NASTY", mymap, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH", mymap, 0, 0, false, true);
            }
            Game->FixMovieSequenceCamera(false);
        }
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
        ParentClass->FishermanTasks = 4;
    }
    if (ParentClass->FishermanTasks == 4) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->EntityForceToLookAtThing(Me, Game->GetThingWithScriptName("M_FishermanLookAtRiver"));
        ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
        while (!IsActiveThreadTerminating()) {
            IdleChatter();
            Game->NewScriptFrame();
        }
    }
}
