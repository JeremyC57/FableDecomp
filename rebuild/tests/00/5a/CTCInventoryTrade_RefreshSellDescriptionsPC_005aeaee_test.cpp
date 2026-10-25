// Behaviour test for CTCInventoryTrade::RefreshSellDescriptionsPC (0x005AEAEE): every sell entry that already has a
// description child gets it marked for deletion (method 1) and a new description (built for selling) is position-added
// to the parent, named, added to the entry and handed to the viewport; entries without a description are skipped --
// through a copy of the body over recording stand-in components.
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
    std::vector<Component*> positionChildren;
    std::vector<Component*> fakeChildren;
    Component(const std::string& n) : name(n), deletionMethod(0), builtFor(-1), buying(false) { }
    Component* FindChild(const std::string& n)
    {
        for (unsigned int i = 0; i < children.size(); ++i)
            if (children[i]->name == n) return children[i];
        return 0;
    }
};

static const std::string kSellDescription = "SELL_DESC";
static std::vector<Component*> g_made;

struct Trade {
    std::map<Component*, std::pair<long, long> > ItemsToSell;
    Component* PCurrentOpenTradeMenu;
    Component* ConstructTradeDescription(long defIndex, unsigned int, bool buying, bool)
    {
        Component* c = new Component("new");
        c->builtFor = defIndex;
        c->buying = buying;
        g_made.push_back(c);
        return c;
    }
    void RefreshSellDescriptionsPC(Component* parent)
    {
        Component* viewport = PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT");
        for (std::map<Component*, std::pair<long, long> >::iterator it = ItemsToSell.begin(); it != ItemsToSell.end(); ++it) {
            Component* oldDescription = it->first->FindChild(kSellDescription);
            if (oldDescription != 0) {
                oldDescription->deletionMethod = 1;
                Component* description = ConstructTradeDescription(it->second.first, 1, false, false);
                parent->positionChildren.push_back(description);
                description->name = kSellDescription;
                it->first->children.push_back(description);
                viewport->fakeChildren.push_back(description);
            }
        }
    }
};

int main()
{
    Component menu("menu"), viewport("PC_DESCRIPTION_VIEWPORT"), parent("parent");
    menu.children.push_back(&viewport);
    Component a("a"), b("b"), oldA(kSellDescription);
    a.children.push_back(&oldA);
    Trade trade;
    trade.PCurrentOpenTradeMenu = &menu;
    trade.ItemsToSell[&a] = std::make_pair(17L, 2L);
    trade.ItemsToSell[&b] = std::make_pair(23L, 1L);
    trade.RefreshSellDescriptionsPC(&parent);
    if (oldA.deletionMethod != 1 || g_made.size() != 1 || g_made[0]->builtFor != 17 || g_made[0]->buying ||
        g_made[0]->name != kSellDescription || a.children.size() != 2 || a.children[1] != g_made[0] ||
        b.children.size() != 0 || parent.positionChildren.size() != 1 || viewport.fakeChildren.size() != 1 ||
        viewport.fakeChildren[0] != g_made[0]) {
        std::printf("005aeaee_TEST FAIL\n");
        return 1;
    }
    for (unsigned int i = 0; i < g_made.size(); ++i) delete g_made[i];
    std::printf("005aeaee_TEST PASS\n");
    return 0;
}