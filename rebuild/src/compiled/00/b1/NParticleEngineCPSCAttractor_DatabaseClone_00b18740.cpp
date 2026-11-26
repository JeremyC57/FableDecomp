// NParticleEngine::CPSCAttractor::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCAttractor@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b18740.cpp: vtable slot 2 of NParticleEngine::CPSCAttractor; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF7FB0 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d, queue nf458): self is the PDB virtual ?DatabaseClone@CPSCAttractor@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x012A04AC slot 2, no call site), was ?DatabaseClone@CPSCAttractor@NParticleEngine@@QAEPAUIDrawableObj@@XZ on a local IDrawableObj model.
//   The callee 0x00AF7FB0 is NParticleEngine::CPSCAttractor::Alloc, ?Alloc@CPSCAttractor@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?CreateInternalPrimitive@@YIPAUIDrawableObj@@PAVCPSCAttractor@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#include "engine/CPSCBase.h"

namespace NParticleEngine {
// Local model (NF462, ruling r460d): CPSCAttractor has no shared header; it derives from the shared CPSCBase (RTTI: its ctor 0x00B18020 calls
// CPSCBase() 0x00AF2620 first, then stores vtable 0x012A04AC). Ego_r.pdb + FableWin.pdb name the ctor, Alloc and the ten CPSCBase overrides
// declared here; the class's own members (AttractorEnabled.. in Ego_r) are opaque. sizeof 0x30 (Alloc's operator new(0x30)).
class CPSCAttractor : public CPSCBase {
public:
    CPSCAttractor();                                    // 0x00B18020
    static CPSCBase* FABLE_FASTCALL Alloc();            // 0x00AF7FB0
    virtual CPSCBase* DatabaseClone() const;            // 0x00B18740
    virtual CCharString GetClassTypeName() const;
    virtual EPSCType GetComponentType() const;
    virtual EPSCSubType GetComponentSubType() const;
    virtual CCharString GetGUIName() const;
    virtual void InterpolateTo(const CPSCBase&,const float);
    virtual void ReadBinary(CDataInputStream&);
    virtual void WriteBinary(CDataOutputStream&) const;
    virtual void DatabaseCopy(const CPSCBase&);
    virtual void GetComponentCost(CParticleSystemCost&,const CParticleSystem&,const float,const float);
private:
    unsigned char _members[0x30 - 0xc];                 // +0x0c own members (opaque)
};
}

namespace NParticleEngine {
CPSCBase* CPSCAttractor::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
