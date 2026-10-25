// Behaviour test for CTCInventoryTrade::RefreshDescriptions (0x005AE6AF): both the buy and the sell entries that already
// have a description child get it marked for deletion (method 1) and replaced by a new description (built for buying /
// selling respectively), named, added to the entry and handed to the first component's text-area viewport -- through a
// copy of the body over recording stand-in components.
#include <cstdio>
#include <map>
#include <string>
#include <utility>
#include <vector>

struct Component {
    std::string name;
    int deletionMethod;
    long builtFor;
    bool buying;
    std::vector<Component*> children;
    std::vector<Component*> fakeChildren;
    Component(const std::string& n) : name(n), deletionMethod(0), builtFor(-1), buying(false) { }
    Component* FindChild(const std::string& n)
    {
        for (unsigned int i = 0; i < children.size(); ++i)
            if (children[i]->name == n) return children[i];
        return 0;
    }
};

static const std::string kBuyDescription = "BUY_DESC";
static const std::string kSellDescription = "SELL_DESC";
static std::vector<Component*> g_made;

struct Trade {
    std::vector<Component*> Components;
    std::map<Component*, std::pair<long, long> > ItemsToBuy;
    std::map<Component*, std::pair<long, long> > ItemsToSell;
    Component* ConstructTradeDescription(long defIndex, unsigned int, bool buying, bool)
    {
        Component* c = new Component("new");
        c->builtFor = defIndex;
        c->buying = buying;
        g_made.push_back(c);
        return c;
    }
    void Refresh(std::map<Component*, std::pair<long, long> >& items, const std::string& name, bool buying,
                 Component* viewport)
    {
        for (std::map<Component*, std::pair<long, long> >::iterator it = items.begin(); it != items.end(); ++it) {
            Component* oldDescription = it->first->FindChild(name);
            if (oldDescription != 0) {
                oldDescription->deletionMethod = 1;
                Component* description = ConstructTradeDescription(it->second.first, 1, buying, false);
                description->name = name;
                it->first->children.push_back(description);
                viewport->fakeChildren.push_back(description);
            }
        }
    }
    void RefreshDescriptions()
    {
        Component* viewport = Components[0]->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
        Refresh(ItemsToBuy, kBuyDescription, true, viewport);
        Refresh(ItemsToSell, kSellDescription, false, viewport);
    }
};

int main()
{
    Component frame("frame"), viewport("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    frame.children.push_back(&viewport);
    Component buy("buy"), sell("sell"), plain("plain"), oldBuy(kBuyDescription), oldSell(kSellDescription);
    buy.children.push_back(&oldBuy);
    sell.children.push_back(&oldSell);
    Trade trade;
    trade.Components.push_back(&frame);
    trade.ItemsToBuy[&buy] = std::make_pair(5L, 1L);
    trade.ItemsToBuy[&plain] = std::make_pair(6L, 1L);
    trade.ItemsToSell[&sell] = std::make_pair(9L, 3L);
    trade.RefreshDescriptions();
    if (oldBuy.deletionMethod != 1 || oldSell.deletionMethod != 1 || g_made.size() != 2 ||
        g_made[0]->builtFor != 5 || !g_made[0]->buying || g_made[0]->name != kBuyDescription ||
        g_made[1]->builtFor != 9 || g_made[1]->buying || g_made[1]->name != kSellDescription ||
        plain.children.size() != 0 || buy.children.size() != 2 || sell.children.size() != 2 ||
        viewport.fakeChildren.size() != 2) {
        std::printf("005ae6af_TEST FAIL\n");
        return 1;
    }
    for (unsigned int i = 0; i < g_made.size(); ++i) delete g_made[i];
    std::printf("005ae6af_TEST PASS\n");
    return 0;
}