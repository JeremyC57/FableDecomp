// NCameraModes::CStringAndCage::SetSACPosition(const C3DVector& position) -- pull the camera towards `position` by adding it as a go-to attractor of weight 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871E40, 18 bytes, `ret 4`.
// Leaf: ?SetSACPosition@CStringAndCage
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Name: vtable 0x0125C404 slot 30 (CStringAndCageBase::SetSACPosition 0x0069DDC0 is the base slot); FableWin.pdb ?SetSACPosition@CStringAndCage@NCameraModes.
// Callees: NCameraModes::CBase::AddAttractorGoTo(const C3DVector&, float) @ 0x0084E1A0 (pushes a {position, weight} node on the
//          go-to attractor list at +0x78; FableWin.pdb ?AddAttractorGoTo@CBase@NCameraModes@@IAEXABVC3DVector@@M@Z, manifest
//          "LinkedListInsertNode" is a generic label).
// Shape: the weight literal 1.0f pushed as an immediate, the reference forwarded; a plain call (the callee pops 8, we pop 4).
#include "engine/C3DVector.h"
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    unsigned char _body[0xa0];                     // +0x04..+0xa3 (retail)
    void AddAttractorGoTo(const C3DVector& position, float weight);   // 0x0084E1A0
};
class CStringAndCageBase : public CBase { public:
    virtual void SetSACPosition(const C3DVector& position);
};
struct CStringAndCage : CStringAndCageBase {
    virtual void SetSACPosition(const C3DVector& position);
};
void CStringAndCage::SetSACPosition(const C3DVector& position)
{
    AddAttractorGoTo(position, 1.0f);
}
}