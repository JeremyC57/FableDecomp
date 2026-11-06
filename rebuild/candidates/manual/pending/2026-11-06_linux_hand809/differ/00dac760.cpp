// NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main() -- the Oakvale guard: once the hero has bad deeds the guard has not yet dealt with and the hero comes within a deed-scaled range, allies with the hero, chases him down, freezes him in a cut scene and reads out the charge sheet (first time in full, later the "again" list), answers good / bad / neutral when talked to, and complains when the hero hits him
// Grade: DIFFER(5906v5867) -- one inline-threshold split of the implicit seh_me (STB) dtor: VC7.1 expands it at one T pad more than retail, so the expanded pads share their operator-delete tails the other way round (93 difflines, 39 bytes); this source grades RELOCATION_MATCH under the diagnostic /d2inlT72 (not a landing flag), the same split as 0x00E8EF80
// Next idea: the lane-wide STB-dtor inline cost: retail behaves as if the implicit ~CScriptGameResourceObjectScriptedThingBase (or its ~CCountedPointer member) cost a few IL units more than the header model (a user-declared inline dtor costs enough but adds a vptr store); test a cost-only respelling of the shared model over the landed STB rows, else leave parked
// Leaf: ?Main@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DAC760, 5867 bytes, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012D83C0 stored by
//   CNOVI_Guard::Alloc 0x00DAC580 (slot 0 the landed scalar deleting dtor 0x00DAC630, slot 2 the landed Init 0x00DAC650, slot 3 the
//   landed GetParentScript 0x00DAC620); Ego_r.pdb and FableWin.pdb both hold ?Main@CNOVI_Guard@CQ_NewOakValeIntroScript@NScript@@EAEXXZ.
// Callees: IsActiveThreadTerminating 0x00F35B30, the scripted-thing handle (CBaseObject 0x0099A380, IsValid 0x00CD23B9, Release
//   0x00CD2770, FollowThing 0x007E7320, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490, ~
//   0x007E74D0), ~CScriptThing 0x004AA840, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF; interface slots through Game.
// Data: ParentClass (+0x14) GoodDeedsPerformed +0x54, BadDeedsPerformed +0x58, GuardsDealtWithBadDeeds +0x68, GuardsSpokenOnce
//   +0x92, WhichBadDeedsPerformed[5] +0xfc; the two unnamed .data floats 0x013AC840 (3.0f, the arrest range) and 0x013AC844
//   ({0, 5, 10, 15}, the notice range by deed count, capped at 3) -- no PDB name, descriptive names.
// Shape: generated entity script: `for (;; NewScriptFrame())` main loop with the T-check at the top of the body, a T-check before every
//   nested statement, WAIT_UNTIL loops; the GetHealth > 0 speech blocks of the CNOVI_Victim 0x00DBCD60 model.
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_GUARD
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ECutsceneBehaviour.h"
#include "engine/fable_script_distance.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
// The two unnamed .data floats the guard reads (no PDB name; descriptive): 0x013AC840 = 3.0f, 0x013AC844 = {0, 5, 10, 15}.
extern float g_NOVIGuardArrestDistance;          // 0x013AC840
extern float g_NOVIGuardNoticeDistances[4];      // 0x013AC844
void NScript::CQ_NewOakValeIntroScript::CNOVI_Guard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.BadDeedsPerformed-ParentClass.GuardsDealtWithBadDeeds>0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),g_NOVIGuardNoticeDistances[ParentClass.BadDeedsPerformed<3?ParentClass.BadDeedsPerformed:3])) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),g_NOVIGuardArrestDistance)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),true);
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_SCRMSG_GUARD_GOING_AFTER",false,Me,Game->GetHero());
                    seh_me.FollowThing(Game->GetHero(),1.0f,true);
                }
                Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_PAUSED);
                while (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),g_NOVIGuardArrestDistance)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                if (ParentClass.BadDeedsPerformed-ParentClass.GuardsDealtWithBadDeeds>0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.GuardsDealtWithBadDeeds=ParentClass.BadDeedsPerformed;
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    while (!Game->IsHeroControlledByPlayer()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
                if (!ParentClass.GuardsSpokenOnce) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_10",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_20",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (ParentClass.WhichBadDeedsPerformed[0]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[1]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_DERELICTION_OF_DUTY",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[2]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_VIOLENCE",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[3]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_TEDDY_TO_BULLY",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[4]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_CONCEALED_AFFAIR",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_30",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_40",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_50",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_60",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    ParentClass.GuardsSpokenOnce=true;
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (ParentClass.WhichBadDeedsPerformed[0]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[1]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_DERELICTION_OF_DUTY",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[2]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_VIOLENCE",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[3]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_TEDDY_TO_BULLY",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (ParentClass.WhichBadDeedsPerformed[4]) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_CRIME_CONCEALED_AFFAIR",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_AFTER_READ_LIST",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            if (ParentClass.BadDeedsPerformed==0 && ParentClass.GoodDeedsPerformed>0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(conversation,"TEXT_QST_048_GUARD_ON_TALK_GOOD",false,Me,Game->GetHero());
            }
            else if (ParentClass.BadDeedsPerformed>0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(conversation,"TEXT_QST_048_GUARD_ON_TALK_BAD",false,Me,Game->GetHero());
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(conversation,"TEXT_QST_048_GUARD_ON_TALK_NEUTRAL",false,Me,Game->GetHero());
            }
            while (Game->IsConversationActive(conversation)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_GUARD_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
    }
}
