#include <cassert>
#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

typedef std::string CCharString;
typedef std::string CWideString;

template<class T> class CCountedPointer {
    T* data;
public:
    explicit CCountedPointer(T* p=0) : data(p) { }
    void Reset(T* p) { data=p; }
    T* Get() const { return data; }
    T* operator->() const { return data; }
};

enum { TCI_SHOP, TCI_HERO_STATS };
class CTCShop {
public:
    bool title, tattoo, barber;
    CTCShop() : title(false), tattoo(false), barber(false) { }
    bool IsTitleShop() const { return title; }
    bool IsTattooShop() const { return tattoo; }
    bool IsBarberShop() const { return barber; }
};

class CTCHeroStats {
public:
    long GetMoney() const { return 900; }
};

class CThing {
public:
    CTCShop shop;
    CTCHeroStats stats;
    void GetTC(int type, CTCShop** p) { assert(type==TCI_SHOP); *p=&shop; }
    void PeekTC(int type, const CTCHeroStats** p) const { assert(type==TCI_HERO_STATS); *p=&stats; }
};

namespace NUISystem {
enum { UI_TYPE_TEXT };

class CComponent {
    std::map<CCharString,CComponent*> by_name;
    CCharString name;
public:
    unsigned position_count, desc_count, child_adds;
    bool initialized;
    long state;
    std::vector<CCountedPointer<CComponent> > children;
    explicit CComponent(const char* n)
        : name(n), position_count(0), desc_count(0), child_adds(0), initialized(false), state(0) { }
    virtual ~CComponent() { }
    CComponent* FindChild(const char* n);
    CComponent* FindFirstChildOfType(int type) { assert(type==UI_TYPE_TEXT); return FindChild("ENTRY_TEXT"); }
    std::vector<CCountedPointer<CComponent> >& GetChildren() { return children; }
    void AddChildDynamically(const CCountedPointer<CComponent>& child)
    { children.push_back(child); ++child_adds; }
    void AddPositionChild(CComponent* child) { assert(child); ++position_count; }
    void Initialise() { initialized=true; }
    void SetName(const CCharString& n) { name=n; }
    CCharString GetName() const { return name; }
    void ChangeState(long n) { state=n; }
};

class CText : public CComponent {
    CWideString value;
public:
    explicit CText(const char* n) : CComponent(n), value(n) { }
    CWideString GetValue() const { return value; }
    void SetValue(const CWideString& text, bool a, bool b) { assert(a && b); value=text; }
};

class CList : public CText {
public:
    long offsets;
    explicit CList(const char* n) : CText(n), offsets(0) { }
    void AddNonScrollingChildDynamically(const CCountedPointer<CComponent>& child)
    { assert(child.Get()); }
    void InitialiseOffsets(long n) { offsets=n; }
};

class CViewport : public CList {
public:
    explicit CViewport(const char* n) : CList(n) { }
    void AddFakeChild(CComponent* child) { assert(child); ++desc_count; }
};

class CManager {
    std::vector<CComponent*> objects;
public:
    static CManager* GetInstance() { static CManager manager; return &manager; }
    CComponent* CreateComponent(const char* name, bool)
    {
        CComponent* p=new CViewport(name);
        objects.push_back(p);
        if (CCharString(name)=="PC_TRADING_GOODS_LIST") {
            p->children.push_back(CCountedPointer<CComponent>(CreateComponent("PC_TRADING_MENU_CLOTHING_ENTRY",false)));
            p->children.push_back(CCountedPointer<CComponent>(CreateComponent("PC_TRADING_MENU_WEAPONS_ENTRY",false)));
            p->children.push_back(CCountedPointer<CComponent>(CreateComponent("PC_TRADING_MENU_ITEMS_ENTRY",false)));
        }
        return p;
    }
    ~CManager()
    {
        for (unsigned i=0; i<objects.size(); ++i) delete objects[i];
    }
};

CComponent* CComponent::FindChild(const char* n)
{
    CComponent*& slot=by_name[n];
    if (!slot) slot=CManager::GetInstance()->CreateComponent(n,false);
    return slot;
}
}

using namespace NUISystem;

class CDataBank {
public:
    CWideString GetTextBySymbol(const char* name) const { return name; }
};

class CPlayer {
    CThing thing;
public:
    const CThing* PeekPControlledCreature() const { return &thing; }
};

class CPlayerManager {
    CPlayer player;
public:
    const CPlayer& GetMainPlayer() const { return player; }
};

