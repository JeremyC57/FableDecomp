// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CLevelLoader>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCLevelLoader@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050C2E0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x0050C2E0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_0050c2e0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCLevelLoader@@@@SIXPAX@Z; stored by Reset 0x0050FB90, called at 0x00507C79 with a fresh CLevelLoader (ctor 0x006C26B0, RTTI vtable .?AVCLevelLoader@@); Ego_r.pdb CWorldMap::PLevelLoader is CCountedPointer<CLevelLoader>.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CLEVELLOADER
#include "engine/CLevelLoader.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CLevelLoader>::DeleteData(void*);
