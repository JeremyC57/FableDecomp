// NScript::CQ_SecretPassageScript::CMotherTorture::Main() -- the hero's mother in the secret-passage torture chamber: live while alive, unkillable and off the combo counter with a quest-core minimap orb; unless already found, take control, wait for the hero to pull MotherCellLever, open MotherCellDoor, set the PRISON_ESCAPE objective 06 (GraveyardCircle), turn the orb green and mark her found; then follow the hero (3.0, region-following) with the danger music and the SCARLET_ROBE narrator, and each frame answer a hero talk (LETS_GO) or hero hit (ON_HIT, healing spells excepted) under a paused speech movie, else when the 15 s speech timer has run out say HIT_BY_ENEMY / FIGHTING_COMPLIMENT (combo > 3, 1 in 100) / USE_FLOURISH (1 in 200) / NOT_FAR_NOW (1 in 400)
// Grade: RELOCATION_MATCH (B24S2, stock flags)
// Leaf: ?Main@CMotherTorture@CQ_SecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEE2E0, 3102 bytes, thiscall, no EH frame, plain `ret` (per-exit epilogue copies), frame 0x84.
// Identity (proven): Ego_r.pdb ?Main@CMotherTorture@CQ_SecretPassageScript@NScript@@EAEXXZ; slot 1 of the CMotherTorture entity vtable
//   0x012DDE6C (stored by Alloc 0x00DEE1A0; slot 0 scalar deleting dtor 0x00DEE2C0, 2 Init 0x00DEE250, 3 GetParentScript 0x00DEE240,
//   5 OnPredicateFail 0x00DEE2B0); labels_anniversary_2026-11-04_waves23.tsv agrees. Prior art: Codex pass 13 00dee2e0 (DIFFER 3134v3102).
// Callees: SetPredicate 0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0; ctor inline with the copy 0x004ABE90 called, dtor
//   expanded one level: vtable restore from ebx + ~CScriptThing 0x004AA840 called), IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase
//   (IsValid 0x00CD23B9, Release 0x00CD2770, GetScriptThing 0x007E7490, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450, ~ 0x007E74D0),
//   MovieBase dtor 0x006E7B80 / PImp dtor 0x006E7AB0, CTimer ctor/dtor 0x00CD4450 / 0x00CD4470 (expanded RegisterTimer / DeregisterTimer
//   through pDSTGame 0x0143E8F8), rand 0x00BFEB16, CCharString 0x0099EBF0 / ~ 0x0099EAE0; interface slots 7, 8, 70, 72, 150, 152, 229, 264,
//   296, 350, 351, 362, 364, 366, 370, 379, 425, 469, 480, 517, 526, 594, 595, 655, 696; CScriptThing::MsgIsUsedBy (slot 20) inline.
// Data: engine/NScript_CQ_SecretPassageScript.h (opt-in FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE, overlay): ParentClass (CQ_SecretPassageScript*,
//   +0x14, Ego_r.pdb) -> MotherFound +0x4b.
// Shape (FableWin.pdb locals seh_me, MotherSpeechTimer, _Fail_Val, MotherLever / MotherCellDoor, speech_movie / PauseMyEntitiesPlease,
//   HLStyleConversationIndex; FableWin 0x038A0690 statement order): generated entity script (TRetVoid `_Fail_Val` exits, a T-check at every
//   nested block head and after every wait loop); the main loop is FableWin's for-scope form `if (false) {} else for (;; NSF)` and the
//   speech timer test is CTimer::operator== (FableWin calls ??8CTimer). CTimer ctor / dtor plain inline (the landed text), its operators
//   expanded; CIsThingAlive ctor / dtor under inline_depth(0) with CPREDICATEBASE_NOVTABLE (the H40 form of 0x00DE9690 / 0x00ED8680).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/NScript_CQ_SecretPassageScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/EScriptAIPriority.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/EMusicSetType.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE bool CTimer::operator==(long time) const { return GetTime() == time; }
inline bool CScriptThing::MsgIsUsedBy(const CCharString& user) const { const CScriptThing* imp = PImp.Data; if (!imp) return false; return imp->MsgIsUsedBy(user); }
void NScript::CQ_SecretPassageScript::CMotherTorture::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    if (!ParentClass->MotherFound) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        CScriptThing MotherLever=Game->GetThingWithScriptName("MotherCellLever");
        CScriptThing MotherCellDoor=Game->GetThingWithScriptName("MotherCellDoor");
        Game->EntitySetTargetable(MotherCellDoor,false);
        while (!MotherLever.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->OpenDoor(MotherCellDoor);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06","GraveyardCircle","");
        Game->MiniMapSetMarkerGraphic(Me,"HUD_ORB_GREEN_SMALL");
        ParentClass->MotherFound=true;
        if (seh_me.IsValid()) seh_me.Release();
    }
    Game->EntitySetDeedReactionsEnabled(Me,false);
    Game->EntitySetOpinionReactionsEnabled(Me,false);
    Game->EntitySetWillBeUsingNarrator(Me,"SCARLET_ROBE");
    Game->EntityFollowThing(Me,Game->GetHero(),3.0f,true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(),Me,true);
    Game->OverrideMusic(MUSIC_SET_DANGER,false,false);
    CTimer MotherSpeechTimer;
    if (false) {} else for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
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
                seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_LETS_GO",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
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
                seh_me.Speak(Game->GetHero(),"TEXT_QST_055_MOTHER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (seh_me.IsValid()) seh_me.Release();
        } else if (MotherSpeechTimer==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_HIT_BY_ENEMY",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game->GetPlayerCreatureCombatMultiplierRunningNumHits()>3 && rand()%100==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_FIGHTING_COMPLIMENT",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (Game->IsPlayerCreatureFlourishEnabled() && rand()%200==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_USE_FLOURISH",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
            else if (rand()%400==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_MOTHER_NOT_FAR_NOW",false,Me,Game->GetHero());
                MotherSpeechTimer=15;
            }
        }
    }
}
