// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerSpriteTrails>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerSpriteTrails@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B360D0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B360D0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b360d0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerSpriteTrails@@@@SIXPAX@Z; stored by Reset 0x00B37940, called at 0x00B33F0A on `this+0x2B8` with a fresh CEnginePrimitiveManagerSpriteTrails (ctor 0x00BCA770, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::SpriteTrailManager (8th).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#include "engine/CEnginePrimitiveManagerSpriteTrails.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerSpriteTrails>::DeleteData(void*);
