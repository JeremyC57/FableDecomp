// NCameraModes::CCameraPointModeBase::IsUseRightStickLookAround() const -- returns false
// Leaf: ?IsUseRightStickLookAround@CCameraPointModeBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E260, thiscall, `ret`.
// Identity: slot 4 of NCameraModes::CDead's vtable 0x0125C5F4 (defined here: the slot holds this body in 5 vtable(s), all of NCameraModes::CCameraPointModeBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsUseRightStickLookAround(void) at this slot; Ego_r/FableWin name set: ?IsUseRightStickLookAround@CCameraPointModeBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CCameraPointModeBase {
public:
    virtual bool IsUseRightStickLookAround() const;
};
bool CCameraPointModeBase::IsUseRightStickLookAround() const
{
    return false;
}
}
