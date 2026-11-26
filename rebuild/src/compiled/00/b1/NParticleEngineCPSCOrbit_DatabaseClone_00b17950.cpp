// NParticleEngine::CPSCOrbit::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCOrbit@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b17950.cpp: vtable slot 2 of NParticleEngine::CPSCOrbit; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF7F40 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d: caller census of the factory self respell, retail call at 0x00B17954): self is the PDB virtual ?DatabaseClone@CPSCOrbit@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x012A0374 slot 2, no call site), was ?DatabaseClone@CPSCOrbit@NParticleEngine@@QAEPAUIDrawableObj@@XZ on a local IDrawableObj model.
//   The callee 0x00AF7F40 is NParticleEngine::CPSCOrbit::Alloc, ?Alloc@CPSCOrbit@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?GetDrawableObject_00af7f40@@YIPAUIDrawableObj@@PAVCPSCOrbit@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#include "engine/NParticleEngine_CPSCOrbit.h"

namespace NParticleEngine {
CPSCBase* CPSCOrbit::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
