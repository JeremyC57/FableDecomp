// CTCWeapon::IsMeleeWeaponClass(EWeaponClass) -- true for the two melee weapon classes (1 and 2)
// Leaf: IsMeleeWeaponClass@CTCWeapon
// Retail: Fable.exe @ 0x005DA510, 19 bytes, `ret` with the class in ecx: Ego_r.pdb ?IsMeleeWeaponClass@CTCWeapon@@SI_NW4EWeaponClass@@@Z
// (public static __fastcall). Callers: CTCHeroStats::GetFavouriteWeapon 0x0057AEB4, CTCInventoryWeapons
// FinalConstructWeaponItemEntriesPC 0x005C47D5, NUISystem::CManager 0x0041E6D3, ... Enumerator names are ours.
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_MELEE_1 = 1, WEAPON_CLASS_MELEE_2 = 2, WEAPON_CLASS_FORCE_DWORD = 0x7fffffff };
class CTCWeapon {
public:
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass weapon_class);
};

bool __fastcall CTCWeapon::IsMeleeWeaponClass(EWeaponClass weapon_class)
{
    return weapon_class == WEAPON_CLASS_MELEE_1 || weapon_class == WEAPON_CLASS_MELEE_2;
}