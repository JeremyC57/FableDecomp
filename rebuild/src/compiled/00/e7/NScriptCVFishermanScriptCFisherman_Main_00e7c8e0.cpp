// NScript::CV_FishermanScript::CFisherman::Main() -- the fisherman: until the hero has saved him from the wasps play the intro cut-scene with three cut-scene wasps, run to his hiding spot, call for help at the window while wasps live, then thank the hero (quest card, objective, fishing-spot game info); afterwards open the hut door, run the fishing FishingMarker (or the knows-fishing line), play the caught-fish cut-scene and complete the quest, then idle-chat looking at the river
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: visible ordinary timer constructor/destructor preserve native calls and permit TimerIndex caching across the window loop.
// Leaf: ?Main@CFisherman@CV_FishermanScript@NScript@@EAEXXZ
// Retail: 0x00E7C8E0, 5358 bytes, thiscall.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CFisherman@CV_FishermanScript@NScript@@EAEXXZ; the
//   address has one .rdata reference, slot 1 of the entity vtable 0x012EA0B4 that the landed CFisherman::Alloc 0x00E7B7B0 stores
//   (reference 0x00E7B7E5).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
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
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/EScriptEntityMoveType.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern long NPCAttitudes[3];                           // 0x0143E928; both original PDBs
extern CCharString NPCAttitudeStrings[3];               // 0x0143E934; both original PDBs
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
#include "engine/CIsVariableFalse.h"

inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
CCharString FABLE_FASTCALL operator+(const char*, const CCharString&);                                // 0x0099F690

void NScript::CV_FishermanScript::CFisherman::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass->FishermanMustRunAway));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (!ParentClass->HeroHasSavedFishermanFromWasps) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass->FishermanTasks = 1;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptThing CSWasp1 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP1").GetPos(), "CutsceneWasp", false);
            CScriptThing CSWasp2 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP2").GetPos(), "CutsceneWasp", false);
            CScriptThing CSWasp3 = Game.CreateCreature("CREATURE_HORNET_01", Game.GetThingWithScriptName("MK_INTRO_WASP3").GetPos(), "CutsceneWasp", false);
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            CScriptGameResourceObjectScriptedThingBase seh_wasp1;
            CScriptGameResourceObjectScriptedThingBase seh_wasp2;
            CScriptGameResourceObjectScriptedThingBase seh_wasp3;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp1, seh_wasp1, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp2, seh_wasp2, SCRIPT_AI_PRIORITY_HIGHEST);
            Game.StartScriptingEntity(CSWasp3, seh_wasp3, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            mymap["WASP1"] = seh_wasp1;
            mymap["WASP2"] = seh_wasp2;
            mymap["WASP3"] = seh_wasp3;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_INTRO", mymap, 0, 0, false, true);
            Game.FixMovieSequenceCamera(false);
            ParentClass->IntroPlayed = true;
        }
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing FishermanHides = Game.GetThingWithScriptName("M_FishermanHiding");
        {
            C3DVector local_position = FishermanHides.GetPos();
            float local_range = 3.0f;
            EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(local_position, 1.0f, local_movetype, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        CScriptThing WindowMarker = Game.GetThingWithScriptName("M_FishermanWindowPos");
        CTimer HelpTimer;
        HelpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
        while (ParentClass->WaspsAlive > 0) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (HelpTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(WindowMarker, Game.GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                HelpTimer = pGlobalScriptDef->F_FishermanHelpDelay;
                long Conversation = Game.AddNewConversation(Me, false, false);
                Game.AddPersonToConversation(Conversation, Game.GetHero());
                Game.AddLineToConversation(Conversation, "TEXT_QST_023_HELP", false, Me, Game.GetHero());
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetAsScared(Me, false);
        ParentClass->HeroHasSavedFishermanFromWasps = true;
        Game.GiveHeroMorality(pGlobalScriptDef->FishermanSaveHimMorality);
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_FISHERMAN_THANKS", mymap, 0, 0, false, true);
            Game.FixMovieSequenceCamera(false);
            if (!ParentClass->QuestCardGiven) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FISHERMAN", Game.GetActiveQuestName(), false);
                ParentClass->QuestCardGiven = true;
                Game.SetQuestCardObjective(Game.GetActiveQuestName(), "TEXT_QUEST_FISHERMAN_OBJECTIVE_01", "", "");
            }
            if (Game.IsXbox()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO");
                while (!Game.MsgIsGameInfoClickedPast()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.DisplayGameInfo("TEXT_QST_023_FISHING_SPOT_INFO_PC");
                while (!Game.MsgIsGameInfoClickedPast()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        ParentClass->FishermanTasks = 2;
    }
    Game.OpenDoor(Game.GetThingWithScriptName("FishermanHutDoor"));
    Game.EntitySetAllStategroupsEnabled(Me, false);
    Game.EntitySetOpinionReactionsEnabled(Me, true);
    Game.EntitySetDeedReactionsEnabled(Me, true);
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
    if (ParentClass->FishermanTasks == 2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.SetThingHasInformation(Me, false, true, false);
        CScriptThing FishingMarker = Game.GetNearestWithScriptName(Me, "FishermanTutorialMain");
        if (FishingMarker.IsAlive()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
            while (Game.IsFishingSpotEnabled(FishingMarker)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                IdleChatter();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_023_TUTORIAL_10", false, Me, Game.GetHero());
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->FishermanTasks = 3;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_023_KNOWS_FISHING" + NPCAttitudeStrings[NPCAttitudes[2]], false, Me, Game.GetHero());
            seh_me.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass->FishermanTasks = 4;
        }
    }
    Game.ClearThingHasInformation(Me);
    if (ParentClass->FishermanTasks == 3) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            Game.StartScriptingEntity(Game.GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
            mymap["HERO"] = seh_hero;
            mymap["FISHER"] = seh_me;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            Game.FixMovieSequenceCamera(true);
            if (NPCAttitudeStrings[NPCAttitudes[2]] == "HATE") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH_NASTY", mymap, 0, 0, false, true);
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                RunCutsceneMacro("CS_FISHERMAN_CAUGHTFISH", mymap, 0, 0, false, true);
            }
            Game.FixMovieSequenceCamera(false);
        }
        Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, true, false);
        ParentClass->FishermanTasks = 4;
    }
    if (ParentClass->FishermanTasks == 4) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_MEDIUM)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntityForceToLookAtThing(Me, Game.GetThingWithScriptName("M_FishermanLookAtRiver"));
        ChatterTimer = pGlobalScriptDef->F_FishermanIdleChatDelay;
        while (!IsActiveThreadTerminating()) {
            IdleChatter();
            Game.NewScriptFrame();
        }
    }
}