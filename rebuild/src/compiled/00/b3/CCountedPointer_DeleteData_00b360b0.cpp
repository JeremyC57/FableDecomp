// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerFlareSprites>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerFlareSprites@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B360B0, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B360B0`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b360b0, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerFlareSprites@@@@SIXPAX@Z; stored by Reset 0x00B377E0, called at 0x00B33EBE on `this+0x2A8` with a fresh CEnginePrimitiveManagerFlareSprites (ctor 0x00BCE070, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::FlareSpriteManager (6th).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERFLARESPRITES_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManagerFlareSprites.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerFlareSprites>::DeleteData(void*);
