// NParticleEngine::CPSCEmitterGeneric::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCEmitterGeneric@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b15b00.cpp: vtable slot 2 of NParticleEngine::CPSCEmitterGeneric; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF7ED0 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d, queue nf458): self is the PDB virtual ?DatabaseClone@CPSCEmitterGeneric@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x012A002C slot 2, no call site), was ?DatabaseClone@CPSCEmitterGeneric@NParticleEngine@@QAEPAUIDrawableBase@@XZ on a local IDrawableBase model.
//   The callee 0x00AF7ED0 is NParticleEngine::CPSCEmitterGeneric::Alloc, ?Alloc@CPSCEmitterGeneric@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?GetDrawableObject@@YIPAUIDrawableBase@@PAVCPSCEmitterGeneric@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#pragma optimize("g",on)
#define FABLE_RETAIL_CPSCEMITTERGENERIC_CTOR
#define FABLE_RETAIL_CPSCEMITTERGENERIC_ALLOC
#include "engine/CPSCEmitterGeneric.h"

namespace NParticleEngine {
CPSCBase* CPSCEmitterGeneric::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
