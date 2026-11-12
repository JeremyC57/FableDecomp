// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CDriveManager>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCDriveManager@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A6C20, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x009A6C20`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_009a6c20, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCDriveManager@@@@SIXPAX@Z; stored by Reset 0x009A73D0 (reloc_bindings stand-in ?Reset@CPDrive@@QAEXPAVCDriveManager@@@Z, caller 0x009A6610), called at 0x009A6878 with a fresh 8-byte object whose vptr is 0x0129AFF4 (RTTI .?AVCDriveManager@@) and at 0x009A7586, both on the global 0x013CAA28; the last of CSystemManager's Reset family 0x009A6FD0..0x009A73D0 (Display, Input, Graphic, Mesh, Sound, Font, Drive).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CDRIVEMANAGER_PATH
#include "engine/CDriveManager.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CDriveManager>::DeleteData(void*);
