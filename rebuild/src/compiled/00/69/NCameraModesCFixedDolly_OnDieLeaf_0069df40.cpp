// NCameraModes::CFixedDolly::OnDieLeaf() -- does nothing
// Leaf: ?OnDieLeaf@CFixedDolly@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DF40, thiscall, `ret`.
// Identity: slot 25 of NCameraModes::CFixedDolly's vtable 0x0125C48C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CFixedDolly or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts OnDieLeaf(void) at this slot; Ego_r/FableWin name set: ?OnDieLeaf@CFixedDolly@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CFixedDolly {
public:
    virtual void OnDieLeaf();
};
void CFixedDolly::OnDieLeaf()
{
}
}
