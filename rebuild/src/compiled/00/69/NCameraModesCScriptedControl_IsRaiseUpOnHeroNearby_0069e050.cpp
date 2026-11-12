// NCameraModes::CScriptedControl::IsRaiseUpOnHeroNearby() const -- returns false
// Leaf: ?IsRaiseUpOnHeroNearby@CScriptedControl@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069E050, thiscall, `ret`.
// Identity: slot 7 of NCameraModes::CScriptedControl's vtable 0x0125C58C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptedControl or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 9 TLC-named slots of that vtable puts IsRaiseUpOnHeroNearby(void) at this slot; Ego_r/FableWin name set: ?IsRaiseUpOnHeroNearby@CScriptedControl@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptedControl {
public:
    virtual bool IsRaiseUpOnHeroNearby() const;
};
bool CScriptedControl::IsRaiseUpOnHeroNearby() const
{
    return false;
}
}
