// NScript::CV_ExposeMayorScript::CEM_Rhodri::Main() -- Rhodri in Expose the Mayor: live while conscious; take scripted control; until the quest stage reaches 5: face the hero every 4 s when he is within 10 m; when talked to at stage 1 play CS_MAYOREXPOSE_RHODRI (ExplainedPlight, stage 2, HauntedHouse objective), otherwise under a paused speech movie say TEXT_QST_B18_RHODRI_LETTER at stage 4 or TEXT_QST_B18_RHODRI_CHATTER(_MARRIED); when hit by the hero (except a heal-life spell) say TEXT_QST_B18_RHODRI_ATTACKED(_MARRIED); until he has explained his plight call the hero over (TEXT_QST_B18_RHODRI_ATTENTION) every 7 s within 15 m
// Inline-budget: #pragma inline_depth(0) round the row-local `inline ~CIsThingConscious() { }` definition -- retail expands the predicate dtor (filter vtable 0x012C2FE8 hoisted in ebx, stored by ctor and dtor) and CALLS the nested ~CScriptThing 0x004AA840; with the nested dtor expanded into it VC7.1 calls the whole outer dtor instead; no flag reaches it (/d2inlS5/S15/S20, /d2inlT30..T100 all differ: T changes the inner inlining too, GOTCHAS 2026-11-03)
// Leaf: ?Main@CEM_Rhodri@CV_ExposeMayorScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E799F0, 2580 bytes, thiscall, plain `ret` (four epilogue copies), frame 0x90.
// Identity (proven): Ego_r.pdb ?Main@CEM_Rhodri@CV_ExposeMayorScript@NScript@@EAEXXZ; slot 1 of the CEM_Rhodri entity vtable stored by
//   the landed Alloc 0x00E765D0.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_RHODRI
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ExposeMayorScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
// Inline control (owner decision 2026-11-08, device 3): compile the predicate's destructor with no inlining of its own, so its
// body stays `~CScriptThing()` as a call; Main then expands ~CIsThingConscious (filter vtable store + call 0x004AA840) exactly as retail.
#pragma inline_depth(0)
inline NScript::CIsThingConscious::~CIsThingConscious() { }
#pragma inline_depth()
inline CScriptThing::~CScriptThing() { }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
CCharString FABLE_FASTCALL operator+(const CCharString&, const char*);                                            // 0x0099F600

void NScript::CV_ExposeMayorScript::CEM_Rhodri::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    {
        CV_ExposeMayorScript& parent = ParentClass;
        if (parent.seh_Rhodri.IsValid())
            parent.seh_Rhodri.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Rhodri, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer TalkTimer;
    CTimer FaceHeroTimer;
    while (ParentClass.StageNumber != 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (FaceHeroTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            FaceHeroTimer = 4;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass.StageNumber == 1) {
                if (IsActiveThreadTerminating())
                    return;
                ExplainedPlight = true;
                ParentClass.PlayCutscene("CS_MAYOREXPOSE_RHODRI", true, "NULL");
                ParentClass.StageNumber = (CV_ExposeMayorScript::STAGE_IN_PATH)2;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_02", "HauntedHouse", "");
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass.StageNumber == 4) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(ParentClass.seh_Rhodri.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game->GetHero(), "TEXT_QST_B18_RHODRI_LETTER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(ParentClass.seh_Rhodri.GetScriptThing()) > 0.0f) {
                        ParentClass.seh_Rhodri.Speak(Game->GetHero(), CCharString("TEXT_QST_B18_RHODRI_CHATTER") + (PMasterData->HeroMarriedLadyGrey ? "_MARRIED" : ""), GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Rhodri.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Rhodri.Speak(Game->GetHero(), CCharString("TEXT_QST_B18_RHODRI_ATTACKED") + (PMasterData->HeroMarriedLadyGrey ? "_MARRIED" : ""), GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (ParentClass.seh_Rhodri.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
        }
        if (!ExplainedPlight && TalkTimer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating())
                return;
            TalkTimer = 7;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B18_RHODRI_ATTENTION", false, Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
}