// NScript::CV_ArcheryCompetitionScript::DoCountDown(long) -- before an archery round: take scripted control of the hero
//   (wait until StartScriptingEntity succeeds), then show the remaining seconds of a script timer as screen titles once a
//   second ("n" for 0.6 s each), then "TEXT_QST_004_COUNTDOWN_40" (GO) and release the hero. Every terminate check returns.
// Leaf: ?DoCountDown@CV_ArcheryCompetitionScript@NScript@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E36750, 536 bytes, thiscall `ret 4`; RELOCATION_MATCH.
// Identity: Ego_r.pdb + FableWin.pdb ?DoCountDown@CV_ArcheryCompetitionScript@NScript@@QAEXJ@Z (Ego_r order DoTimesUp then
//   DoCountDown matches retail 0x00E36590 / 0x00E36750); caller CAC_Owner::Main 0x00E350D8 `ParentClass.DoCountDown(3)`.
// Callees: IsActiveThreadTerminating 0x00CB7940; CBaseObject 0x0099A380 / ~CBaseObject 0x0099A430; out-of-line
//   ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0; IsValid 0x00CD23B9; Release 0x00CD2770; operator delete 0x00BFE9BC
//   (inline CCountedPointer release); CCharString(const char*,long) 0x0099EBF0 / ~CCharString 0x0099EAE0; GFIntToCharString
//   0x0099F830 (__fastcall); CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 87-90 timer
//   (CTimer inline through pDSTGame 0x0143E8F8), 113 AddScreenTitleMessage, 376 Pause.
// Shape: the generated-script idiom `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` (script_language.hpp) is what lays the
//   exits out as retail does: the handle dtor is expanded once (wait-loop exit) and called out of line at the other exits,
//   the post-wait / in-loop exits share a block placed in line after the post-wait test. A plain `return;` gives the same
//   code with the exit blocks moved to the end (541 v 536).
// Header opt-in: FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN declares DoCountDown in CV_ArcheryCompetitionScript.h.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase*pDSTGame;
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()){}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer&CTimer::operator=(long value){pDSTGame->SetTimer(TimerIndex,value);return *this;}
inline long CTimer::GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
CCharString __fastcall GFIntToCharString(long);  // 0x0099F830

void NScript::CV_ArcheryCompetitionScript::DoCountDown(long count)
{
    TRetVoid _Fail_Val;
    CTimer timer;
    timer = count;
    CScriptGameResourceObjectScriptedThingBase seh;
    while (!Game->StartScriptingEntity(Game->GetHero(), seh, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    while (timer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->AddScreenTitleMessage(GFIntToCharString(timer.GetTime() + 1), 0.6f, false);
        Game->Pause(1.0f);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->AddScreenTitleMessage("TEXT_QST_004_COUNTDOWN_40", 1.0f, false);
    Game->Pause(0.3f);
    if (seh.IsValid())
        seh.Release();
}