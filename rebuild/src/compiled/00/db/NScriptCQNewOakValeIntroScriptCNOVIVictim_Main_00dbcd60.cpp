// NScript::CQ_NewOakValeIntroScript::CNOVI_Victim::Main() -- the bullied boy in Oakvale: thanks the hero once the bully is subdued, cowers and pleads otherwise, shows the hitting-friends hint once, and when the hero hits him sulks to the bully or plays the brat-hit cutscene.
// Leaf: ?Main@CNOVI_Victim@CQ_NewOakValeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBCD60, length 4309 bytes.
// Name: slot 1 of the entity vtable 0x012D87B8 (0 scalar deleting dtor 0x00DAEE90, 2 Init 0x00DAEEB0, 3 GetParentScript
//   0x00DAEE80, all landed as CNOVI_Victim) that Alloc 0x00DAEDE0 stores; Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CNOVI_Victim@CQ_NewOakValeIntroScript@NScript@@EAEXXZ.
// Callees: SetPredicate 0x00F35B10 with the CIsThingConscious temporary (vtable 0x012C2FE8, ctor/dtor expanded),
//   IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 / ~ 0x004AA840 / IsAlive 0x004AB130, scripted-thing
//   handle (CBaseObject 0x0099A380, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770, Speak 0x007E7390,
//   IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490, operator= 0x008ABD10, ~ 0x007E74D0), MovieBase ctor
//   0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro
//   0x00CBFB7D, CCharString 0x0099EBF0 / ~ 0x0099EAE0, AddBadDeed 0x00DAEA70; interface slots through Game.
// Data: ParentClass (+0x14) BullySubdued / VictimShake / HeroAttackedVictim / GivenHeroTeddy /
//   VictimComplainsAboutLosingTeddy +0x96; own DisplayedGameInfo +0x1C, DoneThanks +0x1D (engine/CQ_NewOakValeIntroScript.h
//   opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER).
// Shape: generated entity script on the AffairMan model: TRetVoid return value, `for (; !false; NewScriptFrame())` main loop,
//   a T-check before every statement. The movie ctor/dtor and the STB dtor/assign are out of line (OUTOFLINE opt-ins) and the
//   cast-map ctor/dtor are declared-only specializations, so each cutscene block is calls only; the CIsThingConscious
//   ctor/dtor are inline so the predicate temporary expands as retail does.
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER
#include "engine/C3DVector.h"
#include "engine/CQ_NewOakValeIntroScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_cutscene.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_NewOakValeIntroScript::CNOVI_Victim::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing Bully=Game->GetThingWithScriptName("NOVI_Bully");
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.BullySubdued) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!DoneThanks) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoneThanks=true;
                if (seh_me.IsValid()) seh_me.Release();
                Game->EntitySetAsScared(Me,false);
                while (!ParentClass.BullyRanOff) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->SetIsPushableByHero(Me,true);
                Game->EntitySetAsUseMovementInActions(Me,true);
                Game->ClearThingHasInformation(Me);
            }
        }
        else if (ParentClass.VictimShake) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.VictimShake=false;
            Game->EntitySetFacingAngleTowardsThing(Me,Bully,false);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.BullySubdued) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.HeroAttackedVictim) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_THANKS_AFTER_HIT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_THANKS",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetAsScared(Me,false);
                Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.HeroAttackedVictim) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_PLEA",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                Game->EntitySetAsScared(Me,true);
                Game->EntitySetFacingAngleTowardsThing(Me,Bully,false);
            }
            if (!DisplayedGameInfo) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->IsXbox()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DisplayedGameInfo=true;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
            ParentClass.AddBadDeed((CQ_NewOakValeIntroScript::EBadDeeds)2);
            ParentClass.HeroAttackedVictim=true;
            if (!ParentClass.GivenHeroTeddy) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.GivenHeroTeddy=true;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                if (seh_hero.IsValid()) seh_hero.Release();
                while (!Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["BRAT"]=seh_me;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_OAKVALEINTRO_BRATHIT",mymap,0,0,false,true);
                Game->FixMovieSequenceCamera(false);
                Game->ClearThingHasInformation(Me);
                Game->EntitySetFacingAngleTowardsThing(Me,Bully,false);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                CScriptThing bully=Game->GetThingWithScriptName("NOVI_Bully");
                if (bully.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddPersonToConversation(conversation,bully);
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_VICTIM_EVIL_BROS",false,Me,bully);
                    Game->AddLineToConversation(conversation,"TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM",false,Me,bully);
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (seh_me.IsValid()) seh_me.Release();
                    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_EVIL_BROS",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
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
        if (ParentClass.VictimComplainsAboutLosingTeddy) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_048_VICTIM_EVIL_BROS_10",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            ParentClass.VictimComplainsAboutLosingTeddy=false;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}