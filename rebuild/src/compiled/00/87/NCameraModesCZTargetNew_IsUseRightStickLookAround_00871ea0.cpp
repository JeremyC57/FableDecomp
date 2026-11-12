// NCameraModes::CZTargetNew::IsUseRightStickLookAround() const -- returns false
// Leaf: ?IsUseRightStickLookAround@CZTargetNew@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00871EA0, thiscall, `ret`.
// Identity: slot 4 of NCameraModes::CZTargetNew's vtable 0x0127754C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CZTargetNew or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsUseRightStickLookAround(void) at this slot; Ego_r/FableWin name set: ?IsUseRightStickLookAround@CZTargetNew@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CZTargetNew {
public:
    virtual bool IsUseRightStickLookAround() const;
};
bool CZTargetNew::IsUseRightStickLookAround() const
{
    return false;
}
}
