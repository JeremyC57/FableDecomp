// NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main() -- a Temple of Light acolyte: flees to the exit once the hero attacks the temple, gives advice when talked to, takes the hero's donation (morality-scaled level speech, morality reward, the cool-thing rewards, the gold) and otherwise prays at the altar on a random timer.
// Leaf: ?Main@CTOL_AcolyteGood@CV_TempleOfLightScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDD170, length 6594 bytes, thiscall, no EH frame, ends in its `ret` at 0x00EDEB31.
// Name: slot 1 of the entity vtable 0x012F3E94 (0 scalar deleting dtor 0x00EDC2A0, 2 Init 0x00EDC720, 3 GetParentScript 0x00EDC270,
//   4 OnPersist 0x00EDC280, 5 OnPredicateFail 0x00EDC290, all landed as CTOL_AcolyteGood) that Alloc 0x00EDC1D0 stores; Ego_r.pdb
//   and FableWin.pdb both hold ?Main@CTOL_AcolyteGood@CV_TempleOfLightScript@NScript@@EAEXXZ.
// Callees: IsActiveThreadTerminating 0x00F35B30, CScriptThing ~ 0x004AA840 / IsAlive 0x004AB130, scripted-thing handle (IsValid
//   0x00CD23B9, Release 0x00CD2770, ClearCommands 0x007E7360, MoveToPosition 0x007E72F0, Speak 0x007E7390, PlayAnimation 0x007E73D0,
//   PlayLoopingAnimation 0x007E73F0, ClearAllActionsIncludingLoopingAnimations 0x007E7410, IsPerformingScriptTask 0x007E7450,
//   GetScriptThing 0x007E7490, ~ 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, FadeOutAndKillEntity 0x00CBE716, vector<CScriptThing> dtor 0x008AC970,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, rand 0x00BFEB16; interface slots through Game and pDSTGame (AcolytePrayerTimer).
// Data: own AcolytePrayerTimer +0x1C; ParentClass DonationTotal +0x48, DonationThisVisit +0x4C, DonationCounter +0x50,
//   DonationInvalid +0x54, DonationActive +0x55, AttackedByHero +0x5B, ReceivedReward +0x5E, GivenCoolThing1/2 +0x5F/+0x60,
//   AwardWeapon +0x61, MoralityCurrent +0x68, MoralityCumulative +0x6C; pGlobalScriptDef Temple* members (+0x7FC .. +0x84C).
// Shape: generated entity script: TRetVoid return value, a T-check before every statement, and the main loop written
//   `for (;; NewScriptFrame()) { if (T) return ...; ... }` (chunk 824): the loop-top return then shares retail's expanded
//   ~altar / ~seh_me exit, and VC7.1 keeps the shared [~altar, ~seh_me, ret] return tail after the LAST (vector) pad as retail
//   does; the `for (; !T; NewScriptFrame())` form (chunk 811) left it after the first (exit_point) pad (GOTCHAS chunk 813 lever).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_TEMPLEOFLIGHTSCRIPT_ACOLYTEGOOD
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_TempleOfLightScript.h"
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
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716
void NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing altar=Game->GetThingWithScriptName("M_SoundAltar");
    Game->SetThingPersistent(Me,false);
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.AttackedByHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing exit_point=Game->GetThingWithScriptName("M_TOL_ExitPoint");
            if (exit_point.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(exit_point.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,exit_point,2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.0f,true,false);
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_064_ACOLYTE_ADVICE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        else if (!ParentClass.AttackedByHero && ParentClass.DonationActive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.DonationActive=false;
            ParentClass.DonationThisVisit=Game->GetBettingAmount();
            Game->SetBettingAccept(false);
            Game->SetBettingAmount(0);
            ParentClass.DonationTotal+=ParentClass.DonationThisVisit;
            Game->UpdateOnlineScore_ChapelOrTemple((float)ParentClass.DonationTotal);
            seh_me.ClearAllActionsIncludingLoopingAnimations();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            float morality_change=0.0f;
            ParentClass.MoralityCurrent=Game->GetHeroMorality();
            float donation;
            if (Game->GetHeroMoralityCategory()==6) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                donation=(float)ParentClass.DonationThisVisit*pGlobalScriptDef->TempleDonationLevelFiddleSuper;
            }
            else if (Game->GetHeroMoralityCategory()==5) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                donation=(float)ParentClass.DonationThisVisit*pGlobalScriptDef->TempleDonationLevelFiddleVery;
            }
            else if (Game->GetHeroMoralityCategory()==4) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                donation=(float)ParentClass.DonationThisVisit*pGlobalScriptDef->TempleDonationLevelFiddle;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                donation=(float)ParentClass.DonationThisVisit;
            }
            if (ParentClass.DonationInvalid) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_064_DONATION_WEAPON_OUT",GROUP_SELECT_RANDOM,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.DonationInvalid=false;
            }
