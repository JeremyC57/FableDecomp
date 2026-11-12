// NCameraModes::CSnapToPoint::OnDieLeaf() -- does nothing
// Leaf: ?OnDieLeaf@CSnapToPoint@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E390, thiscall, `ret`.
// Identity: slot 25 of NCameraModes::CSnapToPoint's vtable 0x0124DE5C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CSnapToPoint or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts OnDieLeaf(void) at this slot; Ego_r/FableWin name set: ?OnDieLeaf@CSnapToPoint@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CSnapToPoint {
public:
    virtual void OnDieLeaf();
};
void CSnapToPoint::OnDieLeaf()
{
}
}
