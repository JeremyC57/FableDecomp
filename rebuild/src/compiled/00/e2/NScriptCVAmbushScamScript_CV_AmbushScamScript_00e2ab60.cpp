// NScript::CV_AmbushScamScript::CV_AmbushScamScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, Game/MasterScriptData pair, own vtable, a registered timer at +0x7c, five empty CCharStrings at +0x80
// Leaf: ??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe @ 0x00E2AB60, 85 bytes (+ int3 padding), `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (??0CScriptBase@@QAE@XZ), CGameScriptInterfaceBase::RegisterTimer (+0x15c) through
// pDSTGame @ 0x0143E8F8, CCharString::CCharString() @ 0x0099E4B0 (five times, in an inline counted loop).
// Identity (naming wave 35d): catalog NScript::CV_AmbushScamScript::CV_AmbushScamScript; the landed PDB-named Alloc 0x00E322C0
//   (?Alloc@CV_AmbushScamScript@NScript@@SIPAVCScriptBase@@...; registered as "V_AmbushScam" by CScriptInfoManager::RegisterScripts
//   00cd52d0, wave3/labels_alloc.tsv) calls this address as its constructor (reloc_bindings: 00e322c0 binds the PDB ctor to 0x00E2AB60);
//   it stores vtable 0x012E2428, the vtable of the ??_G 0x00E322F0 / ~CV_AmbushScamScript 0x00E32310 rows; PDB
//   ??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in Ego_r.pdb / FableWin.pdb.
// Was: CVtable_012E2428::CVtable_012E2428(void* owner, CGameScriptInterface* game) (`// Leaf: ??0CVtable_012E2428`, a local
//   CScriptObject novtable intermediate base storing the interface at +0x40 and the owner at +0x44); same bytes.
// Model: the shared header engine/CV_AmbushScamScript.h (CLevelScriptBase<CQ_SunnyvaleMasterData>: Game +0x40, MasterScriptData
//   +0x44; RandomSpeechTimer +0x7c, RandomSpeech[5] +0x80); with FABLE_RETAIL_CLEVELSCRIPTBASE the CLevelScriptBase<T> ctor is
//   inline and stores both pointers before this class's vtable. The timer member's inline constructor registers the timer
//   (CTimer() : TimerIndex(RegisterTimer())); the CCharString[5] member array is built by an inline `dec ebx ; jne` loop (no EH in
//   this module, so no `eh vector constructor iterator`).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase*
//   (ESTABLISHED_SPELLINGS).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in)
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }

#include "engine/CV_AmbushScamScript.h"

NScript::CV_AmbushScamScript::CV_AmbushScamScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
