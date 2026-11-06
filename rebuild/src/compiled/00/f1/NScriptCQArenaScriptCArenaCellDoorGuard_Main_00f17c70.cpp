// NScript::CQ_ArenaScript::CArenaCellDoorGuard::Main() -- the arena cell door guard: answers the hero by arena state (early chat, the enter-arena question; on yes he and the second door guard trade the round's lines, the screen fades and the arena starts), heals off hero hits, and walks back to / faces GuardingDoorMarkerRight
// Leaf: ?Main@CArenaCellDoorGuard@CQ_ArenaScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CArenaCellDoorGuard@CQ_ArenaScript@NScript@@EAEXXZ (private virtual); retail entity vtable
//   0x012F95A8 slot 1 = 0x00F17C70 (its only .rdata reference, 0x012F95AC), stored by the Alloc 0x00F17B70; slot 0 the landed scalar
//   deleting dtor 0x00F17C50, slot 2 the landed Init 0x00F17C20.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F17C70, 7740 bytes incl. the 8-entry ArenaRound jump table at 0x00F19A8C (cases 0..7); no EH frame.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CBaseObject 0x0099A380 / ~ 0x0099A430 (seh_me ctor and the
//   expanded ~seh_me at the natural exit); ScriptedThingBase ctor 0x007E72A0 (seh_guard2), IsValid 0x00CD23B9, Release 0x00CD2770,
//   ClearCommands 0x007E7360, Speak(const char*) 0x007E7390, IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490,
//   MoveToPosition 0x007E72F0, dtor 0x007E74D0; MovieBase ctor 0x006E7B60 / dtor 0x006E7B80; CWideScreenMagicPauseEntities
//   0x00CBE09A (its body expanded: PauseAllNonScriptedEntities slot 379); CCharString 0x0099EBF0 / ~ 0x0099EAE0; ~CScriptThing
//   0x004AA840; IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall); operator delete 0x00BFE9BC (the expanded PImp release); the
//   interface slots through Game (+0x04); float 0.0f 0x0122DEDC.
// Shape: the main loop is `for (;; NSF) { if (T) return ...; ... }` (chunk 809 form): its loop-top return is retail's natural
//   exit 0x00F1994C with ~seh_me expanded in place, and the first StartScriptingEntity wait pad 0x00F17EBC joins that expansion;
//   every other return cross-jumps to the shared [~movie; ~seh_me; ret] tail 0x00F19A72. The `for (; !T; NSF)` spelling expands
//   ~seh_me only at the natural exit and duplicates the epilogues (a2, 193 lines). TRetVoid _Fail_Val supplies the returns.
//   The four Speak blocks are the GetHealth > 0 guarded SPEAK pattern; each ROUND<n> pair is seh_me then seh_guard2.
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "engine/C3DVector.h"
#include "engine/CQ_ArenaScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline CScriptThing::~CScriptThing() {}
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float);

void NScript::CQ_ArenaScript::CArenaCellDoorGuard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetOpinionReactionsEnabled(Me,false);
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    bool Turned=false;
    Game->SetThingHasInformation(Me,false,true,false);
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!EarlyTalk) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_FIRST",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    EarlyTalk=true;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_SECOND",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            } else if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL_GO && !ChamTalk) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_LATE_QUESTION",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.NeedBertForSpeech=true;
                    CScriptThing guard2=Game->GetThingWithScriptName("ArenaCellDoorGuard2");
                    CScriptGameResourceObjectScriptedThingBase seh_guard2;
                    if (seh_guard2.IsValid()) seh_guard2.Release();
                    while (!Game->StartScriptingEntity(guard2,seh_guard2,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND0",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                        seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND0",GROUP_SELECT_NONE,false,true,false);
                        while (seh_guard2.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                    Game->Pause(0.5f);
                    ParentClass.ArenaState=MAS_IN_ARENA;
                    Game->ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
                    seh_guard2.Release();
                    ParentClass.NeedBertForSpeech=false;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_NO",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                ChamTalk=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_SECOND_QUESTION",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.NeedBertForSpeech=true;
                    CScriptThing guard2=Game->GetThingWithScriptName("ArenaCellDoorGuard2");
                    CScriptGameResourceObjectScriptedThingBase seh_guard2;
                    if (seh_guard2.IsValid()) seh_guard2.Release();
                    while (!Game->StartScriptingEntity(guard2,seh_guard2,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    switch (ParentClass.ArenaRound) {
                    case 0:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND0",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND0",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 1:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND1",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND1",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND1_SECOND",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 2:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND2",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND2",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 3:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND3",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND3",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 4:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND4",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND4",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 5:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND5",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND5",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 6:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND6",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND6",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    case 7:
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND7",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Me,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND7",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(guard2,"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND7_SECOND",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (Game->GetHealth(seh_guard2.GetScriptThing())>0.0f) {
                            seh_guard2.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND7_SECOND",GROUP_SELECT_NONE,false,true,false);
                            while (seh_guard2.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        break;
                    }
                    Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                    Game->Pause(0.5f);
                    ParentClass.ArenaState=MAS_IN_ARENA;
                    Game->ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
                    seh_guard2.Release();
                    ParentClass.NeedBertForSpeech=false;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ANOTHER_REFUSAL",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready=true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->ModifyThingHealth(Me,10000.0f,false);
                Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                ParentClass.InHitCutsceneAlready=false;
            }
        } else if (IsDistanceBetweenThingsOver(Me,Game->GetThingWithScriptName("GuardingDoorMarkerRight"),1.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardingDoorMarkerRight").GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
            Turned=false;
        } else if (!Turned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Turned=true;
                Game->EntitySetFacingAngle(Me,Game->GetThingWithScriptName("GuardingDoorMarkerRight").GetAngleXY(),false);
            }
        }
    }
}