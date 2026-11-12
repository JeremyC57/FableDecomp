// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManager2DSprites>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DSprites@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4B040, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B4B040`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b4b040, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManager2DSprites@@@@SIXPAX@Z; stored by Reset 0x00B4B4C0, called at 0x00B4AD04 on `this+0x28` with a fresh CEnginePrimitiveManager2DSprites (ctor 0x00BAD040, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer2D::Sprite2DManager (PDB +0x2C).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_VIRTUAL_DTOR
#include "engine/CEnginePrimitiveManager2DSprites.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManager2DSprites>::DeleteData(void*);
