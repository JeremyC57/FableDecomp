// NCameraModes::CProjectileWeapon::IsShortenOnOcclusion() const -- returns true
// Leaf: ?IsShortenOnOcclusion@CProjectileWeapon@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B620, thiscall, `ret`.
// Identity: slot 6 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CProjectileWeapon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CProjectileWeapon@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CProjectileWeapon {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool CProjectileWeapon::IsShortenOnOcclusion() const
{
    return true;
}
}
