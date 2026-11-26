// NParticleEngine::CPSCUpdateNormal::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCUpdateNormal@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b0/CThingTrackNode_DrawIsDrawable_00b0f970.cpp: vtable slot 2 of NParticleEngine::CPSCUpdateNormal; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF7E60 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d: caller census of the factory self respell, retail call at 0x00B0F974): self is the PDB virtual ?DatabaseClone@CPSCUpdateNormal@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x0129FC1C slot 2, no call site), was ?DatabaseClone@CPSCUpdateNormal@NParticleEngine@@QAEPAUIDrawableObj@@XZ on a local IDrawableObj model.
//   The callee 0x00AF7E60 is NParticleEngine::CPSCUpdateNormal::Alloc, ?Alloc@CPSCUpdateNormal@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?GetDrawableObject_00af7e60@@YIPAUIDrawableObj@@PAVCPSCUpdateNormal@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#pragma optimize("g",on)
#define FABLE_RETAIL_CPSCUPDATENORMAL_NO_CONTAINERS
#define FABLE_RETAIL_CPSCUPDATENORMAL_ALLOC
#include "engine/CPSCUpdateNormal.h"

namespace NParticleEngine {
CPSCBase* CPSCUpdateNormal::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
