// NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman::Main() -- the husband's lover: live while conscious; take scripted control and find NOVI_AffairWife / NOVI_AffairMan; until the wife comes within 5 units: walk back home when moved off it, say TEXT_QST_048_AFFAIRWOMAN_ON_HIT when the hero hits her (a bad deed), TEXT_QST_048_AFFAIRWOMAN_BUSY when he talks to her (the man faces the hero meanwhile), play RECEIVE_KISS / RECEIVE_HUG when the man asks; then run to AffairWomanRunOffPoint, wait until she is off screen and remove her
// Grade: DIFFER(2858v2714) -- per-exit ~CScriptThing split: retail expands only the in-loop GetScriptThing temporary of the walk-home loop and calls ~CScriptThing 0x004AA840 at every return pad; with the row-local inline dtor ours also expands AffairWife / AffairMan at the walk-home wait-loop return (one pad, 9 calls) and keeps the main-loop bottom jump shape; 135 lines
// Next idea: cool that one inner-wait-loop exit (it is an inner-loop exit, hot to c2) without changing the walk-home loop shape, or find why retail homes the predicate at [esp+0x60] (ours +0x6c); TRetVoid, for/break and block scopes tried (a3-a8)
// Leaf: ?Main@CNOVI_AffairWoman@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_AFFAIRWOMAN
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }

void NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman::Main()
{
    {
        CIsThingConscious conscious(Me);
        SetPredicate(conscious);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CScriptThing AffairWife = Game->GetThingWithScriptName("NOVI_AffairWife");
    CScriptThing AffairMan = Game->GetThingWithScriptName("NOVI_AffairMan");
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (IsDistanceFromThingToPositionOver(Me, Me.GetHomePos(), 0.1f)) {
            if (IsActiveThreadTerminating()) return;
            float local_range = 0.0f;
            C3DVector local_position = Me.GetHomePos();
            EScriptEntityMoveType local_movetype = ENTITY_MOVE_WALK;
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.TalkingToWoman = true;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_048_AFFAIRWOMAN_ON_HIT", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
            ParentClass.TalkingToWoman = false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.TalkingToWoman = true;
            seh_me.ClearAllActions();
            seh_me.ClearCommands();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->EntitySetFacingAngleTowardsThing(AffairMan, Game->GetHero(), false);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_048_AFFAIRWOMAN_BUSY", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            Game->EntitySetFacingAngleTowardsThing(AffairMan, Me, false);
            ParentClass.TalkingToWoman = false;
        }
        if (ParentClass.ReceiveKiss && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("RECEIVE_KISS", false, true, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            ParentClass.ReceiveKiss = false;
        }
        if (ParentClass.ReceiveHug && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("RECEIVE_HUG", false, true, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            ParentClass.ReceiveHug = false;
        }
        if (IsDistanceBetweenThingsUnder(Me, AffairWife, 5.0f))
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    {
    CScriptThing RunOffPos = Game->GetThingWithScriptName("AffairWomanRunOffPoint");
    Game->EntitySetAsUseMovementInActions(Me, true);
    Game->SetIsPushableByHero(Me, true);
    {
        float local_range = 0.0f;
        C3DVector local_position = RunOffPos.GetPos();
        EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
        while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    if (seh_me.IsValid()) seh_me.Release();
    while (Game->IsCameraPosOnScreen(Me.GetPos())) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveThing(Me, false, true);
    }
}
