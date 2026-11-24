// Behaviour test for 0x006001E0 CTCInventoryMap::ConstructRegionDescriptionShops: with compute set it counts every TCI_SHOP building of
// the region's maps (tavern first, then barber / tattoo / title, else general), writes the counts into the CUIRegionInfo, and adds one
// text line per non-zero kind in the order general, tavern, barber, tattoo, title (plural symbol + count when more than one, the
// PC or UI template by `pc`); without compute it reads the counts back from the CUIRegionInfo.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

enum { KIND_GENERAL, KIND_BARBER, KIND_TATTOO, KIND_TITLE };
struct Building { int map; bool shop; bool tavern; int kind; };
struct RegionInfo { long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns; };
struct Line { std::string symbol; long count; std::string templ; unsigned char b, g, r; };

static void Build(const std::vector<int>& maps, const std::vector<Building>& world, RegionInfo* info, bool compute, bool pc,
                  std::vector<Line>& out)
{
    long shops = 0, title = 0, barber = 0, tattoo = 0, taverns = 0;
    if (compute) {
        for (unsigned long i = 0; i < maps.size(); ++i)
            for (size_t b = 0; b < world.size(); ++b) {
                const Building& building = world[b];
                if (!building.shop || building.map != maps[i]) continue;
                if (building.tavern) ++taverns;
                else if (building.kind == KIND_BARBER) ++barber;
                else if (building.kind == KIND_TATTOO) ++tattoo;
                else if (building.kind == KIND_TITLE) ++title;
                else ++shops;
            }
        info->BarberShops = barber; info->TatooShops = tattoo; info->Shops = shops; info->TitleShops = title; info->Taverns = taverns;
    } else {
        barber = info->BarberShops; tattoo = info->TatooShops; shops = info->Shops; title = info->TitleShops; taverns = info->Taverns;
    }
    struct { long n; const char* stem; unsigned char b, g, r; } kinds[5] = {
        { shops, "GENERAL", 0, 0xff, 0 }, { taverns, "TAVERN", 0x3b, 0xae, 0x8d }, { barber, "BARBER", 0, 0x80, 0xff },
        { tattoo, "TATTOIST", 0, 0x80, 0xff }, { title, "TITLE", 0, 0x80, 0xff } };
    for (int k = 0; k < 5; ++k) {
        if (!kinds[k].n) continue;
        Line line;
        line.symbol = std::string("TEXT_GUI_MENU_") + kinds[k].stem + (kinds[k].n > 1 ? "_SHOPS" : "_SHOP");
        line.count = kinds[k].n > 1 ? kinds[k].n : 0;
        line.templ = pc ? "PC_TEXT_INSIDE_CONTAINER_TEMPLATE" : "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
        line.b = kinds[k].b; line.g = kinds[k].g; line.r = kinds[k].r;
        out.push_back(line);
    }
}

int main()
{
    std::vector<int> maps; maps.push_back(3); maps.push_back(7);
    std::vector<Building> world;
    Building b0 = { 3, true, false, KIND_GENERAL }; world.push_back(b0);
    Building b1 = { 7, true, false, KIND_GENERAL }; world.push_back(b1);
    Building b2 = { 3, true, true, KIND_BARBER }; world.push_back(b2);    // tavern wins over barber
    Building b3 = { 7, true, false, KIND_TITLE }; world.push_back(b3);
    Building b4 = { 9, true, false, KIND_BARBER }; world.push_back(b4);   // other map
    Building b5 = { 3, false, false, KIND_TATTOO }; world.push_back(b5);  // not a shop
    RegionInfo info; std::memset(&info, 0, sizeof info);
    std::vector<Line> lines;
    Build(maps, world, &info, true, true, lines);
    if (info.Shops != 2 || info.Taverns != 1 || info.BarberShops != 0 || info.TitleShops != 1 || info.TatooShops != 0) { std::printf("006001e0_TEST FAIL\n"); return 1; }
    if (lines.size() != 3 || lines[0].symbol != "TEXT_GUI_MENU_GENERAL_SHOPS" || lines[0].count != 2 || lines[1].symbol != "TEXT_GUI_MENU_TAVERN_SHOP"
        || lines[1].b != 0x3b || lines[2].symbol != "TEXT_GUI_MENU_TITLE_SHOP" || lines[2].templ != "PC_TEXT_INSIDE_CONTAINER_TEMPLATE") { std::printf("006001e0_TEST FAIL\n"); return 1; }
    RegionInfo cached; std::memset(&cached, 0, sizeof cached); cached.TatooShops = 4;
    std::vector<Line> lines2;
    Build(maps, world, &cached, false, false, lines2);
    if (lines2.size() != 1 || lines2[0].symbol != "TEXT_GUI_MENU_TATTOIST_SHOPS" || lines2[0].count != 4 || lines2[0].templ != "UI_TEXT_INSIDE_CONTAINER_TEMPLATE") { std::printf("006001e0_TEST FAIL\n"); return 1; }
    std::printf("006001e0_TEST PASS\n");
    return 0;
}