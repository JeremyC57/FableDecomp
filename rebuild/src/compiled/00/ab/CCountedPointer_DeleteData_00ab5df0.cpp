// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CSurface>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCSurface@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB5DF0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00AB5DF0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00ab5df0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCSurface@@@@SIXPAX@Z; stored by Reset 0x00AB5E80 (reloc_bindings stand-in ?Reset@CCountedSurfacePointer@@QAEXPAVCSurface@@@Z, caller 0x00AB5390), called at 0x00AB53BB with a fresh CSurface (ctor 0x009F2D60, RTTI vtable); Ego_r.pdb CMouseDX::CursorGraphic is CCountedPointer<CSurface>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CSURFACE
#include "engine/CSurface.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CSurface>::DeleteData(void*);
