// Behaviour test of CTCInventoryWeapons::ConstructWeaponDescription (005c6b26): the description is
//   "Damage <d> (<+diff>)\n" + "Augmentations <names, comma separated | None>\n"
//   + (owned items only) "Value <shop buy price when the hero is in a live shop that stocks it, else the default price>\n"
//   + (melee only) "Weight <Heavy | Light>\n" (class 2 heavy, class 1 light, other melee classes no word) + "Kills <n>\n";
// augmentations / kills come from the def's initial augmentations (fromDef) or the owned item's per-item data.
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
static long FloatToLongNear(float v)                 // the x87 fistp under round-to-nearest-even (GFFloatToLongNear)
{
    double f = std::floor((double)v), r = (double)v - f;
    long n = (long)f;
    if (r > 0.5 || (r == 0.5 && (n & 1))) ++n;
    return n;
}
static std::string L2S(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
struct CPerItemData { std::vector<long> Augmentations; long NumberOfKills; CPerItemData() : NumberOfKills(0) { } };
class CWeaponDef { public: int Class; float Damage; float ShotDamage; long Slots; std::vector<long> Initial; long DefaultPrice; };
struct CShop { bool Alive; bool Stocks; long Price; };
static bool IsMelee(int c) { return c == 1 || c == 2 || c == 4; }
static const char* AugName(long a) { return a == 1 ? "Sharp" : a == 2 ? "Silver" : "Flame"; }
static std::string Describe(const CWeaponDef& def, const CPerItemData* owned, const CShop* shop, float damage, bool fromDef)
{
    CPerItemData data;
    const CPerItemData* item;
    if (fromDef) {
        data.Augmentations.resize(def.Slots, 0);
        for (unsigned long i = 0; i < def.Initial.size() && i < data.Augmentations.size(); ++i)
            data.Augmentations[i] = def.Initial[i];
        item = &data;
    } else
        item = owned;
    float weaponDamage = IsMelee(def.Class) ? def.Damage : def.ShotDamage;
    float multiplier = 1.0f;
    for (unsigned i = 0; i < item->Augmentations.size(); ++i) if (item->Augmentations[i] > 0) multiplier += 0.5f;
    long itemDamage = FloatToLongNear(multiplier * weaponDamage * 10.0f);
    long difference = itemDamage - FloatToLongNear(damage * 10.0f);
    std::string t = "Damage " + L2S(itemDamage) + " (" + (difference > 0 ? "+" : "") + L2S(difference) + ")\n";
    t += "Augmentations ";
    const std::vector<long>& a = item->Augmentations;
    long last = -1, used = 0;
    for (long i = 0; i < (long)a.size(); ++i) if (a[i] > 0) { ++used; last = i; }
    if (used > 0) {
        for (long i = 0; i < (long)a.size(); ++i)
            if (a[i] > 0) { t += AugName(a[i]); if (i < last) t += ", "; }
    } else
        t += "None";
    t += "\n";
    if (!fromDef) {
        long value = def.DefaultPrice;
        if (shop && shop->Alive && shop->Stocks) value = shop->Price;
        t += "Value " + L2S(value) + "\n";
    }
    if (IsMelee(def.Class)) {
        t += "Weight ";
        if (def.Class == 2) t += "Heavy"; else if (def.Class == 1) t += "Light";
        t += "\n";
    }
    t += "Kills " + L2S(item->NumberOfKills) + "\n";
    return t;
}
int main()
{
    CWeaponDef sword; sword.Class = 2; sword.Damage = 2.5f; sword.ShotDamage = 9.0f; sword.Slots = 3; sword.DefaultPrice = 100;
    sword.Initial.push_back(1); sword.Initial.push_back(0); sword.Initial.push_back(3); sword.Initial.push_back(2);
    int bad = 0;
    std::string t = Describe(sword, 0, 0, 3.0f, true);    // 2.0 * 2.5 * 10 = 50 v 30
    bad |= t != "Damage 50 (+20)\nAugmentations Sharp, Flame\nWeight Heavy\nKills 0\n";
    CPerItemData owned; owned.Augmentations.push_back(0); owned.NumberOfKills = 7;
    CShop shop = { true, true, 140 };
    t = Describe(sword, &owned, &shop, 2.5f, false);
    bad |= t != "Damage 25 (0)\nAugmentations None\nValue 140\nWeight Heavy\nKills 7\n";
    shop.Alive = false;
    CWeaponDef bow = sword; bow.Class = 3; bow.ShotDamage = 1.25f;
    t = Describe(bow, &owned, &shop, 1.0f, false);       // 12.5 -> 12 (even), ranged: no weight line, dead shop: default price
    bad |= t != "Damage 12 (+2)\nAugmentations None\nValue 100\nKills 7\n";
    CPerItemData empty;
    t = Describe(bow, &empty, 0, 2.0f, false);
    bad |= t != "Damage 12 (-8)\nAugmentations None\nValue 100\nKills 0\n";
    if (bad) { std::printf("005c6b26_TEST FAIL %s\n", t.c_str()); return 1; }
    std::printf("005c6b26_TEST PASS\n");
    return 0;
}