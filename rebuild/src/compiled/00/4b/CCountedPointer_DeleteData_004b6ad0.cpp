// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CScriptBase>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCScriptBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B6AD0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x004B6AD0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_004b6ad0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCScriptBase@@@@SIXPAX@Z; stored by the landed CActiveQuest::CActiveQuest 0x004B0310 for PScript (+0x08; Ego_r.pdb CActiveQuest::PScript is CCountedPointer<CScriptBase>, the ctor's `CScriptBase* script` argument; the landed row's own comment names 0x004B6AD0 the CScriptBase destroy function) and by the CCountedPointer<CScriptBase>(T*) ctor 0x004B7A50.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CScriptBase>::DeleteData(void*);
