// NCameraModes::CFirstPerson::IsRaiseUpOnHeroNearby() const -- returns false
// Leaf: ?IsRaiseUpOnHeroNearby@CFirstPerson@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873AD0, thiscall, `ret`.
// Identity: slot 7 of NCameraModes::CFirstPerson's vtable 0x012776AC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CFirstPerson or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsRaiseUpOnHeroNearby(void) at this slot; Ego_r/FableWin name set: ?IsRaiseUpOnHeroNearby@CFirstPerson@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CFirstPerson {
public:
    virtual bool IsRaiseUpOnHeroNearby() const;
};
bool CFirstPerson::IsRaiseUpOnHeroNearby() const
{
    return false;
}
}
