// NCameraModes::CProjectileWeapon::GetModeType() const -- returns (EMode)0xf
// Leaf: ?GetModeType@CProjectileWeapon@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B5F0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CProjectileWeapon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CProjectileWeapon@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
namespace NCameraModes { enum EMode { }; }
namespace NCameraModes {
class CProjectileWeapon {
public:
    virtual NCameraModes::EMode GetModeType() const;
};
NCameraModes::EMode CProjectileWeapon::GetModeType() const
{
    return (EMode)0xf;
}
}
