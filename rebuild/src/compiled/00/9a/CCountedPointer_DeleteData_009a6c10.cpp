// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CFontManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCFontManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6C10, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A6C10`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a6c10, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCFontManager@@@@SIXPAX@Z; stored by Reset 0x009A7340 (reloc_bindings stand-in ?Reset@CPFont@@QAEXPAVCFontManager@@@Z, caller 0x009A6610), called at 0x009A691E with a fresh CFontManager (ctor 0x009E2E30, RTTI vtable); Ego_r.pdb CSystemManager::PFontManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CFONTMANAGER_VIRTUAL_DTOR
#include "engine/CFontManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CFontManager>::DeleteData(void*);
