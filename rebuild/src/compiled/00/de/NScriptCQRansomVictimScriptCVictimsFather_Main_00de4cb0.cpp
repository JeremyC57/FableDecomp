// NScript::CQ_RansomVictimScript::CVictimsFather::Main() -- the kidnapped boy's father (the village chief): calls the hero over until he talks, plays the ransom intro and pays the advance, then offers extra help while his bodyguard lives, reacts to the bodyguard's death and to being hit, and once the victim is rescued teleports home and plays the good outro (telling or not telling of the brother's treachery).
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive ctor and dtor definitions + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- retail expands the predicate ctor / dtor one level and calls the nested CScriptThing copy 0x004ABE90 / dtor 0x004AA840, while the plain temporaries expand the row-local inline ~CScriptThing; with plain inline VC7.1 calls ~CIsThingAlive whole or expands the nested dtor too (GOTCHAS chunks 799/810); no flag splits one inline tree
// Leaf: ?Main@CVictimsFather@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE4CB0, length 6778 bytes, thiscall, no EH frame, ends in its `ret` at 0x00DE6729.
// Name: slot 1 of the entity vtable 0x012DD190 (0 scalar deleting dtor 0x00DE19D0, 2 Init 0x00DE19F0, 4 OnPersist 0x00DE4610, landed
//   as CVictimsFather); Ego_r.pdb and FableWin.pdb both hold ?Main@CVictimsFather@CQ_RansomVictimScript@NScript@@EAEXXZ.
// Data: own IntroDone +0x1C, ReactedToBodyguardsDeath +0x1D; ParentClass QuestStartScreened +0x48, MissionSucceeded +0x49,
//   RescuedVictim +0x4B, ExtraAssistanceGiven +0x4F, ThanksCutsceneStarting +0x53, HitChief +0x54, seh_Father +0x7C; PMasterData
//   RansomVictimSaveVictim +0x12C, RansomVictimHaveVictimKilled +0x12D; pGlobalScriptDef RV_* rewards +0xF68..+0xF7C.
// Shape: generated entity script, a T-check before every statement; the father speaks through the parent's seh_Father.
#include "fable_compiler.h"
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_RansomVictimScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
inline CScriptThing::~CScriptThing() { }
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
// Inline control (owner decision 2026-11-08, device 3): the predicate's special members are compiled with no inlining of their own,
// so the CScriptThing member copy / destructor stay calls (0x004ABE90 / 0x004AA840) when Main expands them, as retail does at every
// predicate site; FABLE_RETAIL_CPREDICATEBASE_NOVTABLE keeps the empty base constructor from becoming a call under the region.
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
#define VF_SPEAK(TEXT,GROUP) \
    if (Game->GetHealth(ParentClass.seh_Father.GetScriptThing())>0.0f) { \
        ParentClass.seh_Father.Speak(Game->GetHero(),TEXT,GROUP,false,true,false); \
        while (ParentClass.seh_Father.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            if (IsActiveThreadTerminating()) return; \
        } \
        if (IsActiveThreadTerminating()) return; \
    }
#define VF_HIT_TEST(LINE) \
    (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))
