// NCameraModes::CCameraPointModeBase::OnInitLeaf() -- does nothing
// Leaf: ?OnInitLeaf@CCameraPointModeBase@NCameraModes@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E280, thiscall, `ret`.
// Identity: slot 24 of NCameraModes::CDead's vtable 0x0125C5F4 (defined here: the slot holds this body in 2 vtable(s), all of NCameraModes::CCameraPointModeBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts OnInitLeaf(void) at this slot; Ego_r/FableWin name set: ?OnInitLeaf@CCameraPointModeBase@NCameraModes@@UAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CCameraPointModeBase {
public:
    virtual void OnInitLeaf();
};
void CCameraPointModeBase::OnInitLeaf()
{
}
}
