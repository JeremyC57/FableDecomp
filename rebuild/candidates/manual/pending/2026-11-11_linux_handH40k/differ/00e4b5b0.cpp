// NScript::CV_BanditTollScript::CTollBandit::Main() -- the toll bandit: set up the toll, spawn the parent's toll-paid watcher, then run the toll state machine (1 wait for the reluctant trader, 2 halt the hero and ask for the toll, 3 hero has no money, 4 wait for the toll, 5 toll paid, 6 bandits angered: fight, then flee to the escape point and fade out)
// Grade: DIFFER(10610v10594) -- 591 masked lines, 170 hunks (697 before; chunk H40j, 3 grades). Levers: BC_PAY `long answer = 0;` (retail `xor ebp,ebp` at 0x00E4C0AC) and call_timer at function scope with no `{ }` block (retail runs ~CTimer on [esp+10h] at the exit 0x00E4DD1F). Left: frame-slot order (scalars retail lets share a slot) and the hunks that follow from it.
// Next idea: replay the frame packer (tools/pk2.sh, no grade) against retail's slots before any spelling: look for one counter / answer variable shared across rounds (the 0x00D647F0 lever). Byte-neutral: dropping the STB OUTOFLINE_DTOR. H40k (3 grades): /d2inlT73, T75 and a row-local inline ~CScriptThing all = 591 (as the draft).
// Leaf: ?Main@CTollBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4B5B0, 10594 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CTollBandit entity vtable 0x012E4D88 (slot 0 the landed scalar deleting dtor 0x00E4B1A0,
//   slot 2 Init 0x00E48BF0, 3 GetParentScript 0x00E48BE0, 4 OnPersist 0x00E4B400); Ego_r.pdb / FableWin.pdb
//   ?Main@CTollBandit@CV_BanditTollScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CV_BanditTollScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C
extern "C" int __cdecl rand();
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
__forceinline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define BC_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define BC_SAY(TEXT,G) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,G,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define BC_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define BC_LINE(CONV,TEXT) Game->AddLineToConversation(CONV,TEXT,false,Me,Game->GetHero())
#define BC_WAIT_TASK \
    while (seh_me.IsPerformingScriptTask()) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define BC_WAIT_CONV(CONV) \
    while (Game->IsConversationActive(CONV)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define BC_SET_FEE \
    std::vector<CScriptThing> followers; \
    Game->GetFollowingEntityList(Game->GetHero(),followers); \
    if (followers.size()) { \
        if (T) return; \
        ParentClass.BanditTollFee=pGlobalScriptDef->TollBanditFeeWithFollower; \
    } \
    else { \
        if (T) return; \
        ParentClass.BanditTollFee=pGlobalScriptDef->TollBanditFee; \
    }
#define BC_ASK(TEXT) \
    if (T) return; \
    Game->GiveHeroYesNoQuestion(TEXT,"TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define BC_PAY \
    long answer=0; \
    if (ParentClass.BanditTollFee==pGlobalScriptDef->TollBanditFee) { \
        BC_ASK("TEXT_QST_011_TOLLBANDIT_QUESTION_NO_FOLLOWER") \
    } \
    else if (ParentClass.BanditTollFee==pGlobalScriptDef->TollBanditFeeWithFollower) { \
        BC_ASK("TEXT_QST_011_TOLLBANDIT_QUESTION_WITH_FOLLOWER") \
    } \
    if (answer==1 && Game->GetHeroGold()>=ParentClass.BanditTollFee) { \
        if (T) return; \
        ParentClass.TollPaid=true; \
        Game->GiveHeroGold(-ParentClass.BanditTollFee); \
        Game->EntityGiveGold(Me,ParentClass.BanditTollFee); \
    } \
    else { \
        if (T) return; \
        if (answer==1) { \
            if (T) return; \
            BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_NOT_ENOUGH_MONEY",GROUP_SELECT_RANDOM) \
        } \
        else if (answer==0 && Game->GetHeroGold()>=ParentClass.BanditTollFee) { \
            if (T) return; \
            BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_HAS_MONEY",GROUP_SELECT_RANDOM) \
        } \
        else { \
            if (T) return; \
            BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_FAR_NO_MONEY",GROUP_SELECT_NONE) \
        } \
    }