class CMainGameComponent {
    CPlayerManager player_manager;
    CDataBank text_bank;
public:
    static CMainGameComponent* Get() { static CMainGameComponent game; return &game; }
    const CPlayerManager& PeekPlayerManager() const { return player_manager; }
    const CDataBank& GetTextBank() const { return text_bank; }
};

class CTCInventoryItem {
public:
    struct CCard { bool found; bool IsNull() const { return !found; } };
    static CCard GetTattooCardDef(long index) { CCard card={index==91}; return card; }
    static CCard GetHairCardDef(long index) { CCard card={index==92}; return card; }
};

class CTCInventoryStats {
public:
    static CWideString GetXString() { return " x"; }
};

CWideString GFIntToWideString(long n)
{
    char digits[24];
    std::sprintf(digits,"%ld",n);
    return digits;
}

static const char* CLOTHES_SUBTITLE="Clothes_Subtitle";
static const char* WEAPONS_SUBTITLE="Weapons_Subtitle";
static const char* ITEMS_SUBTITLE="Items_Subtitle";

class CTCInventoryTrade {
public:
    typedef std::map<CComponent*, CList*> Lists;
    typedef std::map<CComponent*, std::pair<CWideString, long> > Totals;
    enum EItem_Menu_Type { ITEM_MENU_TYPE_CLOTHES, ITEM_MENU_TYPE_WEAPONS, ITEM_MENU_TYPE_ITEMS };
    CThing building;
    CComponent* PTop_Level_List;
    CComponent* PCurrentOpenTradeMenu;
    std::vector<CComponent*> ItemBuyLists, ItemsToBuy, Components;
    std::list<long> NotInStock;
    Lists BuyListsByCategory;
    Totals NumBuyableThings;
    long TotNumClothesToBuy, TotNumWeaponsToBuy, TotNumItemsToBuy;
    unsigned refreshes, attached, accepted, listed_unavailable;
    bool list_goods;
    explicit CTCInventoryTrade(bool list)
        : PTop_Level_List(CManager::GetInstance()->CreateComponent("TOP",false)),
          PCurrentOpenTradeMenu(0), refreshes(0), attached(0), accepted(0), listed_unavailable(0), list_goods(list) { }
    CThing* GetBuildingThing() { return &building; }
    void ResetTradeLists() { }
    void InitialiseBuyableItems(std::vector<std::pair<long,long> >& buyable)
    {
        if (!list_goods) return;
        buyable.push_back(std::make_pair(1L,3L));
        buyable.push_back(std::make_pair(2L,4L));
        buyable.push_back(std::make_pair(3L,5L));
        buyable.push_back(std::make_pair(91L,6L));
        buyable.push_back(std::make_pair(92L,7L));
    }
    void AttachToFrame(CComponent* frame) { assert(frame); ++attached; }
    CComponent* GetMenuBranchFromDefIndexPC(long index, CComponent* menu, EItem_Menu_Type& type)
    {
        assert(index>0 && index<=3);
        type=static_cast<EItem_Menu_Type>(index-1);
        return menu->children[index-1].Get();
    }
    bool AddBuyItemToListPC(CComponent* frame, long index, long qty, CList* list, CViewport* view, bool pc)
    {
        assert(frame && list && view && pc && index>0 && index<=3);
        if (qty) ++accepted;
        else ++listed_unavailable;
        return true;
    }
    void ConstructSubtitlesPC(CComponent* menu, const Totals& totals, CComponent* frame)
    { assert(menu && frame && totals.size()==(list_goods?3U:0U)); }
    void RefreshEntriesAmountsAndAvailabilitiesPC(CComponent* menu, const Lists& lists)
    { assert(menu && lists.size()==(list_goods?3U:0U)); ++refreshes; }
    void ConstructActualBuyInventoryPC();
};

