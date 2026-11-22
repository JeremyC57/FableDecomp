// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CGraphicDataBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCGraphicDataBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419036, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc by Reset<CGraphicDataBank> 0x004194D1 and OpenGraphicBank 0x009F83D0.
// Names (NF223, 2026-11-22; was the behavioural extern "C" DeleteIfSet, catalog _global::DeleteViaVtable0_00419036): FableWin.pdb, Ego_r.pdb and
//   Ego_d.pdb ?DeleteData@?$CCountedPointer@VCGraphicDataBank@@@@SIXPAX@Z; both landed binders (0x004194D1, 0x009F83D0) bind that spelling here.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T (virtual ~CGraphicDataBank, slot 0); the explicit
//   instantiation emits the COMDAT.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CGraphicDataBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CGraphicDataBank>::DeleteData(void*);
