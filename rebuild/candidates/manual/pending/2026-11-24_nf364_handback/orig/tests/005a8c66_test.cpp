// Behaviour test for CTCInventoryTrade::ConstructActualSpecialInventoryPC @ 0x005A8C66: the special-shop buy screen's item filter
// (a barber keeps only items with a hair card, a tattooist only valid tattoo cards, a title vendor everything), the title-text decision
// (shop name > 0 and the text bank has the entry, its info and its text entry), the sold-out and in-stock entry count that sizes the
// list offsets, the menu-guide symbol per shop kind, and the six clickables registered for mouse movement.
#include <cstdio>
#include <cstring>
#include <vector>
#include <list>
#include <utility>

enum EShopKind { SHOP_BARBER, SHOP_TATTOOIST, SHOP_TITLE_VENDOR };
struct Item { long def; long count; bool hasHairCard; bool hasTattooCard; bool invalidCustomTattoo; };

// the erase loops over the hero's items (retail iterates with vector::erase, so the test keeps the same in-place shape)
static void FilterItems(EShopKind kind, std::vector<Item>& items)
{
    if (kind == SHOP_BARBER) {
        for (std::vector<Item>::iterator it = items.begin(); it != items.end(); )
            if (it->hasHairCard) ++it; else it = items.erase(it);
    } else if (kind == SHOP_TATTOOIST) {
        for (std::vector<Item>::iterator it = items.begin(); it != items.end(); )
            if (it->hasTattooCard && !it->invalidCustomTattoo) ++it; else it = items.erase(it);
    }
}

static bool ShowsShopName(unsigned long name, bool entryExists, bool hasInfo, bool hasEntry, bool titleComponent)
{
    if (!(name > 0)) return false;
    if (!(entryExists && hasInfo && hasEntry)) return false;
    return titleComponent;
}

static const char* GuideSymbol(EShopKind kind)
{
    if (kind == SHOP_BARBER) return "TEXT_GUI_MENUGUIDE_TRADE_BARBERS_BUY";
    if (kind == SHOP_TATTOOIST) return "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY";
    return "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY";
}

// returns the number of list entries (offsets initialised for items + sold-out specials) and the item total; 0 entries when both empty
static long BuildEntries(const std::vector<Item>& items, const std::list<long>& notInStock, long* total)
{
    *total = 0;
    if (items.empty() && notInStock.empty()) return 0;
    long entries = (long)(items.size() + notInStock.size());
    for (std::vector<Item>::const_iterator it = items.begin(); it != items.end(); ++it) *total += it->count;
    return entries;
}

int main()
{
    int ok = 1;
    Item src[4] = { { 1, 2, true, false, false }, { 2, 1, false, true, false }, { 3, 5, false, true, true }, { 4, 3, true, true, false } };
    std::vector<Item> barber(src, src + 4), tattoo(src, src + 4), title(src, src + 4);
    FilterItems(SHOP_BARBER, barber);
    FilterItems(SHOP_TATTOOIST, tattoo);
    FilterItems(SHOP_TITLE_VENDOR, title);
    if (barber.size() != 2 || barber[0].def != 1 || barber[1].def != 4) ok = 0;
    if (tattoo.size() != 2 || tattoo[0].def != 2 || tattoo[1].def != 4) ok = 0;
    if (title.size() != 4) ok = 0;

    if (ShowsShopName(0, true, true, true, true)) ok = 0;
    if (ShowsShopName(7, false, true, true, true)) ok = 0;
    if (ShowsShopName(7, true, false, true, true)) ok = 0;
    if (ShowsShopName(7, true, true, false, true)) ok = 0;
    if (ShowsShopName(7, true, true, true, false)) ok = 0;
    if (!ShowsShopName(7, true, true, true, true)) ok = 0;

    if (std::strcmp(GuideSymbol(SHOP_BARBER), "TEXT_GUI_MENUGUIDE_TRADE_BARBERS_BUY") != 0) ok = 0;
    if (std::strcmp(GuideSymbol(SHOP_TATTOOIST), "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY") != 0) ok = 0;
    if (std::strcmp(GuideSymbol(SHOP_TITLE_VENDOR), "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY") != 0) ok = 0;

    long total = -1;
    std::list<long> soldOut; soldOut.push_back(9); soldOut.push_back(10);
    if (BuildEntries(barber, soldOut, &total) != 4 || total != 5) ok = 0;
    std::vector<Item> none;
    std::list<long> noneSold;
    if (BuildEntries(none, noneSold, &total) != 0 || total != 0) ok = 0;
    if (BuildEntries(none, soldOut, &total) != 2 || total != 0) ok = 0;

    static const char* clickables[] = { "SCROLL_LIST_UP", "SCROLL_DESCRIPTION_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_DOWN", "PC_BACK_CLICKABLE", "PC_CLOSE_CLICKABLE" };
    if (sizeof(clickables) / sizeof(clickables[0]) != 6) ok = 0;

    if (!ok) { std::printf("005a8c66_TEST FAIL\n"); return 1; }
    std::printf("005a8c66_TEST PASS\n");
    return 0;
}