void NScript::CV_BanditTollScript::CTollBandit::Main()
{
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    BC_START
    SetUpBanditToll();
    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CV_BanditTollScript>(CCharString("ParentClass.")+CCharString("CheckForBanditTollPaid"),0,&ParentClass,&CV_BanditTollScript::CheckForBanditTollPaid),CCharString(""));
    CScriptThing escape=Game->GetThingWithScriptName("M_BanditEscapePoint");
    if (ParentClass.BanditsAngered) {
        if (T) return;
        TollState=6;
    }
    if ((TollState==5 && !ParentClass.TollPaid) || (!ParentClass.BanditsAngered && TollState==6)) {
        if (T) return;
        TollState=2;
    }
    Game->SetThingHasInformation(Me,false,true,false);
    long kill_distance=pGlobalScriptDef->KillTraderTriggerDistance;
    while (TollState==1) {
        NSF;
        if (T) return;
        if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)kill_distance) && !ParentClass.TraderDead) {
            if (T) return;
            CScriptThing trader=Game->GetThingWithScriptName("ReluctantTrader");
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,trader);
            Game->AddLineToConversation(conversation,"TEXT_QST_011_TOLLBANDIT_TRADER_REFUSES_20",false,trader,Me);
            Game->AddLineToConversation(conversation,"TEXT_QST_011_TOLLBANDIT_TRADER_REFUSES_30",false,Me,trader);
            ParentClass.AssassinateReluctantTrader=true;
            BC_WAIT_CONV(conversation)
            TollState=2;
            conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,trader);
            Game->AddLineToConversation(conversation,"TEXT_QST_011_TOLLBANDIT_TRADER_DIES_10",false,Me,trader);
            KilledReluctantTrader=true;
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            seh_me.PlayAnimation("ST_WAVE_SPECIAL_02",false,false,false,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_CALL_HERO_OVER_10");
        }
        if (ParentClass.TraderDead) {
            if (T) return;
            TollState=2;
        }
        Should_I_Flee(seh_me);
    }
    if (T) return;
    CTimer call_timer;
    call_timer=pGlobalScriptDef->TollBanditCallHeroTimer;
    while (TollState==2 && !ParentClass.BanditsAngered) {
        NSF;
        if (T) return;
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        if (ParentClass.TollPaid) {
            if (T) return;
            TollState=4;
        }
        if (IsDistanceBetweenThingsUnder(Me,Game->GetHero(),6.0f)) {
            if (T) return;
            BC_SET_FEE
            seh_me.PlayAnimation("HALT",false,false,false,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            BC_MOVIE
            if (ParentClass.BanditTollFee==pGlobalScriptDef->TollBanditFee) {
                if (T) return;
                BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_TALK_5",GROUP_SELECT_NONE)
            }
            else if (ParentClass.BanditTollFee==pGlobalScriptDef->TollBanditFeeWithFollower) {
                if (T) return;
                BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_WITH_FOLLOWER_TALK_10",GROUP_SELECT_NONE)
            }
            BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_TALK_10",GROUP_SELECT_NONE)
            if (Game->GetHeroGold()<ParentClass.BanditTollFee) {
                if (T) return;
                BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_TALK_NO_MONEY",GROUP_SELECT_NONE)
                TollState=3;
            }
            else {
                if (T) return;
                BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_TALK_MONEY",GROUP_SELECT_NONE)
                TollState=4;
                BC_PAY
            }
        }
        else if (call_timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)pGlobalScriptDef->TollBanditCallHeroDistance)) {
            if (T) return;
            call_timer=pGlobalScriptDef->TollBanditCallHeroTimer;
            seh_me.PlayAnimation("ST_WAVE_SPECIAL_02",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            BC_WAIT_TASK
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_CALL_HERO_OVER_10");
        }
        Should_I_Flee(seh_me);
    }
    if (T) return;
    long warn_distance=pGlobalScriptDef->TollBanditWarningOverDistance;
    CTimer warn_timer;
    while (TollState==3 && !ParentClass.BanditsAngered) {
        NSF;
        if (T) return;
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        if (warn_timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)warn_distance)) {
            if (T) return;
            warn_timer=pGlobalScriptDef->TollBanditWarnHeroTimer;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_NEAR_NO_MONEY");
            seh_me.PlayAnimation("ST_POINT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            BC_WAIT_TASK
        }
        else if (IsDistanceBetweenThingsOver(Me,Game->GetHero(),(float)warn_distance)) {
            if (T) return;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_FAR_NO_MONEY");
            TollState=4;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            BC_MOVIE
            BC_SAY("TEXT_QST_011_TOLLBANDIT_HERO_TALK_NO_MONEY",GROUP_SELECT_NONE)
        }
        if (Game->GetHeroGold()>=ParentClass.BanditTollFee) {
            if (T) return;
            TollState=4;
        }
        Should_I_Flee(seh_me);
    }
    if (T) return;
    bool hero_left=false;
    warn_timer=0;
    long left_distance=pGlobalScriptDef->TollBanditHeroLeftDistance;
    CScriptThing nearest=Game->GetNearestWithScriptName(Game->GetHero(),"TollBandit");
    while (TollState==4 && !ParentClass.BanditsAngered) {
        NSF;
        if (T) return;
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        if (ParentClass.TollPaid) {
            if (T) return;
            TollState=5;
            if (nearest==Me) {
                if (T) return;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_TOLLPAID_10");
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_TOLLPAID_20");
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_TOLLPAID_30");
                seh_me.PlayAnimation("ST_SALUTE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                BC_WAIT_TASK
                BC_WAIT_CONV(conversation)
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            BC_SET_FEE
            BC_MOVIE
            BC_SAY("TEXT_QST_011_TOLLBANDIT_IMPATIENT",GROUP_SELECT_RANDOM)
            BC_PAY
        }
        if (!hero_left && IsDistanceBetweenThingsOver(Me,Game->GetHero(),(float)left_distance)) {
            if (T) return;
            hero_left=true;
        }
        else if (hero_left && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)TollBanditPayMeDistance)) {
            if (T) return;
            hero_left=false;
            if (Game->GetHeroGold()<ParentClass.BanditTollFee) {
                if (T) return;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_NOT_ENOUGH_MONEY_10");
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_NOT_ENOUGH_MONEY_20");
                BC_WAIT_CONV(conversation)
                seh_me.PlayAnimation("ST_POINT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                BC_WAIT_TASK
            }
            else {
                if (T) return;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_HAS_MONEY_10");
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_HAS_MONEY_20");
                BC_WAIT_CONV(conversation)
            }
        }
        else if (warn_timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)TollBanditPayMeDistance)) {
            if (T) return;
            warn_timer=pGlobalScriptDef->TollBanditWarnHeroTimer;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            if (Game->GetHeroGold()<ParentClass.BanditTollFee) {
                if (T) return;
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_CLOSE_NO_MONEY");
                seh_me.PlayAnimation("ST_POINT",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                BC_WAIT_TASK
            }
            else if (rand()%4==0) {
                if (T) return;
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_IMPATIENT_10");
            }
            else if (rand()%4==0) {
                if (T) return;
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_IMPATIENT_20");
            }
            else {
                if (T) return;
                BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_IMPATIENT_30");
            }
        }
        Should_I_Flee(seh_me);
    }
    if (T) return;
    Game->ClearThingHasInformation(Me);
    warn_timer=0;
    while (TollState==5 && !ParentClass.BanditsAngered) {
        NSF;
        if (T) return;
        Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
        if (warn_timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),(float)TollBanditPayMeDistance)) {
            if (T) return;
            warn_timer=pGlobalScriptDef->TollBanditWarnHeroTimer;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            BC_LINE(conversation,"TEXT_QST_011_TOLLBANDIT_HERO_PAID_NOT_MOVED");
        }
        Should_I_Flee(seh_me);
    }
    if (T) return;
    if (ParentClass.BanditsAngered) {
        if (T) return;
        TollState=6;
    }
    while (TollState==6) {
        NSF;
        if (T) return;
        if (ParentClass.BanditsFlee) {
            if (T) return;
            BC_START
            float close_to_exit=pGlobalScriptDef->TollBanditCloseToExit;
            C3DVector escape_pos=escape.GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),escape_pos,close_to_exit+2.0f)) {
                NSF;
                if (T) return;
                seh_me.MoveToPosition(escape_pos,close_to_exit,MOVE_TYPE_RUNNING,false,true);
                BC_WAIT_TASK
            }
            if (T) return;
            FadeOutAndKillEntity(Me,pGlobalScriptDef->TollBanditTimeToFade,true,true);
            return;
        }
        Game->EntitySetInFaction(Me,"FACTION_BANDITS");
        seh_me.FollowThing(Game->GetHero(),1.0f,true);
        Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
        seh_me.Release();
        while (!ParentClass.BanditsFlee) {
            NSF;
            if (T) return;
        }
        if (T) return;
    }
    if (T) return;
}
