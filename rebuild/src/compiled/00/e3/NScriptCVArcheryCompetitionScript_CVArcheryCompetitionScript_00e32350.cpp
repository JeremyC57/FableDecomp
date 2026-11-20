// NScript::CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, Game/MasterScriptData pair, vtable 0x012E2EE4, two registered timers, ten empty random-speech strings
// Leaf: ??0CV_ArcheryCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe @ 0x00E32350, 93 bytes (+ int3 padding), `ret 8`.
// Identity (NF174, q4102; two sources): Ego_r.pdb 0x00E805D0 / FableWin.pdb 0x03935360
//   ??0CV_ArcheryCompetitionScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z; the body stores vtable 0x012E2EE4, whose
//   slot 0 is the ??_G 0x00E37660 that calls the landed ~CV_ArcheryCompetitionScript 0x00E37680 (Ego_r / FableWin ??1...@NScript@@), and
//   slots 1..4 are the landed RegisterMain 0x00E324A0 / Main 0x00E32540 / Init 0x00E323B0 / OnPersist 0x00E37520 of
//   NScript::CV_ArcheryCompetitionScript; the only caller is the Alloc 0x00E37630 (`push 0A4h`, not landed).
// Was: the global-namespace CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game) on a local CScriptObject novtable
//   intermediate base and a local class model (`// Leaf: ??0CV_ArcheryCompetitionScript`); same bytes. Now on the shared header
//   engine/CV_ArcheryCompetitionScript.h (FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR declares the PDB ctor), the landed
//   0x00E2AB60 CV_AmbushScamScript recipe.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (??0CScriptBase@@QAE@XZ), CGameScriptInterfaceBase::RegisterTimer (+0x15c) through
//   pDSTGame @ 0x0143E8F8 (twice, the inline CTimer ctors), CCharString::CCharString() @ 0x0099E4B0 (ten times, inline counted loop).
// Layout: engine/CV_ArcheryCompetitionScript.h (Ego_r.pdb names at retail offsets): GameTimeLeft +0x6c, RandomSpeechTimer +0x70,
//   RandomSpeech[10] +0x74; CLevelScriptBase<CQ_SunnyvaleMasterData> stores Game +0x40 then MasterScriptData +0x44.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail's implicit ~CLevelScriptBase<T> (chunk 713 opt-in)
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR
#include "fable_string.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }

#include "engine/CV_ArcheryCompetitionScript.h"

NScript::CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
