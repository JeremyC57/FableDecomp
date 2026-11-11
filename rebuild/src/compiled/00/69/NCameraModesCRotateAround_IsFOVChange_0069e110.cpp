// NCameraModes::CRotateAround::IsFOVChange() const -- returns false
// Leaf: ?IsFOVChange@CRotateAround@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E110, thiscall, `ret`.
// Identity: slot 10 of NCameraModes::CRotateAround's vtable 0x0125C674 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CRotateAround or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts IsFOVChange(void) at this slot; Ego_r/FableWin name set: ?IsFOVChange@CRotateAround@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CRotateAround {
public:
    virtual bool IsFOVChange() const;
};
bool CRotateAround::IsFOVChange() const
{
    return false;
}
}
