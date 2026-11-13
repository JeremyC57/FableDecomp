// Behaviour test for 0x00600B60 CTCInventoryMap::ConstructRegionDescriptionBuyableBuildings: with compute set it walks every
// TCI_BUYABLE_HOUSE building of the region's maps; a house the player owns counts as an owned shop (when the building also has TCI_SHOP)
// or an owned house, otherwise a house that IsForSale counts as a shop / house for sale; the four counts go into the CUIRegionInfo
// (ShopsOwned, HousesOwned, ShopsForSale, HousesForSale). Without compute it reads them back. It then adds one text line per non-zero
// count in the order houses owned, shops owned, houses for sale, shops for sale (plural symbol + count when more than one, the PC or
// UI template by `pc`; owned lines set colour B=0, for-sale lines B=G=0x80).
#include <cstdio>
#include <cstring>

struct Building { int map; bool buyable; bool shop; bool owned; bool for_sale; };
struct RegionInfo { long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns; };
struct Line { const char* symbol; long count; const char* templ; int b, g; };

static int Build(const int* maps, int map_count, const Building* world, int world_count, RegionInfo* info, bool compute, bool pc,
                 Line* out)
{
    long shops_for_sale = 0, houses_for_sale = 0, houses_owned = 0, shops_owned = 0;
    if (compute) {
        for (unsigned long i = 0; i < (unsigned long)map_count; ++i)
            for (int b = 0; b < world_count; ++b) {
                const Building& building = world[b];
                if (!building.buyable || building.map != maps[i]) continue;
                if (building.owned) {
                    if (building.shop) ++shops_owned; else ++houses_owned;
                } else if (building.for_sale) {
                    if (building.shop) ++shops_for_sale; else ++houses_for_sale;
                }
            }
        info->ShopsOwned = shops_owned; info->HousesOwned = houses_owned;
        info->ShopsForSale = shops_for_sale; info->HousesForSale = houses_for_sale;
    } else {
        shops_owned = info->ShopsOwned; houses_owned = info->HousesOwned;
        shops_for_sale = info->ShopsForSale; houses_for_sale = info->HousesForSale;
    }
    struct Kind { long n; const char* plural; const char* single; int b, g; };
    Kind kinds[4] = {
        { houses_owned, "TEXT_GUI_MENU_HOUSES_OWNED", "TEXT_GUI_MENU_HOUSE_OWNED", 0, -1 },
        { shops_owned, "TEXT_GUI_MENU_SHOPS_OWNED", "TEXT_GUI_MENU_SHOP_OWNED", 0, -1 },
        { houses_for_sale, "TEXT_GUI_MENU_HOUSES_FOR_SALE", "TEXT_GUI_MENU_HOUSE_FOR_SALE1", 0x80, 0x80 },
        { shops_for_sale, "TEXT_GUI_MENU_SHOPS_FOR_SALEQ1", "TEXT_GUI_MENU_SHOP_FOR_SALE1", 0x80, 0x80 } };
    int lines = 0;
    for (int k = 0; k < 4; ++k) {
        if (!kinds[k].n) continue;
        Line& line = out[lines++];
        line.symbol = kinds[k].n > 1 ? kinds[k].plural : kinds[k].single;
        line.count = kinds[k].n > 1 ? kinds[k].n : 0;
        line.templ = pc ? "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" : "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
        line.b = kinds[k].b; line.g = kinds[k].g;
    }
    return lines;
}

static int Fail() { std::printf("00600b60_TEST FAIL\n"); return 1; }

int main()
{
    int maps[2] = { 3, 7 };
    Building world[7] = {
        { 3, true, false, true, false },    // owned house
        { 7, true, false, true, true },     // owned house (owned wins over for sale)
        { 3, true, true, true, false },     // owned shop
        { 7, true, true, false, true },     // shop for sale
        { 3, true, false, false, false },   // neither
        { 9, true, false, false, true },    // other map
        { 3, false, false, true, true } };  // not a buyable house
    RegionInfo info; std::memset(&info, 0, sizeof info);
    Line lines[4];
    int n = Build(maps, 2, world, 7, &info, true, true, lines);
    if (info.HousesOwned != 2 || info.ShopsOwned != 1 || info.ShopsForSale != 1 || info.HousesForSale != 0) return Fail();
    if (n != 3 || std::strcmp(lines[0].symbol, "TEXT_GUI_MENU_HOUSES_OWNED") || lines[0].count != 2
        || std::strcmp(lines[1].symbol, "TEXT_GUI_MENU_SHOP_OWNED") || lines[1].b != 0
        || std::strcmp(lines[2].symbol, "TEXT_GUI_MENU_SHOP_FOR_SALE1") || lines[2].g != 0x80
        || std::strcmp(lines[2].templ, "PC_TEXT_INSIDE_CONTAINER_TEMPLATE")) return Fail();
    RegionInfo cached; std::memset(&cached, 0, sizeof cached); cached.HousesForSale = 5;
    n = Build(maps, 2, world, 7, &cached, false, false, lines);
    if (n != 1 || std::strcmp(lines[0].symbol, "TEXT_GUI_MENU_HOUSES_FOR_SALE") || lines[0].count != 5
        || std::strcmp(lines[0].templ, "UI_TEXT_INSIDE_CONTAINER_TEMPLATE")) return Fail();
    std::printf("00600b60_TEST PASS\n");
    return 0;
}
