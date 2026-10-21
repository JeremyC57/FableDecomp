// behaviour test for 0x005DA510 CTCWeapon::IsMeleeWeaponClass
#include <stdio.h>
enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_MELEE_1 = 1, WEAPON_CLASS_MELEE_2 = 2, WEAPON_CLASS_FORCE_DWORD = 0x7fffffff };
class CTCWeapon {
public:
    static bool __fastcall IsMeleeWeaponClass(EWeaponClass weapon_class);
};

bool __fastcall CTCWeapon::IsMeleeWeaponClass(EWeaponClass weapon_class)
{
    return weapon_class == WEAPON_CLASS_MELEE_1 || weapon_class == WEAPON_CLASS_MELEE_2;
}
int main()
{
    int ok = CTCWeapon::IsMeleeWeaponClass((EWeaponClass)1) && CTCWeapon::IsMeleeWeaponClass((EWeaponClass)2)
          && !CTCWeapon::IsMeleeWeaponClass((EWeaponClass)0) && !CTCWeapon::IsMeleeWeaponClass((EWeaponClass)3)
          && !CTCWeapon::IsMeleeWeaponClass((EWeaponClass)-1);
    printf(ok ? "005da510_TEST PASS\n" : "005da510_TEST FAIL\n");
    return ok ? 0 : 1;
}