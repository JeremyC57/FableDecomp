// CTCDCameraPoint::Construct(const C3DVector& position) -- construct the thing's physics from a CTCParamsPhysicsBase holding the position and the default (up/forward) right-handed set
// Leaf: ?Construct@CTCDCameraPoint@@QAEXABVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778D60, length 127 bytes (`ret 4`). A byte twin sits at 0x0080C250.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 and its dtor @ 0x0099A300 (the manifest's "CBase::CBase"
// labels), CTCPhysicsBase::ConstructFromParams(CTCParamsBase*) (slot 3, +0xC). Vtable 0x0125C818 = CTCParamsPhysicsBase.
// Layout: Ego_r.pdb CTCParamsPhysicsBase (0x28): Position +4, Rhs +0x10 (CRightHandedSet: two C3DVectors, here (0,0,1) and
// (0,1,0)). The owner thing is the def component's +4 word and its physics component sits at the retail CThing +0x60.
// The position member is copied by C3DVector's user copy ctor (the implicit copy picks other registers).
// Shape: CBaseClassNonCopyable is the polymorphic root (its ctor stores a vtable), so the vptr is shared at +0; the params object is a stack local built by an inline ctor over the out-of-line non-copyable base (position copied
// as three dwords, then the vtable store, then the set's six constant floats); its implicit dtor calls the base dtor only.
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) {}
};
struct CBaseClassNonCopyable {                  // polymorphic: its out-of-line ctor/dtor store vtables 0x0129A7C4 / 0x01231710
protected:
    virtual ~CBaseClassNonCopyable();   // slot 0 (retail scalar deleting dtor; was placeholder v00); 0x0099A300
public:
    CBaseClassNonCopyable();                    // 0x0099A2F0
};
class CTCParamsBase : public CBaseClassNonCopyable { public:
};
class CTCParamsPhysicsBase : public CTCParamsBase { public:   // vtable 0x0125C818, 0x28 bytes
    C3DVector Position;                         // +0x04
    CRightHandedSet Rhs;                        // +0x10
    CTCParamsPhysicsBase(const C3DVector& position) : Position(position) {}
    virtual void v00();
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(CTCParamsBase* params);    // slot 3 (+0xc)
};
#include "engine/CThing.h"
class CTCDCameraPoint { public:
    virtual void v00();
    CThing* Owner;                              // +0x04
    void Construct(const C3DVector& position);
};

void CTCDCameraPoint::Construct(const C3DVector& position)
{
    CTCPhysicsBase* physics = Owner->GetPhysicsTC();
    CTCParamsPhysicsBase params(position);
    physics->ConstructFromParams(&params);
}