// NCameraModes::CProjectileWeapon::GetDefaultFOV() const -- returns LockDeltaPhi
// Leaf: ?GetDefaultFOV@CProjectileWeapon@NCameraModes@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B630, thiscall, `ret`.
// Identity: slot 18 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CProjectileWeapon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetDefaultFOV(void) at this slot; Ego_r/FableWin name set: ?GetDefaultFOV@CProjectileWeapon@NCameraModes@@UBEMXZ; `ret` matches the signature.
// Layout: Ego_r.pdb CProjectileWeapon has float LockDeltaPhi at +0x104, the offset retail uses.
namespace NCameraModes {
class CProjectileWeapon {
public:
    virtual float GetDefaultFOV() const;
private:
    unsigned char _base_0x04[0x100];   // +0x04 base class and earlier members (no shared header)
    float LockDeltaPhi;   // +0x104 (Ego_r.pdb CProjectileWeapon)
};
float CProjectileWeapon::GetDefaultFOV() const
{
    return LockDeltaPhi;
}
}
