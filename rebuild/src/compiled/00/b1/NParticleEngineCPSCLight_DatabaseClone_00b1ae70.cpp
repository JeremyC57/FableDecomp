// NParticleEngine::CPSCLight::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCLight@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b1ae70.cpp: vtable slot 2 of NParticleEngine::CPSCLight; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF8020 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d, queue nf458): self is the PDB virtual ?DatabaseClone@CPSCLight@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x012A0624 slot 2, no call site), was ?DatabaseClone@CPSCLight@NParticleEngine@@QAIPAUCPrim@@XZ (a __fastcall member) on a local CPrim model.
//   The callee 0x00AF8020 is NParticleEngine::CPSCLight::Alloc, ?Alloc@CPSCLight@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?CreatePrim@@YIPAUCPrim@@PAVCPSCLight@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CPSCLIGHT_NO_CONTAINERS
#define FABLE_RETAIL_CPSCLIGHT_ALLOC
#include "engine/CPSCLight.h"

namespace NParticleEngine {
CPSCBase* CPSCLight::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
