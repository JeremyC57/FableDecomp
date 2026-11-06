// NScript::CQ_RansomVictimScript::CVictimsBrother::Main() -- the victim's evil brother: take him over, unkillable and without the combo bonus; until the victim is rescued face the hero when near, give the "remind" line when talked to, react to hits (DealWithHeroHittingMe) and, once he and his protection were hit more than once, run to BrotherRunOffMarker and fade out; then mark him on the minimap and loop the same way, except that talking to him while the victim is alive plays the evil outro (victim killed, mission succeeded, ChiefsSonDied, evil morality, brother removed)
// Grade: DIFFER(3056v3076) -- every call, literal, branch and register matches except: (1) frame slots: the CIsThingAlive temporary sits at 0x68 (retail 0x58, with the filter vtable kept in ebx), and the run-off marker / CScriptThing temporaries swap slots (marker 0x40 v 0x4c); (2) block placement: retail emits the four first-loop exit stubs (inline ~CTimer) right after the minimap marker and enters the second loop with a jmp, this build emits them at the end
// Next idea: find the declaration/scope spelling that gives retail's slot order (CScriptThing temporaries below the named marker, the filter temporary in the first loop's movie slot); the stub placement may follow from it
// Leaf: ?Main@CVictimsBrother@CQ_RansomVictimScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CScriptDef.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline CScriptThing::~CScriptThing() {}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);    // 0x00CBE3EA
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                     // 0x00CBE716

void NScript::CQ_RansomVictimScript::CVictimsBrother::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    {
        CQ_RansomVictimScript& parent = ParentClass;
        if (parent.seh_Brother.IsValid()) parent.seh_Brother.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Brother, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    CTimer face_timer;
    for (;;) {
        if (ParentClass.RescuedVictim) break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (face_timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            face_timer = 1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Brother.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            DealWithHeroHittingMe();
        }
        if (ParentClass.HitEvilBrotherAndProtection > 1) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing marker = Game->GetThingWithScriptName("BrotherRunOffMarker");
            while (IsDistanceBetweenThingsOver(marker, Me, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.seh_Brother.MoveToThing(marker, 2.0f, MOVE_TYPE_RUNNING, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.5f, true, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
    if (IsActiveThreadTerminating()) return;
    do {
        if (face_timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            face_timer = 1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetThingWithScriptName("Victim").IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.ThanksCutsceneStarting = true;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase cutscene_movie;
                Game->StartMovieSequence("", cutscene_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                ParentClass.PlayCutscene("CS_RANSOM_OUTRO_EVIL", "NULL");
                PMasterData->RansomVictimHaveVictimKilled = true;
                ParentClass.MissionSucceeded = true;
                Game->SetCategoryActivity("ChiefsSonDied", true);
                Game->GiveHeroMorality(pGlobalScriptDef->RV_HeroEvilMoralityValue);
                Game->RemoveThing(Me, false, true);
            } else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(ParentClass.seh_Brother.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            DealWithHeroHittingMe();
        }
        if (ParentClass.HitEvilBrotherAndProtection > 1) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing marker = Game->GetThingWithScriptName("BrotherRunOffMarker");
            while (IsDistanceBetweenThingsOver(marker, Me, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                if (!ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.seh_Brother.MoveToThing(marker, 2.0f, MOVE_TYPE_RUNNING, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.5f, true, true);
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
