// NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main() -- a Chapel of Evil acolyte: turns on the hero once the chapel is attacked, offers the hero's followers for sacrifice when talked to (intro / question / torture cut-scenes), pays the morality-scaled donation with its cool-thing rewards, and otherwise prays at the altar on a random timer.
// Grade: DIFFER(7692v7655) -- 21 masked lines (unchanged in chunk H40c, no grade spent: c2prio probes only). Re-read with --trace / --blocks: the eax / edx swaps at 0x00E6C3C1 / 0x00E6C432 are NOT the expression-temporary rotation -- they are allocator pieces of the followers._M_start CSE (candidate #94 at the victim pick takes edx on a copy preference `edx: -1`, retail eax), and the loop pair is one priority tie: i (#68) 100 v begin (#4) 92, i picks first and takes ebp because ebx costs 5400, begin gets ebx (retail begin ebp, i ebx). Block shares (K = candidates referenced in the block): preheader K 6 (+24 each), loop top `victim==followers[i]` w 4 K 2 (begin +16, i live-through -8), T block K 1, teleport block K 5 (i -20), bottom K 7 (begin +56, i +112)
// Next idea: retail needs ONE more candidate referenced in the loop-top block (K 3: begin 100 v i 96) or three more in the teleport block -- the lever that closed 0x00D647F0 in this chunk (a block-local named value that coalesces away adds a candidate without code). Probed without effect on the two priorities: `int i`, `i++`, `(victim==followers[i])==false`, the `continue` form, a named bool. Not yet probed: a block-local `const CScriptThing&` / pointer for ONE of the two followers[i] uses, a local copy of `teleport`, `followers.begin()[i]`, the STLport FABLE_RETAIL_STLP_VECTOR operator[] variants (c2prio --blocks on the lines 144-147 blocks is the metric, 25 s, no grade)
// Leaf: ?Main@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6BC50, length 7655 bytes, thiscall, no EH frame.
// Name: slot 1 of the entity vtable 0x012E8468 (0 scalar deleting dtor 0x00E6A5D0, 2 Init 0x00E6AAC0, 3 GetParentScript 0x00E6A5A0,
//   4 OnPersist 0x00E6A5B0, 5 OnPredicateFail 0x00E6A5C0, all landed as CCOE_AcolyteEvil) that Alloc 0x00E6A500 stores; Ego_r.pdb
//   and FableWin.pdb both hold ?Main@CCOE_AcolyteEvil@CV_ChapelOfEvilScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ChapelOfEvilScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define AE_START(SEH,WHO) \
    if (SEH.IsValid()) SEH.Release(); \
    while (!Game->StartScriptingEntity(WHO,SEH,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AE_ANSWER(answer) \
    long answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define AE_METHOD(N,TEXT) \
    if (ParentClass->TortureType==N) { \
        if (T) return; \
        args["$LINE2"]=TEXT; \
    }
#define AE_DONATION(COND,TEXT,N) \
    else if (COND) { \
        if (T) return; \
        args["$DONATION"]=TEXT; \
        morality_change=pGlobalScriptDef->ChapelMoralityRewardFraction[N]*ParentClass->MoralityCurrent; \
    }
void NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    AE_START(seh_me,Me)
    CScriptThing altar=Game->GetThingWithScriptName("M_SoundAltar");
    CScriptThing talk_point=Game->GetThingWithScriptName("M_COE_TalkToAcolyte");
    ParentClass->AttackedByHero=false;
    Game->SetThingPersistent(Me,false);
    for (;; NSF) {
        if (T) return;
        if (ParentClass->AttackedByHero) {
            if (T) return;
            if (seh_me.IsValid()) seh_me.Release();
            Game->SetScriptingStateGroup(Me,ESSG_NONE);
            Game->EntitySetThingAsEnemyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsEnemyOfThing(Game->GetHero(),Me);
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            Game->ClearThingHasInformation(Me);
            for (;;) {
                NSF;
                if (T) return;
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !ParentClass->AttackedByHero) {
            if (T) return;
            seh_me.ClearAllActionsIncludingLoopingAnimations();
            std::vector<CScriptThing> followers;
            CScriptThing victim;
            ParentClass->VictimTortured=false;
            if (Game->GetFollowingEntityList(Game->GetHero(),followers)) {
                if (T) return;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                AE_START(seh_hero,Game->GetHero())
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
                Game->Pause(0.5f);
                ParentClass->TeleportAcolytes(Me);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
                cast["HERO"]=seh_hero;
                cast["ACOLYTE"]=seh_me;
                if (followers.size()==1) {
                    if (T) return;
                    std::map<CCharString,CCharString> args;
                    args["$LINE2"]="TEXT_QST_019_FOLLOWERS_CHOOSE_SINGLE";
                    RunCutsceneMacro("CS_CHAPEL_EVIL_INTRO",cast,0,&args,false,true);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_019_SACRIFICE_VICTIM_SINGLE","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    AE_ANSWER(answer)
                    if (answer==1) {
                        if (T) return;
                        victim=followers[0];
                    }
                }
                else {
                    if (T) return;
                    std::map<CCharString,CCharString> args;
                    args["$LINE2"]="TEXT_QST_019_FOLLOWERS_CHOOSE_MULTIPLE";
                    RunCutsceneMacro("CS_CHAPEL_EVIL_INTRO",cast,0,&args,false,true);
                    Game->GiveHeroYesNoQuestion("TEXT_QST_019_SACRIFICE_VICTIM_MULTIPLE","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                    AE_ANSWER(answer)
                    if (answer==1) {
                        if (T) return;
                        victim=followers[rand()%followers.size()];
                        CScriptThing teleport=Game->GetThingWithScriptName("M_COE_FollowerTeleport");
                        if (followers.size()>1) {
                            if (T) return;
                            for (unsigned long i=0; i<followers.size(); ++i) {
                                if (!(victim==followers[i])) {
                                    if (T) return;
                                    Game->EntityTeleportToThing(followers[i],teleport,false);
                                }
                            }
                        }
                    }
                }
                if (victim.IsAlive()) {
                    if (T) return;
                    std::map<CCharString,CCharString> args;
                    ParentClass->GetMethodOfTorture();
                    AE_METHOD(1,"TEXT_QST_019_TORTURE_METHOD_ROPES")
                    else AE_METHOD(2,"TEXT_QST_019_TORTURE_METHOD_INSECTS")
                    else AE_METHOD(3,"TEXT_QST_019_TORTURE_METHOD_FIRE")
                    else AE_METHOD(4,"TEXT_QST_019_TORTURE_METHOD_SAWS")
                    else AE_METHOD(5,"TEXT_QST_019_TORTURE_METHOD_ACID")
                    else AE_METHOD(6,"TEXT_QST_019_TORTURE_METHOD_KNIVES")
                    RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE_START",cast,0,&args,false,true);
                    CScriptGameResourceObjectScriptedThingBase seh_victim;
                    while (victim.IsAlive() && !Game->StartScriptingEntity(victim,seh_victim,SCRIPT_AI_PRIORITY_HIGHEST)) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                    if (victim.IsAlive()) {
                        if (T) return;
                        ParentClass->TortureVictim(victim,seh_victim,seh_hero);
                        ParentClass->VictimTortured=true;
                    }
                }
                else {
                    if (T) return;
                    RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE_DENIED",cast,0,0,false,true);
                }
                Game->FixMovieSequenceCamera(false);
            }
            else {
                if (T) return;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("",movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_019_FOLLOWERS_NONE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                }
            }
            if (ParentClass->VictimTortured) {
                if (T) return;
                ParentClass->DonationThisVisit=ParentClass->VictimHealth*(rand()%3+1)+50;
                if (ParentClass->VictimIsTrader) {
                    if (T) return;
                    if (Game->IsQuestActive("Q_TraderEscort")) {
                        if (T) return;
                        ParentClass->DonationThisVisit=(long)((float)(rand()%3+1)*((float)ParentClass->VictimHealth*0.25f)+50.0f);
                    }
                    ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*1.125f);
                }
                else if (ParentClass->VictimIsBandit) {
                    if (T) return;
                    ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*0.5f);
                }
                else {
                    if (T) return;
                    if (ParentClass->VictimGender==2) {
                        if (T) return;
                        ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*1.175f);
                    }
                    else if (ParentClass->VictimIsGuard) {
                        if (T) return;
                        ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*1.375f);
                    }
                }
                if (ParentClass->VictimNightTime) {
                    if (T) return;
                    ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*1.75f);
                }
                if (ParentClass->VictimSunday) {
                    if (T) return;
                    ParentClass->DonationThisVisit=(long)((float)ParentClass->DonationThisVisit*1.55f);
                }
                ParentClass->MoralityCurrent=Game->GetHeroMorality();
                ParentClass->DonationTotal+=ParentClass->DonationThisVisit;
                Game->UpdateOnlineScore_ChapelOrTemple((float)ParentClass->DonationTotal);
                float donation;
                if (Game->GetHeroMoralityCategory()==0) {
                    if (T) return;
                    donation=(float)ParentClass->DonationThisVisit*pGlobalScriptDef->ChapelDonationLevelFiddleSuper;
                }
                else if (Game->GetHeroMoralityCategory()==1) {
                    if (T) return;
                    donation=(float)ParentClass->DonationThisVisit*pGlobalScriptDef->ChapelDonationLevelFiddleVery;
                }
                else if (Game->GetHeroMoralityCategory()==2) {
                    if (T) return;
                    donation=(float)ParentClass->DonationThisVisit*pGlobalScriptDef->ChapelDonationLevelFiddle;
                }
                else {
                    if (T) return;
                    donation=(float)ParentClass->DonationThisVisit;
                }
                std::map<CCharString,CCharString> args;
                float morality_change;
                if (donation>=pGlobalScriptDef->ChapelDonationLevel[5]) {
                    if (T) return;
                    args["$DONATION"]="TEXT_QST_019_DONATION_MAXIMUM";
                    ParentClass->AwardWeapon=true;
                    morality_change=pGlobalScriptDef->ChapelMoralityRewardFraction[5]*ParentClass->MoralityCurrent;
                }
                AE_DONATION(donation>pGlobalScriptDef->ChapelDonationLevel[4],"TEXT_QST_019_DONATION_HIGHEST",4)
                AE_DONATION(donation>pGlobalScriptDef->ChapelDonationLevel[3],"TEXT_QST_019_DONATION_HIGH",3)
                AE_DONATION(donation>pGlobalScriptDef->ChapelDonationLevel[2],"TEXT_QST_019_DONATION_MIDDLE",2)
                AE_DONATION(donation>pGlobalScriptDef->ChapelDonationLevel[1],"TEXT_QST_019_DONATION_LOW",1)
                else {
                    if (T) return;
                    args["$DONATION"]="TEXT_QST_019_DONATION_LOWEST";
                    morality_change=pGlobalScriptDef->ChapelMoralityRewardFraction[0]*ParentClass->MoralityCurrent;
                }
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    AE_START(seh_hero,Game->GetHero())
                    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
                    cast["HERO"]=seh_hero;
                    cast["ACOLYTE"]=seh_me;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("",movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_CHAPEL_EVIL_TORTURE_ENDED",cast,0,&args,false,true);
                    Game->FixMovieSequenceCamera(false);
                }
                Game->GiveHeroMorality(morality_change);
                ParentClass->MoralityCumulative+=morality_change;
                if (ParentClass->AwardWeapon && !ParentClass->ReceivedReward && !Game->IsObjectInThingsPossession(pGlobalScriptDef->ChapelDonationRewardObject,Game->GetHero())) {
                    if (T) return;
                    Game->GiveHeroObject(pGlobalScriptDef->ChapelDonationRewardObject,-1,false);
                    ParentClass->AwardWeapon=false;
                    ParentClass->ReceivedReward=true;
                }
                else if ((ParentClass->MoralityCumulative<=pGlobalScriptDef->ChapelMoralityChangeCoolThing1 || (float)ParentClass->DonationTotal>=pGlobalScriptDef->ChapelDonationLevelCoolThing1) && !ParentClass->GivenCoolThing1 && ParentClass->DonationThisVisit!=0) {
                    if (T) return;
                    if (altar.IsAlive()) {
                        if (T) return;
                        long conversation=Game->AddNewConversation(altar,false,false);
                        Game->AddPersonToConversation(conversation,Game->GetHero());
                        Game->AddLineToConversation(conversation,"TEXT_QST_019_PHASE2_REMOVE_AGE",false,altar,Game->GetHero());
                    }
                    Game->SetHeroAge(Game->GetHeroAge()-(float)(pGlobalScriptDef->ChapelDonationRewardAgeMin+rand()%pGlobalScriptDef->ChapelDonationRewardAgeVar));
                    ParentClass->GivenCoolThing1=true;
                }
                else if ((ParentClass->MoralityCumulative<=pGlobalScriptDef->ChapelMoralityChangeCoolThing2 || (float)ParentClass->DonationTotal>=pGlobalScriptDef->ChapelDonationLevelCoolThing2) && !ParentClass->GivenCoolThing2 && ParentClass->DonationThisVisit!=0) {
                    if (T) return;
                    if (altar.IsAlive()) {
                        if (T) return;
                        long conversation=Game->AddNewConversation(altar,false,false);
                        Game->AddPersonToConversation(conversation,Game->GetHero());
                        Game->AddLineToConversation(conversation,"TEXT_QST_019_NEW_TITLE_EVIL",false,altar,Game->GetHero());
                    }
                    Game->GiveHeroTitle(pGlobalScriptDef->ChapelDonationRewardTitle);
                    Game->GiveHeroRenownPoints(pGlobalScriptDef->ChapelRenown);
                    ParentClass->GivenCoolThing2=true;
                }
                ParentClass->DonationThisVisit=0;
                ParentClass->VictimTortured=false;
                Game->UpdateQuestInfoCounter(ParentClass->DonationCounter,ParentClass->DonationThisVisit,-1);
            }
        }
        else if (AcolytePrayerTimer.GetTime()==0) {
            if (T) return;
            AE_START(seh_me,Me)
            seh_me.ClearCommands();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("COE_AltarEvil"),true);
            while (seh_me.IsPerformingScriptTask()) {
                NSF;
                if (T) return;
            }
            if (T) return;
            if (rand()%5==0) {
                if (T) return;
                seh_me.PlayAnimation("ST_PRAY_INTO",false,false,true,false,true,true,false);
                seh_me.PlayLoopingAnimation("ST_PRAY_LOOP",2,false,false,true,false,true,true,false);
                seh_me.PlayAnimation("ST_PRAY_OUTOF",false,false,true,true,true,true,false);
            }
            else {
                if (T) return;
                seh_me.PlayAnimation("ST_PRAY_STANDING_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayLoopingAnimation("ST_PRAY_STANDING_LOOP",2,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayAnimation("ST_PRAY_STANDING_OUTOF",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            AcolytePrayerTimer=pGlobalScriptDef->ChapelAcolytePrayerTimer+rand()%pGlobalScriptDef->ChapelAcolytePrayerTimerVar;
        }
    }
}
