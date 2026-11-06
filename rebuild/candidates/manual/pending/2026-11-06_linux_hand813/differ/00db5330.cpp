// NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main() -- the Oakvale warehouse barrel man: wanders near his barrels, runs through his four MyPhase steps when the quest moves him on, answers the hero (favour, not larking, let-down, no time), complains when hit, and lets the hero overhear him
// Grade: DIFFER(6228v6148) -- 769 lines (317 masked): the CIsThingConscious blocker (GOTCHAS chunk 810): retail calls the CScriptThing copy 0x004ABE90 inside the expanded predicate ctor and ~CScriptThing 0x004AA840 inside its expanded dtor while the four by-value Me copies (SetWanderCentrePoint / Min / Max, SetScriptingStateGroup) expand; with IMPLICIT_COPY the predicate copy expands too and ~CIsThingConscious is called out of line, and the frame differs
// Next idea: the shared lane fix for the chunk 810 blocker; then recheck the exit ~CScriptThing expansions (the implicit inline movie ctor and the for (;; NSF) { T; } loop already took the masked count 789 -> 317)
// Leaf: ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ (private virtual); entity vtable 0x012D9014
//   slot 1 = 0x00DB5330 (its only .rdata reference 0x012D9018), stored by the Alloc 0x00DB51B0; slot 0 the landed scalar deleting dtor
//   0x00DB5310, slot 2 the landed Init 0x00DB5260.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DB5330, 6148 bytes incl. the MyPhase jump table 0x00DB6B24 (cases 1..4); no EH frame.
// Layout: engine/CQ_NewOakValeIntroScript.h, opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA (this chunk's overlay).
// Globals: 0x013AC854 (long 100, the overhear rand modulus) and 0x013AC858 (float 10.0f, the notice distance): no PDB name found, named
//   descriptively.
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
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern "C" int __cdecl rand();                             // 0x00BFEB16 (MSVCR71)
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
extern long BarrelManOverhearChance;                       // 0x013AC854 (initialised 100; descriptive, no PDB name)
extern float BarrelManNoticeDistance;                      // 0x013AC858 (initialised 10.0f; descriptive, no PDB name)
inline CScriptThing::~CScriptThing() {}
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C

void NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game->SetWanderCentrePoint(Me, Me.GetHomePos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, 1.0f);
    Game->SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    CScriptThing man_start = Game->GetThingWithScriptName("M_WHouse_ManStart");
    CScriptThing guard_point = Game->GetThingWithScriptName("M_WHouse_GuardPoint");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (MyPhase != 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            switch (MyPhase) {
            case 1: {
                ParentClass.WatchTimer = 45;
                Game->EntitySetTargetable(Me, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing walk_off = Game->GetThingWithScriptName("M_BarrelManWalkOff");
                C3DVector pos = walk_off.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.0f)) {
                    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(pos, 0.0f, ENTITY_MOVE_RUN, false, false);
                    while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = (EMyPhase)2;
                break;
            }
            case 2: {
                while (ParentClass.WatchTimer.GetTime() != 15) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing walk_off = Game->GetThingWithScriptName("M_BarrelManWalkOff");
                CScriptThing walk_off_alt = Game->GetThingWithScriptName("M_BarrelManWalkOffAlt");
                if (Game->IsCameraPosOnScreen(walk_off.GetPos())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntityTeleportToThing(Me, walk_off_alt, false);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntityTeleportToThing(Me, walk_off, false);
                }
                MyPhase = (EMyPhase)3;
                break;
            }
            case 3: {
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_PAUSED);
                Game->EntitySetTargetable(Me, false);
                {
                    C3DVector pos = man_start.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.0f)) {
                        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(pos, 0.0f, ENTITY_MOVE_RUN, false, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                MyPhase = (EMyPhase)4;
                break;
            }
            case 4:
                ParentClass.BarrelManSpokenToHeroOnReturn = true;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                Game->EntitySetCutsceneBehaviour(Me, CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                Game->EntitySetTargetable(Me, true);
                if (!Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) && !IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddLineToConversation(conversation, "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE", false, Me, CScriptThing());
                    HeroLetMeDown = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.AddBadDeed((EBadDeeds)1);
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_THANKS", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.AddGoodDeed();
                }
                Game->SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
                MyPhase = (EMyPhase)5;
                break;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            ParentClass.AddBadDeed((EBadDeeds)2);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_CAREFUL", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        } else if ((MyPhase == 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), BarrelManNoticeDistance)) || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (MyPhase == 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CTimer move_timer;
                while (IsDistanceFromThingToPositionOver(Me, Game->GetHero().GetPos(), 4.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(Game->GetHero().GetPos(), 2.0f, ENTITY_MOVE_RUN, false, true);
                    move_timer = 2;
                    while (seh_me.IsPerformingScriptTask() && move_timer.GetTime() > 0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_FAVOUR", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->FadeScreenOut(1.0f, 1.0f, CRGBColour(0,0,0,255));
                Game->Pause(2.0f);
                Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("M_WHouse_GuardPoint"), false);
                Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_BarrelManHiddenPos"), false);
                MyPhase = (EMyPhase)2;
                Game->ClearThingHasInformation(Me);
                Game->FadeScreenIn();
                ParentClass.WatchTimer = 45;
                ParentClass.BarrelManLeftHeroInCharge = true;
            } else if (MyPhase < 4) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_NOT_LARKING", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            } else if (MyPhase == 5) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (HeroLetMeDown) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.BarrelBrokenPersistent) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_048_BARRELMAN_NO_TIME", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MyPhase == 0 && (!OverheardYet || rand() % BarrelManOverhearChance == 0) && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            OverheardYet = true;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddLineToConversation(conversation, "TEXT_QST_048_BARRELMAN_OVERHEAR", false, Me, CScriptThing());
        }
    }
}
