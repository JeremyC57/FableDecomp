// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CFontDataBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCFontDataBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2960, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009E2960`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009e2960, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCFontDataBank@@@@SIXPAX@Z; stored by CCountedPointer<CFontDataBank>::Reset 0x009E3090 (reloc_bindings, caller 0x009E2E30); Ego_r.pdb CFontManager::PFontDataBank.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CFONTDATABANK_VIRTUAL_DTOR
#include "engine/CFontDataBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CFontDataBank>::DeleteData(void*);
