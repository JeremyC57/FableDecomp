// Behaviour test for CTCHeroStats::GetFavouriteWeapon (0x0057AEB4): among the weapon items of the hero's weapons inventory (type 1:
// melee only, type 2: ranged only, any other type: all) the one whose per-item count is the largest (strictly larger than 0 and
// than every earlier one; items without per-item data are skipped) is described; with no such item (or no weapons inventory, or a
// favourite def index that is not positive) the "none" label text comes back.
#include <cstdio>
#include <cstring>
struct Item { long def; bool weapon; bool melee; bool has_data; long count; };
struct Inv { const Item* items; int n; int refreshes; };
static const char* GetFavouriteWeapon(Inv* inv, long type)
{
    static char buf[32];
    if (inv) {
        ++inv->refreshes;
        long favourite = -1;
        long max_time_wielded = 0;
        for (long item = 0; (unsigned long)item < (unsigned long)inv->n; ++item) {
            const Item& it = inv->items[item];
            if (it.weapon) {
                if (type == 1) {
                    if (!it.melee) continue;
                }
                else if (type == 2) {
                    if (it.melee) continue;
                }
                if (it.has_data) {
                    long time_wielded_weapon = it.count;
                    if (time_wielded_weapon > max_time_wielded) {
                        max_time_wielded = time_wielded_weapon;
                        favourite = it.def;
                    }
                }
            }
        }
        if (favourite > 0) { std::sprintf(buf, "desc%ld", favourite); return buf; }
    }
    return "TEXT_GUI_STATS_LABEL_NONE";
}
static int fail(int n) { std::printf("0057aeb4_TEST FAIL %d\n", n); return 1; }
int main()
{
    const Item items[] = {
        { 10, true,  true,  true,  5 },     // melee sword, 5
        { 11, false, true,  true,  50 },    // not a weapon (ignored even with the biggest count)
        { 12, true,  false, true,  7 },     // ranged bow, 7
        { 13, true,  true,  false, 99 },    // melee, no per-item data: skipped
        { 14, true,  true,  true,  5 },     // melee tie with 10: the first stays
        { 15, true,  false, true,  3 },     // ranged, 3
    };
    Inv inv = { items, 6, 0 };
    if (std::strcmp(GetFavouriteWeapon(&inv, 0), "desc12") != 0) return fail(1);    // any: the bow (7)
    if (std::strcmp(GetFavouriteWeapon(&inv, 1), "desc10") != 0) return fail(2);    // melee: sword 10 (tie keeps first)
    if (std::strcmp(GetFavouriteWeapon(&inv, 2), "desc12") != 0) return fail(3);    // ranged: bow
    const Item zero[] = { { 20, true, true, true, 0 } };                              // count 0 never beats 0
    Inv z = { zero, 1, 0 };
    if (std::strcmp(GetFavouriteWeapon(&z, 0), "TEXT_GUI_STATS_LABEL_NONE") != 0) return fail(4);
    const Item neg[] = { { -4, true, true, true, 9 } };                               // favourite index not positive
    Inv g = { neg, 1, 0 };
    if (std::strcmp(GetFavouriteWeapon(&g, 0), "TEXT_GUI_STATS_LABEL_NONE") != 0) return fail(5);
    if (std::strcmp(GetFavouriteWeapon(0, 0), "TEXT_GUI_STATS_LABEL_NONE") != 0) return fail(6);
    const Item only_ranged[] = { { 30, true, false, true, 4 } };
    Inv r = { only_ranged, 1, 0 };
    if (std::strcmp(GetFavouriteWeapon(&r, 1), "TEXT_GUI_STATS_LABEL_NONE") != 0) return fail(7);   // melee filter drops it
    const Item only_melee[] = { { 31, true, true, true, 4 } };
    Inv m = { only_melee, 1, 0 };
    if (std::strcmp(GetFavouriteWeapon(&m, 2), "TEXT_GUI_STATS_LABEL_NONE") != 0) return fail(8);   // ranged filter drops it
    if (inv.refreshes != 3) return fail(9);
    const Item last_best[] = { { 40, true, true, true, 2 }, { 41, true, false, true, 6 } };
    Inv lb = { last_best, 2, 0 };
    if (std::strcmp(GetFavouriteWeapon(&lb, 0), "desc41") != 0) return fail(10);  // the loop reaches the last item (`item < size`)
    std::printf("0057aeb4_TEST PASS\n");
    return 0;
}