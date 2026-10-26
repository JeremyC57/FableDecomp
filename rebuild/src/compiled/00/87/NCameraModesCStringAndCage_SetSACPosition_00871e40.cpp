// NCameraModes::CStringAndCage::SetSACPosition(const C3DVector& position) -- pull the camera towards `position` by adding it as a go-to attractor of weight 1
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871E40, 18 bytes, `ret 4`.
// Leaf: ?SetSACPosition@CStringAndCage
// Name: vtable 0x0125C404 slot 30 (CStringAndCageBase::SetSACPosition 0x0069DDC0 is the base slot); FableWin.pdb ?SetSACPosition@CStringAndCage@NCameraModes.
// Callees: NCameraModes::CBase::AddAttractorGoTo(const C3DVector&, float) @ 0x0084E1A0 (pushes a {position, weight} node on the
//          go-to attractor list at +0x78; FableWin.pdb ?AddAttractorGoTo@CBase@NCameraModes@@IAEXABVC3DVector@@M@Z, manifest
//          "LinkedListInsertNode" is a generic label).
// Shape: the weight literal 1.0f pushed as an immediate, the reference forwarded; a plain call (the callee pops 8, we pop 4).
class C3DVector { public:
    float X, Y, Z;
};
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