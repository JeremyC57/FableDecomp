// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEnginePrimitiveManagerParticleSpriteGroup>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleSpriteGroup@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B36160, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B36160`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b36160, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEnginePrimitiveManagerParticleSpriteGroup@@@@SIXPAX@Z; stored by Reset 0x00B37FB0, called at 0x00B3404C on `this+0x308` with a fresh CEnginePrimitiveManagerParticleSpriteGroup (ctor 0x00BC14E0, RTTI vtable); Ego_r.pdb CEnginePrimitiveRenderer::ParticleSpriteGroupManager.
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEnginePrimitiveManagerParticleSpriteGroup>::DeleteData(void*);
