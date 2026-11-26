// NParticleEngine::CPSCRenderSprite::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCRenderSprite@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/af/CThingTrackNode_DrawIsDrawable_00afe580.cpp: vtable slot 2 of NParticleEngine::CPSCRenderSprite; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF7D10 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d, queue nf458): self is the PDB virtual ?DatabaseClone@CPSCRenderSprite@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x0129F67C slot 2, no call site), was ?DatabaseClone@CPSCRenderSprite@NParticleEngine@@QAEPAUIDrawableObj@@XZ on a local IDrawableObj model.
//   The callee 0x00AF7D10 is NParticleEngine::CPSCRenderSprite::Alloc, ?Alloc@CPSCRenderSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?GetDrawableObject@@YIPAUIDrawableObj@@PAVCPSCRenderSprite@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPSCRENDERSPRITE_ALLOC
#include "engine/CPSCRenderSprite.h"

namespace NParticleEngine {
CPSCBase* CPSCRenderSprite::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
