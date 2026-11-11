// NCameraModes::CCameraPointModeBase::IsTrackObservedThing() const -- returns true
// Leaf: ?IsTrackObservedThing@CCameraPointModeBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056E2A0, thiscall, `ret`.
// Identity: slot 28 of NCameraModes::CDead's vtable 0x0125C5F4 (defined here: the slot holds this body in 4 vtable(s), all of NCameraModes::CCameraPointModeBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsTrackObservedThing(void) at this slot; Ego_r/FableWin name set: ?IsTrackObservedThing@CCameraPointModeBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CCameraPointModeBase {
public:
    virtual bool IsTrackObservedThing() const;
};
bool CCameraPointModeBase::IsTrackObservedThing() const
{
    return true;
}
}
