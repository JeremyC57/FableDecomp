// Behaviour test of CTCInventoryWeapons::ConstructWeaponDescriptionContainer (005c90fc): three text components are added to the
// container, in order:
//   1. "Damage <d> (<+diff>)\n" + "Augmentations <names, comma separated | None>\n" + "Value <shop buy price | default price>"
//   2. (melee only) "Weight <Heavy | Light>" (class 2 heavy, class 1 light; other classes the bare label), empty for ranged weapons;
//      for a heavy weapon (class 2) with a hero experience TC and the inventory-experience TC present, every heavy-weapon stat
//      threshold above the hero's stat level zeroes the line's blue and green (text shown red)
//   3. "Kills <n>\n"
// Each line is created from the PC or the console template (live GUI mode) and the text buffer is reset to the null string after each.
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
static long FloatToLongNear(float v)                 // the x87 fistp under round-to-nearest-even (GFFloatToLongNear)
{
    double f = std::floor((double)v), r = (double)v - f;
    long n = (long)f;
    if (r > 0.5 || (r == 0.5 && (n & 1))) ++n;
    return n;
}
static std::string L2S(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
struct CPerItemData { std::vector<long> Augmentations; long NumberOfKills; CPerItemData() : NumberOfKills(0) { } };
class CWeaponDef { public: int Class; float Damage; float ShotDamage; long DefaultPrice; };
struct CShop { bool Alive; bool Stocks; long Price; };
struct CText { std::string Template, Value; unsigned char B, G, R; CText() : B(200), G(200), R(200) { } };
struct CHero { bool HasExperience, HasInventoryExperience; std::map<int, long> Levels; std::map<int, long> HeavyThresholds; };
static bool IsMelee(int c) { return c == 1 || c == 2 || c == 4; }
static const char* AugName(long a) { return a == 1 ? "Sharp" : a == 2 ? "Silver" : "Flame"; }
static void AddLine(std::vector<CText>& container, bool live, std::string& text)
{
    CText line;
    line.Template = live ? "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" : "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
    line.Value = text;
    container.push_back(line);
    text = "";                                         // CWideString::NullString
}
static void Construct(std::vector<CText>& container, const CWeaponDef& def, const CPerItemData& item, const CShop* shop,
                      const CHero& hero, float damage, bool live)
{
    std::string t;
    float weaponDamage = IsMelee(def.Class) ? def.Damage : def.ShotDamage;
    float multiplier = 1.0f;
    for (unsigned i = 0; i < item.Augmentations.size(); ++i) if (item.Augmentations[i] > 0) multiplier += 0.5f;
    long itemDamage = FloatToLongNear(multiplier * weaponDamage * 10.0f);
    long difference = itemDamage - FloatToLongNear(damage * 10.0f);
    t += "Damage " + L2S(itemDamage) + " (" + (difference > 0 ? "+" : "") + L2S(difference) + ")\n";
    t += "Augmentations ";
    const std::vector<long>& a = item.Augmentations;
    long last = -1, used = 0;
    for (long i = 0; i < (long)a.size(); ++i) if (a[i] > 0) { ++used; last = i; }
    if (used > 0) {
        for (long i = 0; i < (long)a.size(); ++i)
            if (a[i] > 0) { t += AugName(a[i]); if (i < last) t += ", "; }
    } else
        t += "None";
    t += "\n";
    long value = def.DefaultPrice;
    if (shop && shop->Alive && shop->Stocks) value = shop->Price;
    t += "Value " + L2S(value);
    AddLine(container, live, t);
    if (IsMelee(def.Class)) {
        t += "Weight ";
        if (def.Class == 2) t += "Heavy"; else if (def.Class == 1) t += "Light";
    }
    AddLine(container, live, t);
    CText& weight = container.back();
    if (def.Class == 2 && hero.HasExperience && hero.HasInventoryExperience) {
        for (std::map<int, long>::const_iterator it = hero.HeavyThresholds.begin(); it != hero.HeavyThresholds.end(); ++it) {
            std::map<int, long>::const_iterator level = hero.Levels.find(it->first);
            if ((level == hero.Levels.end() ? 0 : level->second) < it->second) {
                weight.B = (unsigned char)(weight.B * 0.0f);
                weight.G = (unsigned char)(weight.G * 0.0f);
            }
        }
    }
    t += "Kills " + L2S(item.NumberOfKills) + "\n";
    AddLine(container, live, t);
}
int main()
{
    int bad = 0;
    CWeaponDef axe; axe.Class = 2; axe.Damage = 2.5f; axe.ShotDamage = 9.0f; axe.DefaultPrice = 100;
    CPerItemData owned; owned.Augmentations.push_back(1); owned.Augmentations.push_back(0); owned.Augmentations.push_back(3); owned.NumberOfKills = 7;
    CShop shop = { true, true, 140 };
    CHero hero; hero.HasExperience = true; hero.HasInventoryExperience = true;
    hero.Levels[0] = 3; hero.HeavyThresholds[0] = 2;                                  // strong enough: stays white
    std::vector<CText> c;
    Construct(c, axe, owned, &shop, hero, 3.0f, true);                                // 2.0 * 2.5 * 10 = 50 v 30
    bad |= c.size() != 3;
    bad |= c[0].Value != "Damage 50 (+20)\nAugmentations Sharp, Flame\nValue 140";
    bad |= c[1].Value != "Weight Heavy" || c[1].B != 200 || c[1].G != 200 || c[1].R != 200;
    bad |= c[2].Value != "Kills 7\n" || c[2].Template != "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
    hero.HeavyThresholds[1] = 5;                                                      // physique too low: red weight line
    c.clear();
    Construct(c, axe, owned, 0, hero, 5.0f, false);
    bad |= c[1].B != 0 || c[1].G != 0 || c[1].R != 200 || c[0].Template != "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
    bad |= c[0].Value != "Damage 50 (0)\nAugmentations Sharp, Flame\nValue 100";
    hero.HasInventoryExperience = false;                                              // no experience screen: no colouring
    c.clear();
    Construct(c, axe, owned, 0, hero, 5.0f, false);
    bad |= c[1].B != 200;
    CWeaponDef bow = axe; bow.Class = 3; bow.ShotDamage = 1.5f;
    CPerItemData empty;
    c.clear();
    Construct(c, bow, empty, &shop, hero, 1.0f, true);                                // ranged: an empty weight line, no colouring
    bad |= c[0].Value != "Damage 15 (+5)\nAugmentations None\nValue 140" || c[1].Value != "" || c[2].Value != "Kills 0\n";
    if (bad) { std::printf("005c90fc_TEST FAIL\n"); return 1; }
    std::printf("005c90fc_TEST PASS\n");
    return 0;
}