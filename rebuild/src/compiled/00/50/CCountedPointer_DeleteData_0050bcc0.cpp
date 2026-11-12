// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CWADBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCWADBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050BCC0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0050BCC0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0050bcc0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCWADBank@@@@SIXPAX@Z; stored by CCountedPointer<CWADBank>::Reset 0x0050F2B0 (reloc_bindings, caller 0x004FDAB0); Ego_r.pdb CWorldMap::LevelWADBank is CCountedPointer<CWADBank>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CWADBANK_VIRTUAL_DTOR
#include "engine/CWADBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CWADBank>::DeleteData(void*);
