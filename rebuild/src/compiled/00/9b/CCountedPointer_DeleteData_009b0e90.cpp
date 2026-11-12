// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CSymbolMap>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCSymbolMap@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B0E90, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009B0E90`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009b0e90, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCSymbolMap@@@@SIXPAX@Z; stored by CCountedPointer<CSymbolMap>::Reset 0x009B1F50 (reloc_bindings, caller 0x009E2E30); Ego_r.pdb CFontManager::PSymbols / CDefinitionManager::PSymbolMap.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CSymbolMap.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CSymbolMap>::DeleteData(void*);
