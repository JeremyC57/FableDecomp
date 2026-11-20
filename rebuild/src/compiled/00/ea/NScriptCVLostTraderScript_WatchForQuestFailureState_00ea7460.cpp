// NScript::CV_LostTraderScript::WatchForQuestFailureState(CScriptThing) -- wait for the quest-failed flag, then fail the active quest with the reason's text (trader died / brother died / none), deactivate it and add the Lost Trader quest card
// Leaf: ?WatchForQuestFailureState@CV_LostTraderScript@NScript@@QAEXVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA7460, 419 bytes (432 with padding), `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 /
//   ~CWideString 0x0099B510, ~CScriptThing 0x004AA840 (out of line), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   DeactivateQuestLater (slot 281), SetQuestAsFailed (slot 289), AddQuestCard (slot 292), GetActiveQuestName (slot 655).
// Strings: L"TEXT_QST_042_QUEST_FAILED_TRADER_DIED" 0x012EE3F8, L"TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED"
//   0x012EE398, L"" 0x0122D70C, "V_LostTrader" 0x012C4F98, "OBJECT_QUEST_CARD_LOST_TRADER" 0x012C4F78.
// Layout (retail = Ego_r.pdb - 0x14): g_bQuestFailed +0x4a, ReasonForFailure +0x54; interface pointer +0x40.
// Naming (NF162, 2026-11-20, q6516): was a row-local global `struct CV_LostTraderScript` over a local CQuestScriptBase (leaf
//   ?WatchForQuestFailureState@CV_LostTraderScript@@QAEXVCScriptThing@@@Z) with local CCharString / CWideString models; now on the shared
//   engine/CV_LostTraderScript.h (FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER declares the member) and fable_string.h. Identity: Ego_r.pdb +
//   FableWin.pdb ?WatchForQuestFailureState@CV_LostTraderScript@NScript@@QAEXVCScriptThing@@@Z; CLostTraderBrother::Main 0x00EA59E0 spawns
//   it through CSpawnedFunc1<CV_LostTraderScript, CScriptThing> vtable 0x012EE360 and binds this spelling (its only referencer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites: defined inline after the header.
inline CScriptThing::~CScriptThing() {}
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_LostTraderScript.h"

void NScript::CV_LostTraderScript::WatchForQuestFailureState(CScriptThing trader)
{
    while (!g_bQuestFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (ReasonForFailure == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_042_QUEST_FAILED_TRADER_DIED", true);
    } else if (ReasonForFailure == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_042_QUEST_FAILED_TRADERS_BROTHER_DIED", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, false);
}