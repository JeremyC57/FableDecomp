// CTCWeapon::IsProjectileWeaponClass(EWeaponClass weapon_class) -- true for the projectile weapon class (3)
// Leaf: ?IsProjectileWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z
// Retail: Fable.exe @ 0x005DA530, 9 bytes, `ret` with the class in ecx (public static __fastcall).
// Identity (proven): Ego_r.pdb has exactly two static weapon-class predicates, ?IsMeleeWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z
// (landed at 0x005DA510, classes 1 and 2, right before this body in the CTCWeapon TU) and
// ?IsProjectileWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z; this one tests the remaining class 3. Caller 0x005CBC99 (the
// inventory-weapons TU) tests a weapon's class word +0x2C with it.
// Label correction: the catalog's "CComponent::HasState" (thiscall, two parameters) is wrong: no `this`, one register
// argument, and NUISystem::CComponent::HasState is a virtual `UAE_NK` member.
// Callees: none.
// Shape: `xor eax,eax ; cmp ecx,3 ; sete al` -- the bool result widened in eax before the compare.
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_MELEE_1 = 1, WEAPON_CLASS_MELEE_2 = 2, WEAPON_CLASS_PROJECTILE = 3, WEAPON_CLASS_FORCE_DWORD = 0x7fffffff };
class CTCWeapon {
public:
    static bool __fastcall IsProjectileWeaponClass(EWeaponClass weapon_class);
};

bool __fastcall CTCWeapon::IsProjectileWeaponClass(EWeaponClass weapon_class)
{
    return weapon_class == WEAPON_CLASS_PROJECTILE;
}