// The selected implementation is reproduced verbatim for this standalone fixture.
void CTCInventoryTrade::ConstructActualBuyInventoryPC()
{
    ItemBuyLists.clear();
    ResetTradeLists();
    std::vector<std::pair<long, long> > buyable;
    CTCShop* ptc_shop = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &ptc_shop);
    InitialiseBuyableItems(buyable);
    for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ) {
        if (!CTCInventoryItem::GetTattooCardDef(it->first).IsNull() || !CTCInventoryItem::GetHairCardDef(it->first).IsNull())
            it = buyable.erase(it);
        else
            ++it;
    }
    ItemsToBuy.clear();
    BuyListsByCategory.clear();
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    CManager* pmanager = CManager::GetInstance();
    CComponent* plist_component = PTop_Level_List;
    plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    PCurrentOpenTradeMenu = pmanager->CreateComponent("PC_TRADING_GOODS_LIST", false);
    PCurrentOpenTradeMenu->Initialise();
    AttachToFrame(plist_component->FindChild("PC_MENU_ENTRY_TRADE_BUY"));
    CComponent* pframe = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    CComponent* pviewport_component = PCurrentOpenTradeMenu->FindChild("PC_DESCRIPTION_VIEWPORT");
    CViewport* pviewport = static_cast<CViewport*>(pviewport_component);
    pframe->AddPositionChild(pviewport);
    CList* plist_of_goods = static_cast<CList*>(PCurrentOpenTradeMenu);
    CCountedPointer<CComponent> pdesc;
    CCountedPointer<CComponent> pbuytitle(pmanager->CreateComponent("PC_TRADE_BUY_TITLE", false));
    plist_of_goods->AddNonScrollingChildDynamically(pbuytitle);
    pframe->AddPositionChild(pbuytitle.Get());

    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyable.empty() || !NotInStock.empty()) {
        std::map<CComponent*, long> same_categories;
        for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = same_categories.find(pentry);
            if (found == same_categories.end())
                same_categories[pentry] = 1;
            else
                ++found->second;
        }
        for (std::list<long>::iterator it = NotInStock.begin(); it != NotInStock.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
            std::map<CComponent*, long>::iterator found = same_categories.find(pentry);
            if (found == same_categories.end())
                same_categories[pentry] = 1;
            else
                ++found->second;
        }
        for (std::map<CComponent*, long>::iterator it = same_categories.begin(); it != same_categories.end(); ++it) {
            CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_SUB_LIST", false));
            CList* pbuyable_list = static_cast<CList*>(pchild.Get());
            it->first->AddChildDynamically(pchild);
            pbuyable_list->InitialiseOffsets(it->second);
            BuyListsByCategory[it->first] = pbuyable_list;
        }
        NumBuyableThings.clear();
        for (std::vector<std::pair<long, long> >::iterator it = buyable.begin(); it != buyable.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu, type);
            if (AddBuyItemToListPC(pframe, it->first, it->second, BuyListsByCategory.find(pentry)->second, pviewport, true)) {
                if (NumBuyableThings.find(pentry) == NumBuyableThings.end())
                    NumBuyableThings[pentry].second = it->second;
                else
                    NumBuyableThings[pentry].second += it->second;
                switch (type) {
                case ITEM_MENU_TYPE_CLOTHES: TotNumClothesToBuy += it->second; break;
                case ITEM_MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += it->second; break;
                case ITEM_MENU_TYPE_ITEMS:   TotNumItemsToBuy += it->second;   break;
                }
            }
        }
        std::map<CComponent*, std::pair<CWideString, long> >::iterator endit = NumBuyableThings.end();
        for (std::map<CComponent*, std::pair<CWideString, long> >::iterator it = NumBuyableThings.begin(); it != endit; ++it) {
            CComponent* component = it->first->FindFirstChildOfType(UI_TYPE_TEXT);
            CText* p_text = static_cast<CText*>(component);
            CWideString v(p_text->GetValue());
            it->second.first = v;
            if (it->second.second > 0) {
                v += CTCInventoryStats::GetXString();
                v += GFIntToWideString(it->second.second);
            }
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumClothesToBuy);
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumWeaponsToBuy);
            p_text->SetValue(v, true, true);
        }
        {
            CText* p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
            CWideString v(p_text->GetValue());
            v += CTCInventoryStats::GetXString();
            v += GFIntToWideString(TotNumItemsToBuy);
            p_text->SetValue(v, true, true);
        }
        for (std::list<long>::iterator it = NotInStock.begin(); it != NotInStock.end(); ++it) {
            EItem_Menu_Type type;
            CComponent* pentry = GetMenuBranchFromDefIndexPC(*it, PCurrentOpenTradeMenu, type);
            AddBuyItemToListPC(pframe, *it, 0, BuyListsByCategory.find(pentry)->second, pviewport, true);
        }
    }

    CWideString XString = CTCInventoryStats::GetXString();
    CWideString new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumItemsToBuy + TotNumClothesToBuy + TotNumWeaponsToBuy);
    CCountedPointer<CComponent> pbuy_subtitle;
    CText* pbuy_subtitle_real;
    CComponent* ui_trading_goods_list = PCurrentOpenTradeMenu;
    std::vector<CCountedPointer<CComponent> >& ui_trading_goods_list_children_buy = ui_trading_goods_list->GetChildren();
    for (long i = 0; i < (long)ui_trading_goods_list_children_buy.size(); ++i) {
        pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
        ui_trading_goods_list_children_buy[i]->AddChildDynamically(pdesc);
        pframe->AddPositionChild(pdesc.Get());
        pviewport->AddFakeChild(pdesc.Get());
        pbuy_subtitle.Reset(pmanager->CreateComponent("PC_SUBTITLE", false));
        pbuy_subtitle_real = static_cast<CText*>(pbuy_subtitle.Get());
        CWideString v;
        CCharString n = ui_trading_goods_list_children_buy[i]->GetName();
        if (n == "PC_TRADING_MENU_CLOTHING_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + XString;
            v += GFIntToWideString(TotNumClothesToBuy);
            pbuy_subtitle->SetName(CLOTHES_SUBTITLE);
        } else if (n == "PC_TRADING_MENU_WEAPONS_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + XString;
            v += GFIntToWideString(TotNumWeaponsToBuy);
            pbuy_subtitle->SetName(WEAPONS_SUBTITLE);
        } else if (n == "PC_TRADING_MENU_ITEMS_ENTRY") {
            v = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + XString;
            v += GFIntToWideString(TotNumItemsToBuy);
            pbuy_subtitle->SetName(ITEMS_SUBTITLE);
        }
        pbuy_subtitle_real->SetValue(v, true, true);
        ui_trading_goods_list_children_buy[i]->AddChildDynamically(pbuy_subtitle);
        pframe->AddPositionChild(pbuy_subtitle.Get());
    }
    {
        CComponent* ui_clothes_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_CLOTHING_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_clothes_goods_list_children_buy = ui_clothes_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_clothes_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_clothes_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    {
        CComponent* ui_weapons_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_WEAPONS_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_weapons_goods_list_children_buy = ui_weapons_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_weapons_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_weapons_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    {
        CComponent* ui_items_goods_list = ui_trading_goods_list->FindChild("PC_TRADING_ITEMS_LIST");
        std::vector<CCountedPointer<CComponent> >& ui_items_goods_list_children_buy = ui_items_goods_list->GetChildren();
        for (long i = 0; i < (long)ui_items_goods_list_children_buy.size(); ++i) {
            pdesc.Reset(pmanager->CreateComponent("PC_TRADE_BUY_DESC", false));
            ui_items_goods_list_children_buy[i]->AddChildDynamically(pdesc);
            pframe->AddPositionChild(pdesc.Get());
            pviewport->AddFakeChild(pdesc.Get());
        }
    }
    ConstructSubtitlesPC(ui_trading_goods_list, NumBuyableThings, pframe);

    CText* pgold_real = static_cast<CText*>(plist_of_goods->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"));
    if (pgold_real) {
        const CTCHeroStats* pTCHeroStats = 0;
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature()->PeekTC(TCI_HERO_STATS, &pTCHeroStats);
        long cash = pTCHeroStats->GetMoney();
        pgold_real->SetValue(GFIntToWideString(cash), true, true);
    }
    PCurrentOpenTradeMenu->ChangeState(5);
    Components.push_back(PCurrentOpenTradeMenu);
    if (!ptc_shop->IsTitleShop() && !ptc_shop->IsTattooShop() && !ptc_shop->IsBarberShop())
        RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, BuyListsByCategory);
}

int main()
{
    CTCInventoryTrade normal(true);
    normal.NotInStock.push_back(2);
    normal.ConstructActualBuyInventoryPC();
    assert(normal.attached==1 && normal.accepted==3 && normal.listed_unavailable==1 && normal.refreshes==1);
    assert(normal.TotNumClothesToBuy==3 && normal.TotNumWeaponsToBuy==4 && normal.TotNumItemsToBuy==5);
    CComponent* menu=normal.PCurrentOpenTradeMenu;
    assert(menu->state==5 && menu->initialized && normal.Components.size()==1 && normal.Components[0]==menu);
    assert(menu->FindChild("PC_DESCRIPTION_VIEWPORT")->desc_count==3);
    assert(static_cast<CText*>(menu->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN"))->GetValue()=="900");
    assert(menu->children[0]->child_adds==3);
    CText* subtitle=static_cast<CText*>(menu->children[0]->children[2].Get());
    assert(subtitle->GetName()=="Clothes_Subtitle" && subtitle->GetValue()=="TEXT_GUI_MENU_CLOTHING x3");

    CTCInventoryTrade special(false);
    special.building.shop.tattoo=true;
    special.ConstructActualBuyInventoryPC();
    assert(special.refreshes==0 && special.accepted==0 && special.TotNumItemsToBuy==0);
    assert(special.Components.size()==1 && special.PCurrentOpenTradeMenu->state==5);
    std::puts("005ac10e_TEST PASS");
}
