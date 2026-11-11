// NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main() -- the hero's mother in the prison torture chamber: nags the hero to find his weapons and use the cell lever (brain state 1), follows the hero out once freed with fight comments and a health bar (state 2), flees the kraken, then waits for the graveyard and teleports the hero to its entrance.
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive ctor and dtor definitions + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- retail expands the predicate ctor / dtor one level and calls the nested CScriptThing copy 0x004ABE90 / dtor 0x004AA840, while the plain temporaries expand the row-local inline ~CScriptThing; with plain inline VC7.1 calls ~CIsThingAlive whole or expands the nested dtor too (GOTCHAS chunks 799/810); no flag splits one inline tree
// Leaf: ?Main@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD6A60, length 6494 bytes, thiscall, no EH frame, ends in its `ret` at 0x00DD83BB
//   (the closing graveyard wait loop sits after the epilogue).
// Name: slot 1 of the entity vtable 0x012DC108 (0 scalar deleting dtor 0x00DD6A40, 2 Init 0x00DD6950, 3 GetParentScript 0x00DD6940,
//   4 OnPersist 0x00DD94F0, 5 OnPredicateFail 0x00DD6A00, all landed as CMotherTorture); Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CMotherTorture@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ.
// Callees: SetPredicate 0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0), IsActiveThreadTerminating 0x00F35B30,
//   CScriptThing copy 0x004ABE90 / ~ 0x004AA840 / GetAngleXY 0x004AAA40 / IsAlive 0x004AB130, scripted-thing handle (IsValid
//   0x00CD23B9, Release 0x00CD2770, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, GetScriptThing 0x007E7490, ~ 0x007E74D0),
//   MovieBase dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0, CTimer 0x00CD4450 / ~ 0x00CD4470, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, rand 0x00BFEB16; interface slots through Game and pDSTGame.
// Data: own BrainState +0x1C; ParentClass HeroHasFoundWeapons +0x65, MotherCellDoorOpen +0x66, MotherFollowing +0x67,
//   MotherRescuedFromPrisonAutoSaveCheckPoint +0x7A; PMasterData->MotherDodgingKraken +0x84.
// Shape: generated entity script: TRetVoid return value, a T-check before every statement.
#include "fable_compiler.h"
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_MOTHERTORTURE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { if (!PImp.Data) return false; return PImp.Data->MsgIsUsedBy(user); }
// Inline control (owner decision 2026-11-08, device 3): the predicate's special members are compiled with no inlining of their own,
// so the CScriptThing member copy / destructor stay calls (0x004ABE90 / 0x004AA840) when Main expands them, as retail does at every
// predicate site; FABLE_RETAIL_CPREDICATEBASE_NOVTABLE keeps the empty base constructor from becoming a call under the region.
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#pragma inline_depth()
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#pragma inline_depth()
void NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (BrainState==1) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    Game->SetThingHasInformation(Me,false,false,false);
    Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
    CTimer nag_timer;
    nag_timer=10;
    CScriptThing lever=Game->GetThingWithScriptName("MotherCellLever");
    CScriptThing door=Game->GetThingWithScriptName("MotherCellDoor");
    Game->EntitySetTargetable(door,false);
    while (BrainState==1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (nag_timer.GetTime()==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.HeroHasFoundWeapons) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                nag_timer=10;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_GET_WEAPONS",false,Me,Game->GetHero());
            }
            else if (!ParentClass.MotherCellDoorOpen) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                nag_timer=10;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_USE_LEVER",false,Me,Game->GetHero());
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (!ParentClass.HeroHasFoundWeapons) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                nag_timer=10;
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_GET_WEAPONS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (!ParentClass.MotherCellDoorOpen) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                nag_timer=10;
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_USE_LEVER",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (lever.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HeroHasFoundWeapons) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                BrainState=2;
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.MotherRescuedFromPrisonAutoSaveCheckPoint=true;
                Game->AutoSaveCheckPoint();
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_055_PRISON_MOTHER_NOSTUFF_01",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_055_PRISON_MOTHER_NOSTUFF_02",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_03","PrisonBarracks","");
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (Game->IsLevelLoaded("PrisonTortureChamber")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.MotherCellDoorOpen=true;
        Game->OpenDoor(door);
        Game->EntitySetTargetable(lever,false);
        long conversation=Game->AddNewConversation(Me,false,false);
        Game->AddPersonToConversation(conversation,Game->GetHero());
        Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_GOT_WEAPONS",false,Me,Game->GetHero());
        Game->ActivateQuest("Q_TentacleKrakenBossFight");
        Game->SetHeroWillAsUsable(true);
        Game->MiniMapSetMarkerGraphic(Me,"HUD_ORB_GREEN_SMALL");
    }
    Game->OverrideMusic((EMusicSetType)24,false,false);
    if (BrainState==2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.MotherFollowing=true;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06","GraveyardCircle","");
        Game->SetDeathRecoveryMarkerName("PrisonTortureHSP");
        if (seh_me.IsValid()) seh_me.Release();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->ClearThingHasInformation(Me);
        long conversation=Game->AddNewConversation(Me,false,false);
        Game->AddPersonToConversation(conversation,Game->GetHero());
        Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_HURRY",false,Me,Game->GetHero());
        Game->EntitySetAsAllowedToFollowHero(Me,true);
        Game->EntityFollowThing(Me,Game->GetHero(),1.0f,true);
        Game->EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(Me,false);
        Game->EntitySetAsDamageable(Me,true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(),Me,true);
        Game->SetEntityAsFollowingHeroThroughTeleporters(Me,true);
        Game->AddQuestInfoBarHealth(Me,CRGBColour(255,255,255,255),"HUD_QUEST_ICON_SCARLET_ROBE",1.0f);
        Game->DisplayQuestInfo(true);
        while (Game->IsConversationActive(conversation)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CTimer comment_timer;
        while (BrainState==2 && !PMasterData->MotherDodgingKraken) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetPlayerCreatureCombatMultiplierRunningNumHits()>3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_CANT_HELP",false,Me,Game->GetHero());
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_HIT_BY_HERO",false,Me,Game->GetHero());
                comment_timer=15;
            }
            else if (comment_timer.GetTime()==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_HIT_BY_ENEMY",false,Me,Game->GetHero());
                    comment_timer=15;
                }
                else if (Game->GetPlayerCreatureCombatMultiplierRunningNumHits()>3 && rand()%100==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT",false,Me,Game->GetHero());
                    comment_timer=15;
                }
                else if (Game->IsPlayerCreatureFlourishEnabled() && rand()%200==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_USE_FLOURISH",false,Me,Game->GetHero());
                    comment_timer=15;
                }
                else if (rand()%400==0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_NOT_FAR_NOW",false,Me,Game->GetHero());
                    comment_timer=15;
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    CScriptThing run_point=Game->GetNearestWithScriptName(Me,"TKBF_MotherRunBetweenPoint");
    while (PMasterData->MotherDodgingKraken) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (rand()%200==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_FLEEING_KRAKEN",false,Me,Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer comment_timer;
    while (!Game->IsRegionLoaded("Graveyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_HIT_BY_HERO",false,Me,Game->GetHero());
            comment_timer=15;
        }
        else if (comment_timer.GetTime()==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_HIT_BY_ENEMY",false,Me,Game->GetHero());
                comment_timer=15;
            }
            else if (Game->GetPlayerCreatureCombatMultiplierRunningNumHits()>3 && rand()%100==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT",false,Me,Game->GetHero());
                comment_timer=15;
            }
            else if (Game->IsPlayerCreatureFlourishEnabled() && rand()%200==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_USE_FLOURISH",false,Me,Game->GetHero());
                comment_timer=15;
            }
            else if (rand()%400==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(conversation,"TEXT_QST_055_MOTHER_NOT_FAR_NOW",false,Me,Game->GetHero());
                comment_timer=15;
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing entrance=Game->GetThingWithScriptName("GraveyardFromPrisonRegionEntrance");
    if (entrance.IsAlive()) {
        Game->EntityTeleportToThing(Game->GetHero(),entrance,false);
        Game->EntitySetFacingAngle(Game->GetHero(),entrance.GetAngleXY(),true);
    }
    else {
        while (!Game->IsRegionLoaded("Graveyard")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}