// NCameraModes::CFixedDolly::UpdateDesiredPos(C3DVector& desiredPos) -- the desired camera position is the observed thing's position plus the dolly offset
// Leaf: ?UpdateDesiredPos@CFixedDolly@NCameraModes@@UAEXAAVC3DVector@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x0087B690, 87 bytes, thiscall `ret 4`.
// Identity (proven): FableWin.pdb ?UpdateDesiredPos@CFixedDolly@NCameraModes@@UAEXAAVC3DVector@@@Z; the body sits in the
//   CFixedDolly vtable 0x0125C4F8. Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0
//   (?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ; labelled _Cons_val) and CThing::GetPos() const
//   0x004C73D0 (PDB ?GetPos@CThing@@QBEABVC3DVector@@XZ; the landed models spell it GetPos).
// Layout: DollyVector (Ego_r.pdb CFixedDolly +0xE4) is +0xC0 in retail (CBase's CPhysicsState members are smaller).
// Shape: `pos + DollyVector` (VC7.1 commutes the fadd: retail loads the dolly offset first) through the inline C3DVector::operator+ (?? H C3DVector QBE ?BV0@ABV0@@Z, const return) into
//   a 12-byte frame temporary, then the implicit copy-assignment into desiredPos.
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CThingGameObject : public CThing { };
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    const CThingGameObject* GetObservedThing() const;
};
class CFixedDolly : public CBase {
public:
    unsigned char _pad04[0xc0 - 0x04];
    C3DVector DollyVector;                               // +0xc0
    virtual void UpdateDesiredPos(C3DVector& desiredPos);
};
}
void NCameraModes::CFixedDolly::UpdateDesiredPos(C3DVector& desiredPos)
{
    desiredPos = GetObservedThing()->GetPos() + DollyVector;
}