#define FABLE_TOL_DONATION(Cond,Text,Index) \
            else if (Cond) { \
                if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
                CScriptGameResourceObjectMovieBase speech_movie; \
                Game->StartMovieSequence("",speech_movie); \
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
                    seh_me.Speak(Game->GetHero(),Text,GROUP_SELECT_RANDOM,false,true,false); \
                    while (seh_me.IsPerformingScriptTask()) { \
                        Game->NewScriptFrame(); \
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
                    } \
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); \
                } \
                morality_change=(1.0f-ParentClass.MoralityCurrent)*pGlobalScriptDef->TempleMoralityRewardFraction[Index]; \
            }
            else if (donation>=pGlobalScriptDef->TempleDonationLevel[5]) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_064_DONATION_MAXIMUM",GROUP_SELECT_RANDOM,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.AwardWeapon=true;
                morality_change=(1.0f-ParentClass.MoralityCurrent)*pGlobalScriptDef->TempleMoralityRewardFraction[5];
            }
            FABLE_TOL_DONATION(donation>pGlobalScriptDef->TempleDonationLevel[4],"TEXT_QST_064_DONATION_HIGHEST",4)
            FABLE_TOL_DONATION(donation>pGlobalScriptDef->TempleDonationLevel[3],"TEXT_QST_064_DONATION_HIGH",3)
            FABLE_TOL_DONATION(donation>pGlobalScriptDef->TempleDonationLevel[2],"TEXT_QST_064_DONATION_MIDDLE",2)
            FABLE_TOL_DONATION(donation>pGlobalScriptDef->TempleDonationLevel[1],"TEXT_QST_064_DONATION_LOW",1)
            FABLE_TOL_DONATION(donation>=0.0001f,"TEXT_QST_064_DONATION_LOWEST",0)
#undef FABLE_TOL_DONATION
            else if (donation<0.0001f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_064_DONATION_ZERO",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                morality_change=0.0f;
            }
            ParentClass.MoralityCumulative+=morality_change;
            Game->Pause(1.0f);
            Game->GiveHeroMorality(morality_change);
            if (ParentClass.AwardWeapon && !ParentClass.ReceivedReward && !Game->IsObjectInThingsPossession(pGlobalScriptDef->TempleDonationRewardObject,Game->GetHero())) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroObject(pGlobalScriptDef->TempleDonationRewardObject,-1,false);
                ParentClass.AwardWeapon=false;
                ParentClass.ReceivedReward=true;
            }
            else if ((ParentClass.MoralityCumulative>=pGlobalScriptDef->TempleMoralityChangeCoolThing1 || (float)ParentClass.DonationTotal>=pGlobalScriptDef->TempleDonationLevelCoolThing1) && !ParentClass.GivenCoolThing1 && ParentClass.DonationThisVisit!=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (altar.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(altar,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_064_PHASE2_REMOVE_AGE",false,altar,Game->GetHero());
                }
                Game->SetHeroAge(Game->GetHeroAge()-(float)(pGlobalScriptDef->TempleDonationRewardAgeMin+rand()%pGlobalScriptDef->TempleDonationRewardAgeVar));
                ParentClass.GivenCoolThing1=true;
            }
            else if ((ParentClass.MoralityCumulative>=pGlobalScriptDef->TempleMoralityChangeCoolThing2 || (float)ParentClass.DonationTotal>=pGlobalScriptDef->TempleDonationLevelCoolThing2) && !ParentClass.GivenCoolThing2 && ParentClass.DonationThisVisit!=0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (altar.IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(altar,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_064_NEW_TITLE_GOOD",false,altar,Game->GetHero());
                }
                Game->GiveHeroTitle(pGlobalScriptDef->TempleDonationRewardTitle);
                Game->GiveHeroRenownPoints(pGlobalScriptDef->TempleRenown);
                ParentClass.GivenCoolThing2=true;
            }
            if (!ParentClass.DonationInvalid) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroGold(-ParentClass.DonationThisVisit);
            }
            ParentClass.DonationThisVisit=0;
            Game->UpdateQuestInfoCounter(ParentClass.DonationCounter,ParentClass.DonationThisVisit,-1);
            std::vector<CScriptThing> acolytes;
            long count=Game->GetAllThingsWithScriptName("TOL_AcolyteGood",acolytes);
            for (long i=0; i<count; ++i) {
                if (acolytes[i].IsAlive()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->MiniMapRemoveMarker(acolytes[i]);
                }
            }
            if (altar.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->MiniMapAddMarker(altar,"HUD_ORB_QUEST_VIGNETTE");
            }
        }
        else if (AcolytePrayerTimer.GetTime()==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("TOL_AltarGood"),true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (rand()%5==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_PRAY_INTO",false,false,true,false,true,true,false);
                seh_me.PlayLoopingAnimation("ST_PRAY_LOOP",2,false,false,true,false,true,true,false);
                seh_me.PlayAnimation("ST_PRAY_OUTOF",false,false,true,true,true,true,false);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.PlayAnimation("ST_PRAY_STANDING_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayLoopingAnimation("ST_PRAY_STANDING_LOOP",2,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                seh_me.PlayAnimation("ST_PRAY_STANDING_OUTOF",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            }
            AcolytePrayerTimer=pGlobalScriptDef->TempleAcolytePrayerTimer+rand()%pGlobalScriptDef->TempleAcolytePrayerTimerVar;
        }
    }
}