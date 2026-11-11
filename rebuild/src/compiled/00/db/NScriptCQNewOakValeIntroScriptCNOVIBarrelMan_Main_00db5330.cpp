// NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main() -- the Oakvale warehouse barrel man: wanders near his barrels, runs through his four MyPhase steps when the quest moves him on, answers the hero (favour, not larking, let-down, no time), complains when hit, and lets the hero overhear him
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: genuine predicate ctor/dtor nested depth0; ordinary actual timer constructor/destructor remain calls under guarded auto_inline(off).
// Leaf: ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: 0x00DB5330, 6148 bytes, thiscall.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ (private virtual); entity vtable 0x012D9014
//   slot 1 = 0x00DB5330 (its only .rdata reference 0x012D9018), stored by the Alloc 0x00DB51B0; slot 0 the landed scalar deleting dtor
//   0x00DB5310, slot 2 the landed Init 0x00DB5260.
// Layout: engine/CQ_NewOakValeIntroScript.h, opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA (this chunk's overlay).
// Original Main-local static data: need_wee_frequency (long100) and barrelman_near_distance (float10), proved by original public and typed PDB records and donor/native loads.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CISTHINGCONSCIOUS_ORIGINAL_ACCESS
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/CRGBColour.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/fable_script_distance.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EScriptEntityMoveType.h"
#include "engine/EHeroAbility.h"
extern "C" int __cdecl rand();                             // 0x00BFEB16 (MSVCR71)
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
inline CScriptThing::~CScriptThing() {}
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C

void NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main()
{
    TRetVoid _Fail_Val;
    static long need_wee_frequency = 100;
    static float barrelman_near_distance = 10.0f;
    SetPredicate(CIsThingConscious(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game.SetWanderCentrePoint(Me, Me.GetHomePos());
    Game.SetWanderMinDistance(Me, 0.0f);
    Game.SetWanderMaxDistance(Me, 1.0f);
    Game.SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    CScriptThing guard_man_start_position = Game.GetThingWithScriptName("M_WHouse_ManStart");
    CScriptThing guard_hero_start_position = Game.GetThingWithScriptName("M_WHouse_GuardPoint");
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (MyPhase != PHASE_WAITING) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            switch (MyPhase) {
            case PHASE_WALKING_AWAY: {
                ParentClass.WatchTimer = 45;
                Game.EntitySetTargetable(Me, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing walk_away_pos = Game.GetThingWithScriptName("M_BarrelManWalkOff");
                EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                float local_range = 2.0f;
                C3DVector local_position = walk_away_pos.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                    Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = PHASE_AWAY;
                break;
            }
            case PHASE_AWAY: {
                while (ParentClass.WatchTimer.GetTime() != 15) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing return_pos_1 = Game.GetThingWithScriptName("M_BarrelManWalkOff");
                CScriptThing return_pos_2 = Game.GetThingWithScriptName("M_BarrelManWalkOffAlt");
                if (!Game.IsCameraPosOnScreen(return_pos_1.GetPos())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntityTeleportToThing(Me, return_pos_1, false);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntityTeleportToThing(Me, return_pos_2, false);
                }
                MyPhase = PHASE_RETURNING;
                break;
            }
            case PHASE_RETURNING: {
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
                Game.EntitySetTargetable(Me, false);
                {
                    EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                    float local_range = 2.0f;
                    C3DVector local_position = guard_man_start_position.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range)) {
                        Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(local_position, 0.0f, local_movetype, false, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = PHASE_RETURNED;
                break;
            }
            case PHASE_RETURNED:
                ParentClass.BarrelManSpokenToHeroOnReturn = true;
                Game.EntitySetFacingAngleTowardsThing(Me, Game.GetHero(), false);
                Game.EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                Game.EntitySetTargetable(Me, true);
                if (!Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(), Me) && !IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 10.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
                    Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE", false, Me, CScriptThing());
                    HeroLetMeDown = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    Game.DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED");
                    while (!Game.MsgIsGameInfoClickedPast()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.AddBadDeed(BD_DERELICTION_OF_DUTY);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_THANKS", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.AddGoodDeed();
                }
                Game.SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
                MyPhase = PHASE_OVER;
                break;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
            ParentClass.AddBadDeed(BD_VIOLENCE);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_CAREFUL", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else if ((MyPhase == PHASE_WAITING && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), barrelman_near_distance)) || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (MyPhase == PHASE_WAITING) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CTimer WaitTimer;
                while (IsDistanceFromThingToPositionOver(Me, Game.GetHero().GetPos(), 4.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(Game.GetHero().GetPos(), 2.0f, ENTITY_MOVE_RUN, false, true);
                    WaitTimer = 2;
                    while (seh_me.IsPerformingScriptTask() && WaitTimer.GetTime() > 0) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_FAVOUR", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.FadeScreenOut(1.0f, 1.0f, CRGBColour(0,0,0,255));
                Game.Pause(2.0f);
                Game.EntityTeleportToThing(Game.GetHero(), Game.GetThingWithScriptName("M_WHouse_GuardPoint"), false);
                Game.EntityTeleportToThing(Me, Game.GetThingWithScriptName("M_BarrelManHiddenPos"), false);
                MyPhase = PHASE_AWAY;
                Game.ClearThingHasInformation(Me);
                Game.FadeScreenIn();
                ParentClass.WatchTimer = 45;
                ParentClass.BarrelManLeftHeroInCharge = true;
            } else if (MyPhase < PHASE_RETURNED) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_NOT_LARKING", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (MyPhase == PHASE_OVER) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (HeroLetMeDown) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.BarrelBrokenPersistent) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game.GetHero(), "TEXT_QST_048_BARRELMAN_NO_TIME", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game.NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MyPhase == PHASE_WAITING && (!OverheardYet || rand() % need_wee_frequency == 0) && IsDistanceBetweenThingsUnder(Me, Game.GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            OverheardYet = true;
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_048_BARRELMAN_OVERHEAR", false, Me, CScriptThing());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}