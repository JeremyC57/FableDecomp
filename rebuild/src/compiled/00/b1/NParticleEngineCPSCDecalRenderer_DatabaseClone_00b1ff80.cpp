// NParticleEngine::CPSCDecalRenderer::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCDecalRenderer@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b1ff80.cpp: vtable slot 2 of NParticleEngine::CPSCDecalRenderer; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF8100 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF463; the NF462 r460d / bB2 pattern): self is the PDB virtual ?DatabaseClone@CPSCDecalRenderer@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin;
//   its only retail reference is vtable 0x012A09BC slot 2 (abs 0x012A09C4), no call site), was ?DatabaseClone@CPSCDecalRenderer@NParticleEngine@@QAEPAUIDrawableObj@@XZ
//   on a local IDrawableObj model. The callee 0x00AF8100 is NParticleEngine::CPSCDecalRenderer::Alloc, ?Alloc@CPSCDecalRenderer@NParticleEngine@@SIPAVCPSCBase@2@XZ
//   (Ego_r + FableWin; RegisterPSCs 0x00AF8170 binds it; its self is respelt in this unit), was the stand-in GetDrawableObject_00af8100(CPSCDecalRenderer*) that no
//   table line binds. The copy is CPSCBase::DatabaseCopy (slot 18 = +0x48) on the shared engine/CPSCDecalRenderer.h (+ the ALLOC opt-in). Same bytes.
#pragma optimize("g",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPSCDECALRENDERER_CTOR
#define FABLE_RETAIL_CPSCDECALRENDERER_ALLOC
#include "engine/CPSCDecalRenderer.h"

namespace NParticleEngine {
CPSCBase* CPSCDecalRenderer::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
