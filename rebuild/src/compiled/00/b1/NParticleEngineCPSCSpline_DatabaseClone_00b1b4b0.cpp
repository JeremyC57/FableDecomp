// NParticleEngine::CPSCSpline::DatabaseClone() const -- allocate a fresh component of this class and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCSpline@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/b1/CThingTrackNode_DrawIsDrawable_00b1b4b0.cpp: vtable slot 2 of NParticleEngine::CPSCSpline; Ego_r.pdb lists Alloc / DatabaseClone / DatabaseCopy on every particle component class and the body is exactly alloc-then-copy-from-this; the clone target is the slot-18 virtual
// Retail bytes: push esi ; push edi ; mov edi,ecx ; call Alloc 0x00AF8090 ; mov esi,eax ; mov eax,[esi] ; push edi ; mov ecx,esi ; call [eax+48h] ; pop edi ; mov eax,esi ; pop esi ; ret
// Naming (NF462, ruling r460d: caller census of the factory self respell, retail call at 0x00B1B4B4): self is the PDB virtual ?DatabaseClone@CPSCSpline@NParticleEngine@@UBEPAVCPSCBase@2@XZ (Ego_r + FableWin; its only retail
//   reference is vtable 0x012A0934 slot 2, no call site), was ?DatabaseClone@CPSCSpline@NParticleEngine@@QAEPAUIDrawableObj@@XZ on a local IDrawableObj model.
//   The callee 0x00AF8090 is NParticleEngine::CPSCSpline::Alloc, ?Alloc@CPSCSpline@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r + FableWin; RegisterPSCs
//   0x00AF8170 binds it), was the stand-in ?GetDrawableObject_00af8090@@YIPAUIDrawableObj@@PAVCPSCSpline@NParticleEngine@@@Z. The copy is CPSCBase::DatabaseCopy
//   (pure, slot 18 = +0x48) on the shared engine/CPSCBase.h. Same bytes.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSPLINEBASE_NO_CONTAINERS
#include "engine/CPSCSplineBase.h"

namespace NParticleEngine {
// Local model (NF462, ruling r460d): CPSCSpline has no shared header; it derives from the shared CPSCSplineBase (its ctor 0x00B1B350 calls
// CPSCSplineBase() 0x00B21530 first, then stores vtable 0x012A0934), on that header's containers-free view (r450a / r451a). Ego_r.pdb + FableWin.pdb name the ctor, Alloc and the ten CPSCBase overrides
// declared here; the class's own members are opaque. sizeof 0x40 (Alloc's operator new(0x40)).
class CPSCSpline : public CPSCSplineBase {
public:
    CPSCSpline();                                       // 0x00B1B350
    static CPSCBase* FABLE_FASTCALL Alloc();            // 0x00AF8090
    virtual CPSCBase* DatabaseClone() const;            // 0x00B1B4B0
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
    unsigned char _members[0x40 - 0x34];                // +0x34 own members (opaque)
};
}

namespace NParticleEngine {
CPSCBase* CPSCSpline::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
