// NScript::CQ_RansomVictimScript::CVictimsBrother::Main() -- the victim's evil brother: take him over, unkillable and without the combo bonus; until the victim is rescued face the hero when near, give the "remind" line when talked to, react to hits (DealWithHeroHittingMe) and, once he and his protection were hit more than once, run to BrotherRunOffMarker and fade out; then mark him on the minimap and loop the same way, except that talking to him while the victim is alive plays the evil outro (victim killed, mission succeeded, ChiefsSonDied, evil morality, brother removed)
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive ctor and dtor definitions + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
//   (the landed predicate lever of chunk H40, as landed CFishermansWife::Main 0x00D8BCD0) -- retail expands the predicate temporary's
//   ~CIsThingAlive one level and CALLS the nested ~CScriptThing 0x004AA840; with plain inline VC7.1 expands both or neither (GOTCHAS chunk 822).
// Shape (EVIDENCED, generator pattern of the landed same-TU sibling CVictimsBrotherHelper::Main 0x00DEB280): `TRetVoid _Fail_Val` returns,
//   first loop `while (!ParentClass.RescuedVictim)` (retail rotated: bottom test 0x00DEA93E..0x00DEA946), second loop
//   `for (; !false; Game->NewScriptFrame()) { if (T) return; ... }` -- its rotated head test is the post-minimap check 0x00DEA987, entry
//   `jmp 0x00DEAA46` over the first loop's four exit stubs, bottom `je 0x00DEAA42` with its own exit copy 0x00DEAF7B.
// Leaf: ?Main@CVictimsBrother@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEA3F0, 3076 bytes, thiscall, plain `ret`, no EH frame; ends at 0x00DEAFF3.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb name sets hold ?Main@CVictimsBrother@CQ_RansomVictimScript@NScript@@EAEXXZ; the
//   address has one .rdata reference, slot 1 (0x012DD204) of the CVictimsBrother entity vtable 0x012DD200 stored by the landed
//   CVictimsBrother::Alloc 0x00DE1E60 (Init 0x00DE1F70, GetParentScript 0x00DE1F00, OnPredicateFail 0x00DE1F10 landed).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
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
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EScriptEntityMoveType.h"
#include "engine/TRetVoid.h"
#include "engine/CScriptDef.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
inline CScriptThing::~CScriptThing() {}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float);    // 0x00CBE3EA
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                     // 0x00CBE716

void NScript::CQ_RansomVictimScript::CVictimsBrother::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    {
        CQ_RansomVictimScript& parent = ParentClass;
        if (parent.seh_Brother.IsValid()) parent.seh_Brother.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Brother, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetThingHasInformation(Me, false, false, false);
    CTimer face_timer;
    while (!ParentClass.RescuedVictim) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (face_timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            face_timer = 1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(ParentClass.seh_Brother.GetScriptThing()) > 0.0f) {
                ParentClass.seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DealWithHeroHittingMe();
        }
        if (ParentClass.HitEvilBrotherAndProtection > 1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing marker = Game->GetThingWithScriptName("BrotherRunOffMarker");
            while (IsDistanceBetweenThingsOver(marker, Me, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.seh_Brother.MoveToThing(marker, 2.0f, ENTITY_MOVE_RUN, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            FadeOutAndKillEntity(Me, 1.5f, true, true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (face_timer.GetTime() == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            face_timer = 1;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetThingWithScriptName("Victim").IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.ThanksCutsceneStarting = true;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
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
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(ParentClass.seh_Brother.GetScriptThing()) > 0.0f) {
                    ParentClass.seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_REMIND", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (ParentClass.seh_Brother.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            DealWithHeroHittingMe();
        }
        if (ParentClass.HitEvilBrotherAndProtection > 1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing marker = Game->GetThingWithScriptName("BrotherRunOffMarker");
            while (IsDistanceBetweenThingsOver(marker, Me, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!ParentClass.seh_Brother.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.seh_Brother.MoveToThing(marker, 2.0f, ENTITY_MOVE_RUN, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            FadeOutAndKillEntity(Me, 1.5f, true, true);
        }
    }
}