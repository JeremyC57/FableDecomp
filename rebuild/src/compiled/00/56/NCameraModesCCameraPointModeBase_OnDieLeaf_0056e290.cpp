// NCameraModes::CCameraPointModeBase::OnDieLeaf() -- does nothing
// Leaf: ?OnDieLeaf@CCameraPointModeBase@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E290, thiscall, `ret`.
// Identity: slot 25 of NCameraModes::CDead's vtable 0x0125C5F4 (defined here: the slot holds this body in 2 vtable(s), all of NCameraModes::CCameraPointModeBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts OnDieLeaf(void) at this slot; Ego_r/FableWin name set: ?OnDieLeaf@CCameraPointModeBase@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CCameraPointModeBase {
public:
    virtual void OnDieLeaf();
};
void CCameraPointModeBase::OnDieLeaf()
{
}
}
