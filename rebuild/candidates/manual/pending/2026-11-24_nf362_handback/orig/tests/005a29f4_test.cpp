// Behaviour test for CTCInventoryTrade::ConstructSpecialShopList @ 0x005A29F4: the console special shop (barber / tattooist / title
// vendor). The four trade maps are cleared; the title is the shop-type title unless the text bank has the shop's own name; when there
// are specials or out-of-stock specials the sub-list gets one offset slot per item, every item is added (a bought-out one with count 0)
// and the buyable ones are summed into TotSpecialItemsToBuy, the buy entry reads "<Buy text> x <total>" (also as the one-off
// subtitle) with the shop-type menu guide; the gold text shows the hero's money and the screen title reads "Trade".
#include <cstdio>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
enum ShopType { BARBER, TATTOO, TITLE };
struct Screen {
    std::string title, buyCaption, subtitle, guide, gold, screenTitle;
    long offsets; std::vector<std::pair<long, long> > rows; long total;
};
struct Trade {
    std::map<int, int> itemsToBuy, itemsToSell, sellLists, buyLists;
    std::list<long> notInStock;
    long totSpecial;
};
static std::string Num(long n) { std::ostringstream o; o << n; return o.str(); }
static bool AddBuyItem(long def, long count) { return count > 0 && def != 13; }   // stand-in: def 13 cannot be added
static Screen Construct(Trade& t, const std::vector<std::pair<long, long> >& items, ShopType type, const char* shopName, long money)
{
    Screen s; s.offsets = 0; s.total = 0;
    t.itemsToBuy.clear(); t.itemsToSell.clear(); t.sellLists.clear(); t.buyLists.clear();
    s.title = type == BARBER ? "UI_TRADE_BARBER_TITLE" : type == TATTOO ? "UI_TRADE_TATTOOIST_TITLE" : "UI_TRADE_TITLE_VENDOR_TITLE";
    if (shopName) s.title = shopName;
    t.totSpecial = 0;
    if (!items.empty() || !t.notInStock.empty()) {
        s.offsets = (long)(items.size() + t.notInStock.size());
        for (unsigned int i = 0; i < items.size(); ++i) {
            s.rows.push_back(items[i]);
            if (AddBuyItem(items[i].first, items[i].second)) t.totSpecial += items[i].second;
        }
        s.buyCaption = "Buy x " + Num(t.totSpecial);
        s.subtitle = s.buyCaption;
        s.guide = type == BARBER ? "TEXT_GUI_MENUGUIDE_TRADE_BARBERS_BUY" : type == TATTOO ? "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY"
                                                                     : "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY";
        for (std::list<long>::iterator n = t.notInStock.begin(); n != t.notInStock.end(); ++n)
            s.rows.push_back(std::make_pair(*n, 0L));
    }
    s.total = t.totSpecial;
    s.gold = Num(money);
    s.screenTitle = "Trade";
    return s;
}
int main()
{
    int ok = 1;
    Trade t; t.itemsToBuy[1] = 1; t.buyLists[2] = 2;
    t.notInStock.push_back(40);
    std::vector<std::pair<long, long> > items;
    items.push_back(std::make_pair(10L, 1L)); items.push_back(std::make_pair(11L, 2L)); items.push_back(std::make_pair(13L, 5L));
    Screen s = Construct(t, items, TATTOO, 0, 250);
    if (!t.itemsToBuy.empty() || !t.buyLists.empty()) ok = 0;
    if (s.title != "UI_TRADE_TATTOOIST_TITLE" || s.offsets != 4 || s.rows.size() != 4 || s.rows[3].first != 40 || s.rows[3].second != 0) ok = 0;
    if (s.total != 3 || s.buyCaption != "Buy x 3" || s.subtitle != s.buyCaption) ok = 0;
    if (s.guide != "TEXT_GUI_MENUGUIDE_TRADE_TATTOOIST_BUY" || s.gold != "250" || s.screenTitle != "Trade") ok = 0;
    Trade e; std::vector<std::pair<long, long> > none;
    Screen b = Construct(e, none, BARBER, "Bob's", 7);
    if (b.title != "Bob's" || b.offsets != 0 || !b.rows.empty() || b.buyCaption != "" || e.totSpecial != 0 || b.gold != "7") ok = 0;
    Screen v = Construct(t, items, TITLE, 0, 0);
    if (v.title != "UI_TRADE_TITLE_VENDOR_TITLE" || v.guide != "TEXT_GUI_MENUGUIDE_TRADE_TITLEVENDOR_BUY") ok = 0;
    if (!ok) { std::printf("005a29f4_TEST FAIL\n"); return 1; }
    std::printf("005a29f4_TEST PASS\n");
    return 0;
}