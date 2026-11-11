// NScript::CQ_HerosOldHouseScript::CFishermansWife::Main() -- the fisherman's wife: under scripted control (not killable, not pushable) she cries to a nearby hero every 15 seconds; talked to, she thanks the hero once the ghost was helped, asks the gold question (500 gold, the CS_GHOSTFISH_WIFE_SUCCESS cutscene, gold bag, objective 03, ghost marker) once the booty is dug up, or plays the intro / repeat greeting; hit by the hero she runs to FisherWifeLeaveMarker and fades out; once leaving happy she walks there with shopping asides and fades out on arrival
// Grade: DIFFER(6343v6343) -- size and code equal, 23 masked lines, all two frame slots: retail keeps the GetHealth compare bool at [esp+13h] and WifeTimer at [esp+14h], ours WifeTimer at [esp+10h] and the bool at [esp+17h]. Chunk H40: the predicate blocker is solved (inline_depth(0) round the CIsThingAlive ctor and dtor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE: 93 -> 23 lines)
// Next idea: frame-packer order of the 1-byte temp against the 4-byte timer (IMPLICIT_COPY and the CTimer ctor spelling do not move it): replay the packer (hand886 tools8/phook.py) for the two nodes' weights; the timer's weight counts every exit's dtor reference, so try the loop / exit spellings that change those counts
// Leaf: ?Main@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8BCD0, 6343 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ;
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012D50DC that the landed CFishermansWife::Alloc 0x00D8A650
//   stores (slot 0 ??_G 0x00D8A7D0, 2 Init 0x00D8A700, 3 GetParentScript 0x00D8A6F0, 4 OnPersist 0x00D8ABA0, 5 OnPredicateFail 0x00D8A770).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_CFISHERMANSWIFE
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/EHeroAbility.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_HerosOldHouseScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716

void NScript::CQ_HerosOldHouseScript::CFishermansWife::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me, false, true);
    Game->SetIsPushableByHero(Me, false);
    CScriptThing Booty = Game->GetThingWithScriptName("HiddenBooty");
    CTimer WifeTimer;
    WifeTimer = 5;
    while (!LeavingHappy) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (WifeTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f)) {
            if (IsActiveThreadTerminating()) return;
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_032_FISHERWIFE_CRY", false, Me, Game->GetHero());
            WifeTimer = 15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.Helped) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_THANKS", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else if (ParentClass.Helping && !Game->IsDiggingSpotEnabled(Booty) && Game->GetHeroGold() >= 500) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->GiveHeroYesNoQuestion("TEXT_QST_032_FISHERWIFE_GOLD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (Answer < 0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    Answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (Answer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.Helped = true;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"] = seh_hero;
                    mymap["WIFE"] = seh_me;
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_GHOSTFISH_WIFE_SUCCESS", mymap, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                    Game->GiveHeroMorality(pGlobalScriptDef->HOH_MoralityGain);
                    Game->AddItemToContainer(Me, "OBJECT_GOLDBAG_MEDIUM_WITH_COINS_500");
                    Game->GiveHeroGold(-500);
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_03", "", "OakBay");
                    Game->ClearThingHasInformation(Me);
                    Game->MiniMapRemoveMarker(Me);
                    Game->MiniMapAddMarker(Game->GetThingWithScriptName("GhostFisherman"), "HUD_ORB_QUEST_VIGNETTE");
                    Game->FadeScreenIn();
                    LeavingHappy = true;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (TalkedTo) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_REPEAT_GREET", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        CScriptGameResourceObjectScriptedThingBase seh_hero;
                        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                        mymap["HERO"] = seh_hero;
                        mymap["WIFE"] = seh_me;
                        Game->FixMovieSequenceCamera(true);
                        RunCutsceneMacro("CS_GHOSTFISH_WIFE_INTRO", mymap, 0, 0, false, true);
                        Game->FixMovieSequenceCamera(false);
                        TalkedTo = true;
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (TalkedTo) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_REPEAT_GREET", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["HERO"] = seh_hero;
                    mymap["WIFE"] = seh_me;
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_GHOSTFISH_WIFE_INTRO", mymap, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                    TalkedTo = true;
                }
            }
            Game->EntitySetFacingAngle(Me, initial_angle, true);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            Game->EntitySetAsKillable(Me, true, true);
            CScriptThing LeaveMarker = Game->GetThingWithScriptName("FisherWifeLeaveMarker");
            ParentClass.WifeAttacked = true;
            seh_me.MoveToPosition(LeaveMarker.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass.WifeAttackedAndLeft = true;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    seh_me.MoveToPosition(Game->GetThingWithScriptName("FisherWifeLeaveMarker").GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    if (IsActiveThreadTerminating()) return;
    do {
        if (WifeTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f)) {
            if (IsActiveThreadTerminating()) return;
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_032_FISHERWIFE_SHOPPING_ASIDE", false, Me, Game->GetHero());
            WifeTimer = 15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_SHOPPING", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
            CScriptThing LeaveMarker = Game->GetThingWithScriptName("FisherWifeLeaveMarker");
            seh_me.MoveToPosition(LeaveMarker.GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_032_FISHERWIFE_ATTACKED_AFTER", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetAsKillable(Me, true, true);
            }
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass.WifeAttacked = true;
            CScriptThing LeaveMarker = Game->GetThingWithScriptName("FisherWifeLeaveMarker");
            seh_me.MoveToPosition(LeaveMarker.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            ParentClass.WifeAttackedAndLeft = true;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/fw_a11.cpp)
