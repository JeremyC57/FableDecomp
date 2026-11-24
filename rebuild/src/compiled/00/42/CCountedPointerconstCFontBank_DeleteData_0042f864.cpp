// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<const CFontBank>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@$$CBVCFontBank@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042F864, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc by Reset<const CFontBank> 0x0042FC13.
// Names (NF223, 2026-11-22; was the behavioural extern "C" DeleteIfSet, catalog _gapscan::sub_0042f864): FableWin.pdb and Ego_d.pdb
//   ?DeleteData@?$CCountedPointer@$$CBVCFontBank@@@@SIXPAX@Z (the const-T instantiation; Ego_r.pdb has only the non-const one); the landed
//   binder 0x0042FC13 binds that spelling here.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T (virtual ~CFontBank, slot 0); the explicit
//   instantiation emits the COMDAT.
#define FABLE_RETAIL_CFONTBANK
#include "engine/CFontBank.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<const CFontBank>::DeleteData(void*);
