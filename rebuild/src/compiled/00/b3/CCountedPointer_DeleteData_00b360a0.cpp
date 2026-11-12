// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerSprites>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerSprites@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B360A0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B360A0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b360a0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerSprites@@@@SIXPAX@Z; stored by Reset 0x00B37730, called at 0x00B33E98 on `this+0x2A0` with a fresh CEnginePrimitiveManagerSprites (ctor 0x00BACF70, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::SpriteManager (5th).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerSprites.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerSprites>::DeleteData(void*);
