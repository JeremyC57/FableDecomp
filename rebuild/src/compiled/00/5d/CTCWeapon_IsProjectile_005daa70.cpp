// CTCWeapon::IsProjectile() const -- the weapon definition's Type is one of the two projectile types (6 or 5)
// Leaf: ?IsProjectile@CTCWeapon@@QBE_NXZ
// Retail: Fable.exe @ 0x005DAA70, 46 bytes. Catalog label CTCWeapon::IsMeleeWeapon is wrong (that is 0x005DAA00, landed,
// which tests Class +0x2c against 1/2): this one reads Type (+0x28). Named by position: Ego_r.pdb orders IsMeleeWeapon
// (0x0063A840), IsProjectileWeapon (0x0063A870), IsProjectile (0x0063A8A0), GetDamage, ..., IsUsableBy, and retail has
// IsMeleeWeapon 0x005DAA00, IsProjectileWeapon 0x005DAA40, this 0x005DAA70, ..., IsUsableBy 0x005DAB10; Ego_r.pdb's
// ?IsProjectile@CTCWeapon@@QBE_NXZ is the only bool, argument-free CTCWeapon member left for the slot. Body: a CDefPointer
// copy of WeaponDef (+0x0c), read Type, release the copy, then `type == 6 || type == 5` (enumerator names behavioural).
// Layout: as 0x005DA7D0 / 0x005DAA40 (CWeaponDef Type +0x28, retail = Ego_r.pdb up to +0x50).
// Shape: /O2; the type comes from the inline GetWeaponType() (copy, read, release), so both compares follow the release;
// `if (..) return true; return false;` gives the byte-register results (`xor al,al` / `mov al,1`), a bare `||` return
// gives `xor eax,eax` / `mov eax,1`.
enum EWeaponType { WEAPON_TYPE_NONE, WEAPON_TYPE_PROJECTILE_A = 5, WEAPON_TYPE_PROJECTILE_B = 6 };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefPointeeBase {
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad08b[0x04];                         // +0x24
    EWeaponType   Type;                                  // +0x28
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    EWeaponType GetWeaponType() const { CDefPointer<CWeaponDef> def(WeaponDef); return def->Type; }
    bool IsProjectile() const;
};
bool CTCWeapon::IsProjectile() const
{
    EWeaponType type = GetWeaponType();
    if (type == WEAPON_TYPE_PROJECTILE_B || type == WEAPON_TYPE_PROJECTILE_A)
        return true;
    return false;
}