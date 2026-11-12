// NCameraModes::COrbitGame::IsFOVChange() const -- returns false
// Leaf: ?IsFOVChange@COrbitGame@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E180, thiscall, `ret`.
// Identity: slot 10 of NCameraModes::COrbitGame's vtable 0x0125C6DC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::COrbitGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsFOVChange(void) at this slot; Ego_r/FableWin name set: ?IsFOVChange@COrbitGame@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class COrbitGame {
public:
    virtual bool IsFOVChange() const;
};
bool COrbitGame::IsFOVChange() const
{
    return false;
}
}
