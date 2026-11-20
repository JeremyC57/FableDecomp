// CBriarRose::DoSpeech(const CCharString& line) -- once the speech timer has run out (and the script is not done), start a speech on Briar Rose (Me), point it at the hero, add the line, and restart the speech timer at 3
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Leaf: ?DoSpeech@CBriarRose@CQ_MinionCampScript@NScript@@AAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA2610, length 147 bytes (ret 4). Callers: 0x00DA1BF7, 0x00DA1DCA,
// 0x00DA21A1, 0x00DA2793, 0x00DA2846 (all inside the CBriarRose minion-camp script).
// Callees: the timer service (global 0x0143E8F8) virtual slots 90 (+0x168, GetTimer) and 89 (+0x164, SetTimer);
// IsDone @ 0x00F35B30 (on `this`; manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated);
// the game interface (Game, +4) virtual slots 364 (+0x5b0), 70 (+0x118, twice), 365 (+0x5b4) and 366 (+0x5b8). The
// slot names are descriptive.
// Layout = Ego_r.pdb CBriarRose (80 bytes): Game +4, Me +8, SpeechTimer +0x48.
// Shape: a positive remaining time or a finished script returns at once (both branch to the single `pop esi ; ret`);
// the speech handle lives in ebx; each `Game->Slot(..., Game->GetHero())` is the plain nested call, so the outer
// call's vtable is fetched into ebp BEFORE the inner call (`mov edx,ecx ; mov ebp,[edx]`).
// NF200 (2026-11-20, q6500): the DoSpeech family (0x00DA2610 / 0x00DA26B0 / 0x00DA27B0) moved together onto the shared
//   engine/CQ_MinionCampScript.h (FABLE_RETAIL_NSCRIPT), the prelude of the landed sibling 0x00DA0EF0 CBriarRose::Main: the nested class is
//   NScript::CQ_MinionCampScript::CBriarRose, so DoSpeech is the private Ego_r.pdb 0x00DC9700 + FableWin.pdb 0x037EF460
//   ?DoSpeech@CBriarRose@CQ_MinionCampScript@NScript@@AAEXABVCCharString@@@Z (was the global-class stand-in
//   ?DoSpeech@CBriarRose@@QAEXABVCCharString@@@Z). The row-local CBriarRose / CEntityScriptBase / CQ_MinionCampScript / CGameScriptInterface /
//   CTimerManager / CCharString views are gone; CTimer::GetTime / operator=(long) are the CTimer.h inline bodies retail expands
//   (pDSTGame slots 90 GetTimer / 89 SetTimer), defined after the include as the header asks.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_MinionCampScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
extern CGameScriptInterfaceBase* pDSTGame;                                // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

void NScript::CQ_MinionCampScript::CBriarRose::DoSpeech(const CCharString& line)
{
    if (SpeechTimer.GetTime() > 0)
        return;
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, line, false, Me, Game->GetHero());
    SpeechTimer = 3;
}
