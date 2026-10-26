// CTCWeapon::GetWeaponType() const -- the weapon definition's Type
// Leaf: GetWeaponType@CTCWeapon
// Retail: Fable.exe @ 0x005DA7D0, 32 bytes. Catalog label NInventory::CCategory::GetCategoryIdentifier is wrong (ICF
// propagation): callers (CDrawAbilityButtons::Draw 0x006520F1/0x00652122/0x00652137, CalculateWeaponState 0x0072BA07) fetch
// TC 0x48 (CTCWeapon) first. Body: a CDefPointer copy of WeaponDef (+0x0c; add-ref), read +0x28, release the copy.
// Ego_r.pdb CWeaponDef has Type +0x28 / Class +0x2c, and retail keeps those offsets: landed IsMeleeWeapon (0x005DAA00)
// compares +0x2c against the melee classes and landed GetAnimationSpeedValue (0x005DA850) reads AnimationSpeedVal at
// the PDB +0x40. So +0x28 is Type: Ego_r.pdb ?GetWeaponType@CTCWeapon@@QBE?AW4EWeaponType@@XZ (next to GetWeaponClass).
// Layout: Ego_r.pdb CTCWeapon WeaponDef +0x0c.
// Shape: /O2 (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`).
enum EWeaponType { WEAPON_TYPE_NONE };
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
};
class CTCWeapon {
public:
    unsigned char           _pad00[0xc];                 // +0x00 CTCBase
    CDefPointer<CWeaponDef> WeaponDef;                   // +0x0c
    EWeaponType GetWeaponType() const;
};
EWeaponType CTCWeapon::GetWeaponType() const
{
    CDefPointer<CWeaponDef> def(WeaponDef);
    return def->Type;
}