#define VF_ON_HIT \
    { \
        if (IsActiveThreadTerminating()) return; \
        CScriptGameResourceObjectMovieBase speech_movie; \
        Game->StartMovieSequence("",speech_movie); \
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
        VF_SPEAK("TEXT_QST_B06_FATHER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT) \
        Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero()); \
        Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me); \
        ++ParentClass.HitChief; \
    }
void NScript::CQ_RansomVictimScript::CVictimsFather::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    { CQ_RansomVictimScript& parent = ParentClass; if (parent.seh_Father.IsValid()) parent.seh_Father.Release(); }
    while (!Game->StartScriptingEntity(Me,ParentClass.seh_Father,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_OPTIONAL");
    Game->SetThingHasInformation(Me,false,true,false);
    CTimer over_here_timer;
    CTimer facing_timer;
    while (!IntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (facing_timer.GetTime()==0) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            facing_timer=1;
        }
        if (over_here_timer.GetTime()==0 && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            Game->AddLineToConversation(conversation,"TEXT_QST_B06_FATHER_OVER_HERE",false,Me,Game->GetHero());
            over_here_timer=15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            ParentClass.PlayCutscene("CS_RANSOM_INTRO","NULL");
            Game->GiveHeroGold(7500);
            if (!ParentClass.QuestStartScreened) {
                if (IsActiveThreadTerminating()) return;
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_RANSOM_VICTIM_B_OBJECTIVE_02","WitchwoodCavern","Witchwood1");
                Game->KickOffQuestStartScreen(Game->GetActiveQuestName(),true,false);
                ParentClass.QuestStartScreened=true;
            }
            IntroDone=true;
        }
        if (VF_HIT_TEST(1)) VF_ON_HIT
    }
    if (IsActiveThreadTerminating()) return;
    Game->MiniMapRemoveMarker(Me);
    if (ParentClass.ExtraAssistanceGiven) {
        if (IsActiveThreadTerminating()) return;
        Game->ClearThingHasInformation(Me);
    }
    CScriptThing guard=Game->GetThingWithScriptName("VictimsFatherGuard");
    const CScriptThing* p_guard=&guard;
    while (!ParentClass.RescuedVictim) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass.ExtraAssistanceGiven && guard.IsAlive() && ParentClass.HitChief<=2) {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                VF_SPEAK("TEXT_QST_B06_FATHER_OFFER",GROUP_SELECT_NONE)
                Game->GiveHeroYesNoQuestion("TEXT_QST_B06_CHIEF_ASKS_FOR_HELP","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.ExtraAssistanceGiven=true;
                    Game->ClearThingHasInformation(Me);
                    Game->SetQuestCardGoldReward(Game->GetActiveQuestName(),pGlobalScriptDef->RV_AcceptedGuardLoweredGoldReward);
                    Game->SetQuestCardRenownReward(Game->GetActiveQuestName(),pGlobalScriptDef->RV_AcceptedGuardLoweredRenownReward);
                    VF_SPEAK("TEXT_QST_B06_FATHER_ACCEPTED_HELP_10",GROUP_SELECT_NONE)
                }
                else {
                    if (IsActiveThreadTerminating()) return;
                    VF_SPEAK("TEXT_QST_B06_FATHER_NOT_ACCEPTED_HELP_10",GROUP_SELECT_NONE)
                }
            }
            else {
                if (IsActiveThreadTerminating()) return;
                if (!ParentClass.ExtraAssistanceGiven && !p_guard->IsAlive()) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    VF_SPEAK("TEXT_QST_B06_FATHER_KILLED_BODYGUARD_TALK",GROUP_SELECT_NONE)
                }
                else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    VF_SPEAK("TEXT_QST_B06_FATHER_REMIND",GROUP_SELECT_RANDOM_NO_REPEAT)
                }
            }
        }
        if (VF_HIT_TEST(2)) VF_ON_HIT
        if (!ReactedToBodyguardsDeath && !p_guard->IsAlive() && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f)) {
            if (IsActiveThreadTerminating()) return;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            Game->AddLineToConversation(conversation,"TEXT_QST_B06_FATHER_KILLED_BODYGUARD_10",false,Me,Game->GetHero());
            Game->SetQuestCardGoldReward(Game->GetActiveQuestName(),pGlobalScriptDef->RV_LowerGoldRewardFromKillingChiefsGuard);
            ReactedToBodyguardsDeath=true;
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntityTeleportToPosition(Me,Game->GetThingWithScriptName("MK_RVG_CHIEF").GetPos(),0.0f,false,false);
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_OPTIONAL");
    Game->SetThingHasInformation(Me,false,true,false);
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (Game->GetThingWithScriptName("Victim").IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.ThanksCutsceneStarting=true;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                ParentClass.PlayCutscene("CS_RANSOM_OUTRO_GOOD","NULL");
                PMasterData->RansomVictimSaveVictim=true;
                Game->GiveHeroYesNoQuestion("TEXT_QST_B06_QUESTION_REVEAL_BROTHERS_TREACHERY","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true);
                long answer=Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer<0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    answer=Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (answer==1) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_RANSOM_OUTRO_GOOD_TELL","NULL");
                    Game->GiveHeroMorality(pGlobalScriptDef->RV_HeroTellMoralityValue);
                }
                else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.PlayCutscene("CS_RANSOM_OUTRO_GOOD_NOTELL","NULL");
                }
                ParentClass.MissionSucceeded=true;
                Game->SetCategoryActivity("ChiefsSonSurvived",true);
                Game->GiveHeroMorality(pGlobalScriptDef->RV_HeroGoodMoralityValue);
                PMasterData->RansomVictimSaveVictim=false;
                PMasterData->RansomVictimHaveVictimKilled=false;
            }
            else {
                if (IsActiveThreadTerminating()) return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                VF_SPEAK("TEXT_QST_B06_FATHER_REMIND",GROUP_SELECT_RANDOM_NO_REPEAT)
            }
        }
        if (VF_HIT_TEST(3)) VF_ON_HIT
    }
}