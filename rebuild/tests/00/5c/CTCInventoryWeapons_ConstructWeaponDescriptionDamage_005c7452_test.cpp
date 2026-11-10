// Behaviour test of CTCInventoryWeapons::ConstructWeaponDescriptionDamage (0x005C7452): damage line = TEXT_GUI_MENU_DAMAGE + space
// + round(multiplier(initial augmentations) * weapon damage * 10) + space + "(" + ["+" when better] + difference to the given damage
// in tenths + ")"; melee weapons use their def's Damage, ranged ones their shot's HitThingDamage.
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
static long FloatToLongNear(float v)                 // the x87 fistp under round-to-nearest-even (GFFloatToLongNear)
{
    double f = std::floor((double)v), r = (double)v - f;
    long n = (long)f;
    if (r > 0.5 || (r == 0.5 && (n & 1))) ++n;
    return n;
}
static std::string IntToString(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
class CWeaponDef { public: int Class; float Damage; float ShotHitThingDamage; std::vector<long> InitialAugmentations; long NumberOfSlots; };
static bool IsMeleeWeaponClass(int c) { return c == 1 || c == 2; }
static float GetDamageMultiplierFromAugmentationSet(const std::vector<long>& a)
{
    float m = 1.0f;
    for (unsigned i = 0; i < a.size(); ++i) if (a[i] > 0) m += 0.5f;
    return m;
}
static void ConstructWeaponDescriptionDamage(std::string& text, const CWeaponDef& def, float damage)
{
    std::vector<long> augmentations(def.NumberOfSlots, 0);
    for (unsigned long i = 0; i < def.InitialAugmentations.size(); ++i) {
        if (i >= augmentations.size())
            break;
        augmentations[i] = def.InitialAugmentations[i];
    }
    float weaponDamage;
    if (IsMeleeWeaponClass(def.Class))
        weaponDamage = def.Damage;
    else
        weaponDamage = def.ShotHitThingDamage;
    long itemDamage = FloatToLongNear(GetDamageMultiplierFromAugmentationSet(augmentations) * weaponDamage * 10.0f);
    long difference = itemDamage - FloatToLongNear(damage * 10.0f);
    text += std::string("Damage") + " ";
    text += IntToString(itemDamage) + " ";
    text += "(";
    if (difference > 0)
        text += "+";
    text += IntToString(difference) + ")";
}
int main()
{
    CWeaponDef sword; sword.Class = 1; sword.Damage = 2.5f; sword.ShotHitThingDamage = 99.0f; sword.NumberOfSlots = 2;
    sword.InitialAugmentations.push_back(7); sword.InitialAugmentations.push_back(0); sword.InitialAugmentations.push_back(9);
    std::string t;
    ConstructWeaponDescriptionDamage(t, sword, 3.0f);        // 1.5 * 2.5 * 10 = 37.5 -> 38 (even); 30 -> +8
    int bad = t != "Damage 38 (+8)";
    CWeaponDef bow; bow.Class = 3; bow.Damage = 50.0f; bow.ShotHitThingDamage = 1.0f; bow.NumberOfSlots = 0;
    t = "";
    ConstructWeaponDescriptionDamage(t, bow, 1.25f);         // 10 v 12.5 -> 12 (even): -2
    bad |= t != "Damage 10 (-2)";
    t = "";
    ConstructWeaponDescriptionDamage(t, bow, 1.0f);
    bad |= t != "Damage 10 (0)";
    if (bad) { std::printf("005c7452_TEST FAIL %s\n", t.c_str()); return 1; }
    std::printf("005c7452_TEST PASS\n");
    return 0;
}