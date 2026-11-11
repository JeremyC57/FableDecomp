// NScript::CV_BordelloScript::CBordelloLady::Main() -- a bordello lady (V_Bordello)
// Grade: DIFFER(6330v6330) -- size equal, 4 masked lines at 0x00E40339..0x00E40341: the natural-exit expanded ~seh_me stores (retail Data, Info, vptr = program order; ours vptr, Info, Data). Chunk H40: the predicate blocker is solved here (inline_depth(0) round the CIsThingAlive ctor + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE: 20 -> 4 lines)
// Next idea: the store order is the list scheduler's (c2.dll 0x1078734C): an explicit final return (v1), for(;;) with break / return at the loop top (v2 / v3, 29 lines, loop shape changes) and the ReleaseData dtor form (v4) leave it; dump the scheduler DAG of the last block (hand890 t21/t22) and compare with a row that keeps program order (0x00E84B80 first exit 0x00E84C79)
// Leaf: ?Main@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3EB10, 6330 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): the Ego_r.pdb / FableWin.pdb name sets hold ?Main@CBordelloLady@CV_BordelloScript@NScript@@EAEXXZ (private virtual);
//   the address has one .rdata reference, slot 1 of the entity vtable 0x012E3AC8 that the landed CBordelloLady::Alloc 0x00E3AB90
//   stores (slot 0 ??_G 0x00E3AC40, 2 Init 0x00E3AC70, 4 OnPersist 0x00E3BA80).
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_BORDELLOSCRIPT_CBORDELLOLADY
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/EHeroAbility.h"
#include "engine/CV_BordelloScript.h"
#include "engine/CIsThingAlive.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void NScript::CV_BordelloScript::CBordelloLady::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetCreatureBrain(Me,"BRAIN_PASSIVE_OVERRIDE");
    Game->SetWanderCentrePoint(Me,Me.GetHomePos());
    Game->SetWanderMinDistance(Me,0.0f);
    Game->SetWanderMaxDistance(Me,16.0f);
    Game->SetScriptingStateGroup(Me,(EScriptingStateGroups)4);
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !Game->IsReportedOrUnreportedCrimeKnown(ParentClass.BordelloVillage)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.seh_Whore.Release();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            if (ParentClass.BecomeNunnery) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),GetLHTSTag("HAPPY"),GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HeroTricking && ParentClass.IsHeroDressedAsLady() && !ParentClass.PlayerOwned) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),GetLHTSTag("MAGICIAN_HINT"),GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.PlayerOwned) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_FREE"),GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    Game->GiveHeroYesNoQuestion(GetLHTSTag("PARTY_FREE_QUESTION"),"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_FREE_FOLLOW_ME"),GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        ParentClass.seh_Whore=seh_me;
                        ParentClass.csargs["$SEXTALK"]="TEXT_CS_B13_SEX_"+Name+"_BOSS";
                        ParentClass.csargs["$ENDLINE"]=GetLHTSTag("PARTY_FREE_FINISHING_UP");
                        Game->SetCutsceneSkippable(false);
                        if (Name=="HEDWIG") {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG",true);
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX_QUICKIE",true);
                        }
                        Game->SetCutsceneSkippable(true);
                        PartiedAlready=true;
                        ParentClass.HeroPartying=true;
                        Game->SetNumberOfTimesHeroHasHadSex(Game->GetNumberOfTimesHeroHasHadSex()+1);
                        Game->SetHeroAsHavingHadSex(true);
                        WalkingDownstairs=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_FREE_DECLINED"),GROUP_SELECT_NONE,false,true,false);
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
                    if (HaveTalked) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (PartiedAlready) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_AGAIN"),GROUP_SELECT_NONE,false,true,false);
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
                                seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_PAID_REMINDER"),GROUP_SELECT_NONE,false,true,false);
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
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_PAID_INTRODUCTION"),GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        HaveTalked=true;
                    }
                    Game->GiveHeroYesNoQuestion(GetLHTSTag("PARTY_PAID_QUESTION"),"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    while (answer<0) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        answer=Game->MsgIsQuestionAnsweredYesOrNo();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (answer==1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (Game->GetHeroGold()>=GoldRequired) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_PAID_FOLLOW_ME"),GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            Game->GiveHeroGold(-GoldRequired);
                            ParentClass.seh_Whore=seh_me;
                            ParentClass.csargs["$SEXTALK"]="TEXT_CS_B13_SEX_"+Name;
                            ParentClass.csargs["$ENDLINE"]=GetLHTSTag("PARTY_PAID_PLEASED");
                            Game->SetCutsceneSkippable(false);
                            if (ParentClass.HadSex) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                if (Name=="HEDWIG") {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX_QUICKIE_HEDWIG",true);
                                }
                                else {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX_QUICKIE",true);
                                }
                            }
                            else {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                if (Name=="HEDWIG") {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX_HEDWIG",true);
                                }
                                else {
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                    ParentClass.PlayCutscene("CS_BORDELLO_PAYINGFORSEX",true);
                                }
                                ParentClass.HadSex=true;
                            }
                            Game->SetCutsceneSkippable(true);
                            PartiedAlready=true;
                            ParentClass.HeroPartying=true;
                            Game->SetNumberOfTimesHeroHasHadSex(Game->GetNumberOfTimesHeroHasHadSex()+1);
                            Game->SetHeroAsHavingHadSex(true);
                            WalkingDownstairs=true;
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_PAID_TOO_EXPENSIVE"),GROUP_SELECT_NONE,false,true,false);
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
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),GetLHTSTag("PARTY_PAID_DECLINED"),GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            }
            }
            if (WalkingDownstairs && IsDistanceFromThingToPositionOver(Me,Me.GetHomePos(),2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(Me.GetHomePos(),1.0f,ENTITY_MOVE_WALK,false,true);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                WalkingDownstairs=false;
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        if (!NoLongerWorking && ParentClass.BecomeNunnery) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            NoLongerWorking=true;
            Game->ClearThingHasInformation(Me);
            Game->EntitySetPersonalityOverride(Me,"OPINION_PERSONALITY_EX_PROSTITUTE");
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.seh_Whore.Release();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            seh_me.PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
        }
    }
}
// Attempts: see STATUS.tsv / ATTEMPTS.log (best = probe/bl_a4.cpp)
