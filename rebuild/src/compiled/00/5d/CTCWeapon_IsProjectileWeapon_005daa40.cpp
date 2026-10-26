// CTCWeapon::IsProjectileWeapon() const -- the weapon definition's Class is the projectile class (3)
// Leaf: ?IsProjectileWeapon@CTCWeapon@@QBE_NXZ
// Retail: Fable.exe @ 0x005DAA40, 38 bytes; Ego_r.pdb same symbol. Body: a CDefPointer copy of WeaponDef (+0x0c), read
// Class (+0x2c), release the copy, then `cmp esi,3 ; sete al`. Ego_r.pdb also has the static
// ?IsProjectileWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z, modelled inline here; the enumerator name is behavioural.
// Layout: Ego_r.pdb CTCWeapon WeaponDef +0x0c; CWeaponDef Class +0x2c (retail = PDB here, as landed IsMeleeWeapon
// 0x005DAA00).
// Shape: /O2; the class comes from the inline GetWeaponClass() (copy, read, release) so the compare follows the release
// (`xor eax,eax ; cmp esi,3 ; sete al`); comparing inside the def-pointer scope computes it before the release into bl.
enum EWeaponType { WEAPON_TYPE_NONE };
enum EWeaponClass { WEAPON_CLASS_NONE, WEAPON_CLASS_1, WEAPON_CLASS_2, WEAPON_CLASS_PROJECTILE };
class CDefPointeeBase { public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CWeaponDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x1c];                          // +0x08
    unsigned char _pad08b[0x04];                         // +0x24
    EWeaponType   Type;                                  // +0x28
    EWeaponClass  Class;                                 // +0x2c
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    static bool IsProjectileWeaponClass(EWeaponClass weapon_class) { return weapon_class == WEAPON_CLASS_PROJECTILE; }
    EWeaponClass GetWeaponClass() const { CDefPointer<CWeaponDef> def(WeaponDef); return def->Class; }
    bool IsProjectileWeapon() const;
};
bool CTCWeapon::IsProjectileWeapon() const
{
    return IsProjectileWeaponClass(GetWeaponClass());
}