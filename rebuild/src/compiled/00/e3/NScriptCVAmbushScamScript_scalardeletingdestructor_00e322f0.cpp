// NScript::CV_AmbushScamScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_AmbushScamScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E322F0, 30 bytes, thiscall, `ret 4`; calls 00e32310=NScript::CV_AmbushScamScript::~CV_AmbushScamScript ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012e2428 slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00e2b090 (slot 2) registers 3 entity scripts whose PDB Alloc outer is CV_AmbushScamScript; PDB ??_G/??1CV_AmbushScamScript@NScript@@; refs=1; was CVtable_012e2428::`scalar deleting destructor'.
//   ??_GCV_AmbushScamScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_GCV_AmbushScamScript@NScript@@UAEPAXI@Z; FableWin.pdb ??1CV_AmbushScamScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012e2428, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CV_AmbushScamScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00E2AB60 (rebuild/src/compiled/00/e2/NScriptCVAmbushScamScript_CV_AmbushScamScript_00e2ab60.cpp), its own source unchanged. The constructor body is not part of this graded body.
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
