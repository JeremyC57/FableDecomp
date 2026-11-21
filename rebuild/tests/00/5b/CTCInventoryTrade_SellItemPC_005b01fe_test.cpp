// Behaviour test for 0x005B01FE CTCInventoryTrade::SellItemPC (source-level, as the landed CTCInventoryTrade TU-mate tests
// 0x005AECC0 / 0x005A6769 are). The row's anonymous-namespace subtitle externs (`anonymous namespace'::WEAPONS_SUBTITLE, ...) are
// hashed per TU (?A0x... changes with the object path), so the real row object cannot be bound from a test; the exact production body
// below is compiled against controlled stand-ins with the row's interfaces and checked against an independent model of the sale:
// sell entry / branch counts and captions, the per-category and Sell totals and their subtitle mirrors, the deletion arm (entry and
// empty branch) v the re-title arm, the shop's purchase / payment / inventory removal, the buy-back recount (affordable with room, specials
// apart, the sold item merged or inserted under the buy map's size), the Buy caption, every gold readout, and the closing calls.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define CHECK(c) do { if (!(c)) { printf("check failed at line %d (case %d)\n", __LINE__, g_case); exit(1); } } while (0)
static int g_case = 0;
class CCharString { public: std::string Text; CCharString() {} CCharString(const char* s, long = -1) : Text(s) {} };
class CWideString { public:
    std::string Text;
    CWideString() {}
    CWideString(const char* s) : Text(s) {}
    const CWideString& operator+=(const CWideString& s) { Text += s.Text; return *this; }
};
CWideString FABLE_FASTCALL operator+(const CWideString& a, const CWideString& b) { CWideString r(a); r += b; return r; }
CWideString FABLE_FASTCALL GFIntToWideString(long n) { char b[48]; sprintf(b, "%ld", n); return CWideString(b); }
namespace NUISystem {
enum EType { UI_TYPE_TEXT = 6 };
enum EEvent { UI_EVENT_BACK = 5 };
class CDeletion { public: enum EDeletionMethod { UI_DELETE_NONE = 0, UI_DELETE_THIS_INSTANCE = 1 }; EDeletionMethod Method; CDeletion() : Method(UI_DELETE_NONE) {} };
class CComponent {
public:
    std::map<std::string, CComponent*> Children;
    std::vector<std::pair<std::string, CComponent*> > All;     // FindAllChildren's answer, in order (null entries allowed)
    long Unavailable, Deleted, FindAlls;
    CComponent() : Unavailable(0), Deleted(0), FindAlls(0) {}
    virtual ~CComponent() {}
    CComponent* FindChild(CCharString n) { std::map<std::string, CComponent*>::iterator i = Children.find(n.Text); CHECK(i != Children.end()); return i->second; }
    CComponent* FindFirstChildOfType(EType t) { CHECK(t == UI_TYPE_TEXT); return FindChild("Text"); }
    void FindAllChildren(CCharString n, std::list<CComponent*>& out) {
        CHECK(out.empty()); ++FindAlls;
        for (unsigned i = 0; i < All.size(); ++i) if (All[i].first == n.Text) out.push_back(All[i].second);
    }
    void MakeUnavailableDynamically() { ++Unavailable; }
    void SetDeletion(CDeletion d) { CHECK(d.Method == CDeletion::UI_DELETE_THIS_INSTANCE); ++Deleted; }
};
class CText : public CComponent { public: CWideString Value; long Writes; CText() : Writes(0) {} void SetValue(const CWideString& s, bool a, bool b) { CHECK(a && b); Value = s; ++Writes; } };
class CList : public CComponent {};
static long Events = 0;
class CManager { public: static CManager* GetInstance() { static CManager m; return &m; } void SetEvent(EEvent e) { CHECK(e == UI_EVENT_BACK); ++Events; } };
}
using namespace NUISystem;
enum ETCInterfaceType { TCI_HERO_STATS = 4, TCI_SHOP = 83, InvBase = 100 };
struct Def { long BuyPrice, SellPrice, Type, Inv; bool Space, Special; };
static Def Defs[16];
static long SellPriceQueries, SpaceQueries, SpecialQueries, TypeQueries, PeekQueries;
class CTCShop {
public:
    long Sold, LastSold, BuyQueries;
    CTCShop() : Sold(0), LastSold(-1), BuyQueries(0) {}
    long GetCurrentBuyPrice(long d, long n) const { CHECK(d >= 0 && d < 16 && n > 0); ++const_cast<CTCShop*>(this)->BuyQueries; return Defs[d].BuyPrice * n; }
    long GetCurrentSellPrice(long d, long n) const { CHECK(d >= 0 && d < 16 && n == 1); ++SellPriceQueries; return Defs[d].SellPrice; }
    void BuyItem(long d) { ++Sold; LastSold = d; }
};
class CTCHeroStats { public: long Money, Calls; CTCHeroStats() : Money(0), Calls(0) {} long GetMoney() const { return Money; } void AddMoney(long n, bool f) { CHECK(f); Money += n; ++Calls; } };
namespace NInventory {
class CTCInventoryBase {
public:
    long Removed, RemovedDef, Calls;
    CTCInventoryBase() : Removed(0), RemovedDef(-1), Calls(0) {}
    bool IsSpaceInInventoryForItem(long d) const { CHECK(d >= 0 && d < 16); ++SpaceQueries; return Defs[d].Space; }
    void RemoveItemFromInventory(long d, long n) { Removed += n; RemovedDef = d; ++Calls; }
};
}
static CTCShop Shop; static CTCHeroStats Stats; static NInventory::CTCInventoryBase Inventories[3]; static bool InvPresent[3];
static long ShopLookups, HeroLookups, InvLookups;
class CThing {
public:
    bool IsBuilding;
    CThing(bool b = false) : IsBuilding(b) {}
    template <class T> bool GetTC(ETCInterfaceType t, T** p) {
        if (t == TCI_SHOP) { CHECK(IsBuilding); ++ShopLookups; *p = (T*)&Shop; return true; }
        CHECK(!IsBuilding);
        if (t == TCI_HERO_STATS) { ++HeroLookups; *p = (T*)&Stats; return true; }
        CHECK(t >= InvBase && t < InvBase + 3); ++InvLookups; *p = (T*)&Inventories[t - InvBase]; return true;
    }
    template <class T> bool PeekTC(ETCInterfaceType t, const T** p) const {
        CHECK(!IsBuilding && t >= InvBase && t < InvBase + 3 && *p == 0); ++PeekQueries;
        if (!InvPresent[t - InvBase]) return false;
        *p = (const T*)&Inventories[t - InvBase]; return true;
    }
};
class CThingPlayerCreature : public CThing {};
static CThingPlayerCreature Hero; static CThing Building(true);
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const { return &Hero; } };
class CPlayerManager { public: const CPlayer& GetMainPlayer() const { static CPlayer p; return p; } };
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& s) const {
        if (s.Text == "TEXT_GUI_MENU_ITEMS") return CWideString("Items");
        if (s.Text == "TEXT_GUI_MENU_WEAPONS") return CWideString("Weapons");
        if (s.Text == "TEXT_GUI_MENU_CLOTHING") return CWideString("Clothing");
        if (s.Text == "TEXT_GUI_MENU_SELL") return CWideString("Sell");
        CHECK(s.Text == "TEXT_GUI_MENU_BUY"); return CWideString("Buy");
    }
};
}
using NGameText::CDataBank;
class CMainGameComponent { public: static CMainGameComponent* Get() { static CMainGameComponent g; return &g; } const CDataBank& GetTextBank() const { static CDataBank d; return d; } const CPlayerManager& PeekPlayerManager() const { static CPlayerManager m; return m; } };
class CTCInventoryStats { public: static CWideString GetXString() { return CWideString(" x "); } };
class CTCInventoryItem { public: static ETCInterfaceType GetInventoryType(long d) { CHECK(d >= 0 && d < 16); return (ETCInterfaceType)(InvBase + Defs[d].Inv); } };
namespace { CCharString ITEMS_SUBTITLE("Items_Subtitle"), WEAPONS_SUBTITLE("Weapons_Subtitle"), CLOTHES_SUBTITLE("Clothes_Subtitle"); }
typedef std::map<CComponent*, std::pair<long, long> > CComponentPairMap;
typedef std::map<CComponent*, CList*> CComponentListMap;
typedef std::map<CComponent*, std::pair<CWideString, long> > CSellableMap;
class CTCInventoryTrade {
public:
    enum EItem_Menu_Type { MENU_TYPE_CLOTHING = 0, MENU_TYPE_WEAPONS = 1, MENU_TYPE_ITEMS = 2 };
    CComponent* Components[1];
    CComponent* PCurrentOpenTradeMenu;
    CComponentPairMap ItemsToBuy, ItemsToSell;
    CComponentListMap SellListsByCategory;
    CSellableMap NumSellableThings;
    long TotNumClothesToSell, TotNumWeaponsToSell, TotNumItemsToSell;
    long TotNumClothesToBuy, TotNumWeaponsToBuy, TotNumItemsToBuy, TotSpecialItemsToBuy;
    // fixture
    CComponent* Branch; EItem_Menu_Type BranchType; long BranchQueries;
    long Subtitles, Names, NameDef, NameCount; CComponent* NameEntry; long SellUpdates, Descriptions; CComponent* DescriptionFrame; long EntryRefreshes;
    CTCInventoryTrade() : BranchQueries(0), Subtitles(0), Names(0), NameDef(-1), NameCount(-1), NameEntry(0), SellUpdates(0), Descriptions(0), DescriptionFrame(0), EntryRefreshes(0) {}
    CComponent* GetMenuBranchFromDefIndexPC(long d, CComponent* m, EItem_Menu_Type& t) { CHECK(d >= 0 && d < 16 && m == PCurrentOpenTradeMenu); ++BranchQueries; t = BranchType; return Branch; }
    void RefreshSubtitlesPC(CComponent* m, CSellableMap& s) { CHECK(m == PCurrentOpenTradeMenu && &s == &NumSellableThings); ++Subtitles; }
    void RefreshNameAndTitlePC(CComponent* e, long d, long n) { ++Names; NameEntry = e; NameDef = d; NameCount = n; }
    CThing* GetBuildingThing() const { return &Building; }
    void UpdateSellActionsList() { ++SellUpdates; }
    void RefreshSellDescriptionsPC(CComponent* f) { ++Descriptions; DescriptionFrame = f; }
    bool IsSpecialCase(long d) { CHECK(d >= 0 && d < 16); ++SpecialQueries; return Defs[d].Special; }
    EItem_Menu_Type GetTypeFromDefIndexPC(long d) { CHECK(d >= 0 && d < 16); ++TypeQueries; return (EItem_Menu_Type)Defs[d].Type; }
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilitiesPC(CComponent* m, CComponentListMap& l) { CHECK(m == PCurrentOpenTradeMenu && &l == &SellListsByCategory); ++EntryRefreshes; return std::make_pair(0UL, 0UL); }
    void SellItemPC(CComponent* pcomponent, long quantity);
};

// Exact production target body follows (src/005b01fe.cpp, verbatim).
void CTCInventoryTrade::SellItemPC(CComponent* pcomponent, long quantity)
{
    const CDataBank& rTextBank = CMainGameComponent::Get()->GetTextBank();
    std::pair<long, long>& items = ItemsToSell[pcomponent];
    long item_def_index = items.first;
    CWideString XString = CTCInventoryStats::GetXString();
    items.second -= quantity;
    CComponent* sold_pentry = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_SELL");

    // the category branch the item sits under: its "x N" subtitle drops by the amount sold
    EItem_Menu_Type sold_type;
    CComponent* sold_pparent = GetMenuBranchFromDefIndexPC(item_def_index, PCurrentOpenTradeMenu, sold_type);
    NumSellableThings[sold_pparent].second -= quantity;
    CText* upper_menu_entry = static_cast<CText*>(sold_pparent->FindFirstChildOfType(UI_TYPE_TEXT));
    CWideString sold_v(NumSellableThings[sold_pparent].first);
    if (NumSellableThings[sold_pparent].second > 0) {
        sold_v += CTCInventoryStats::GetXString();
        sold_v += GFIntToWideString(NumSellableThings[sold_pparent].second);
    }
    upper_menu_entry->SetValue(sold_v, true, true);
    RefreshSubtitlesPC(PCurrentOpenTradeMenu, NumSellableThings);

    // the per-type total ("Items x N") and the matching subtitle component
    CText* p_text = 0;
    long new_tot = 0;
    CWideString new_title;
    CCharString new_subtitles;
    switch (sold_type) {
    case MENU_TYPE_ITEMS:
        p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_ITEMS"));
        TotNumItemsToSell -= quantity;
        new_tot = TotNumItemsToSell;
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + XString;
        new_subtitles = ITEMS_SUBTITLE;
        break;
    case MENU_TYPE_WEAPONS:
        p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_WEAPONS"));
        TotNumWeaponsToSell -= quantity;
        new_tot = TotNumWeaponsToSell;
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + XString;
        new_subtitles = WEAPONS_SUBTITLE;
        break;
    case MENU_TYPE_CLOTHING:
        p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild("PC_TEXT_CLOTHING"));
        TotNumClothesToSell -= quantity;
        new_tot = TotNumClothesToSell;
        new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + XString;
        new_subtitles = CLOTHES_SUBTITLE;
        break;
    }
    if (p_text) {
        new_title += GFIntToWideString(new_tot);
        p_text->SetValue(new_title, true, true);
        p_text = static_cast<CText*>(PCurrentOpenTradeMenu->FindChild(new_subtitles));
        p_text->SetValue(new_title, true, true);
    }

    // the overall "Sell x N" total on the sell menu entry
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SELL") + XString;
    new_title += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
    p_text = static_cast<CText*>(sold_pentry->FindChild("PC_TRADING_SELL_TEXT"));
    p_text->SetValue(new_title, true, true);

    if (items.second == 0) {
        // nothing left of this item: the entry goes, and its category list once that is empty too
        CDeletion deletion;
        deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
        pcomponent->SetDeletion(deletion);
        ItemsToSell.erase(pcomponent);
        if (NumSellableThings[sold_pparent].second <= 0) {
            sold_pparent->MakeUnavailableDynamically();
            SellListsByCategory.erase(sold_pparent);
        }
    } else {
        RefreshNameAndTitlePC(pcomponent, item_def_index, items.second);
    }

    // the sale itself: the shop buys the items (CTCShop::BuyItem 0x00774D20, Ego_r.pdb + FableWin.pdb) at its buy price, the hero is paid, the items leave the hero's category inventory
    CTCShop* ptc_shop = 0;
    CThingPlayerCreature* pplayer = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* ptc_hero_stats = 0;
    GetBuildingThing()->GetTC(TCI_SHOP, &ptc_shop);
    pplayer->GetTC(TCI_HERO_STATS, &ptc_hero_stats);
    long items_value = ptc_shop->GetCurrentBuyPrice(item_def_index, quantity);
    for (long count = 0; count < quantity; ++count)
        ptc_shop->BuyItem(item_def_index);
    ptc_hero_stats->AddMoney(items_value, true);
    NInventory::CTCInventoryBase* pinventory_item_is_in = 0;
    pplayer->GetTC(CTCInventoryItem::GetInventoryType(item_def_index), &pinventory_item_is_in);
    pinventory_item_is_in->RemoveItemFromInventory(item_def_index, quantity);
    UpdateSellActionsList();
    CComponent* pframesell = PCurrentOpenTradeMenu->FindChild("PC_UI_INACTIVE_FRAME");
    RefreshSellDescriptionsPC(pframesell);

    // recount what the hero could buy back: affordable, with room in the category inventory; the sold item joins the buy list
    TotNumItemsToBuy = 0;
    TotNumWeaponsToBuy = 0;
    TotNumClothesToBuy = 0;
    TotSpecialItemsToBuy = 0;
    bool found_sell = false;
    for (std::map<CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        if (item_def_index == it->second.first) {
            it->second.second += quantity;
            found_sell = true;
        }
        bool is_list_empty = it->second.second == 0;
        const NInventory::CTCInventoryBase* ptc_inventory_base = 0;
        if (!is_list_empty) {
            pplayer->PeekTC(CTCInventoryItem::GetInventoryType(it->second.first), &ptc_inventory_base);
        }
        bool can_buy = !is_list_empty && ptc_hero_stats->GetMoney() >= ptc_shop->GetCurrentSellPrice(it->second.first, 1)
            && ptc_inventory_base != 0 && ptc_inventory_base->IsSpaceInInventoryForItem(it->second.first);
        if (can_buy) {
                if (IsSpecialCase(it->second.first)) {
                    TotSpecialItemsToBuy += it->second.second;
                } else {
                    EItem_Menu_Type type = GetTypeFromDefIndexPC(it->second.first);
                    long num = it->second.second;
                    switch (type) {
                    case MENU_TYPE_ITEMS:   TotNumItemsToBuy += num;   break;
                    case MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += num; break;
                    case MENU_TYPE_CLOTHING: TotNumClothesToBuy += num; break;
                    }
                }
        }
    }
    if (!found_sell) {
        CComponent* dummy_pointer = (CComponent*)ItemsToBuy.size();
        std::pair<CComponent* const, std::pair<long, long> > value_pair(dummy_pointer, std::pair<long, long>(item_def_index, quantity));
        ItemsToBuy.insert(value_pair);
        EItem_Menu_Type type = GetTypeFromDefIndexPC(item_def_index);
        switch (type) {
        case MENU_TYPE_ITEMS:   TotNumItemsToBuy += quantity;   break;
        case MENU_TYPE_WEAPONS: TotNumWeaponsToBuy += quantity; break;
        case MENU_TYPE_CLOTHING: TotNumClothesToBuy += quantity; break;
        }
    }

    // the overall "Buy x N" total on the buy menu entry
    CComponent* buy_pparent = Components[0]->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    new_title = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_BUY") + XString;
    new_title += GFIntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    p_text = static_cast<CText*>(buy_pparent->FindChild("PC_TRADING_BUY_TEXT"));
    p_text->SetValue(new_title, true, true);

    // every gold readout on the open trade menu shows the hero's new purse
    std::list<CComponent*> result;
    PCurrentOpenTradeMenu->FindAllChildren("PC_GOLD_AMOUNT_TRADE_SCREEN", result);
    std::list<CComponent*>::iterator endit = result.end();
    for (std::list<CComponent*>::iterator it = result.begin(); it != endit; ++it) {
        CComponent* pgold = *it;
        if (pgold) {
            CText* pgold_real = static_cast<CText*>(pgold);
            pgold_real->SetValue(GFIntToWideString(ptc_hero_stats->GetMoney()), true, true);
        }
    }
    RefreshEntriesAmountsAndAvailabilitiesPC(PCurrentOpenTradeMenu, SellListsByCategory);
    CManager::GetInstance()->SetEvent(UI_EVENT_BACK);
}

static unsigned Rng = 12345;
static unsigned Next() { Rng = 1664525u * Rng + 1013904223u; return Rng >> 8; }
static std::string Num(long n) { char b[48]; sprintf(b, "%ld", n); return b; }
static void RunCase(bool forced, unsigned mask) {
    CTCInventoryTrade t;
    CComponent root, menu, sold_entry, buy_entry, item, branch, frame, other_entry;
    CText branch_text, cat_text[3], subtitle[3], sell_text, buy_text, gold[3], decoy;
    CList sell_list;
    const char* cat_names[3] = { "PC_TEXT_CLOTHING", "PC_TEXT_WEAPONS", "PC_TEXT_ITEMS" };
    const char* cat_heads[3] = { "Clothing", "Weapons", "Items" };
    const std::string sub_names[3] = { CLOTHES_SUBTITLE.Text, WEAPONS_SUBTITLE.Text, ITEMS_SUBTITLE.Text };
    t.Components[0] = &root; t.PCurrentOpenTradeMenu = &menu; t.Branch = &branch;
    root.Children["PC_MENU_ENTRY_TRADE_SELL"] = &sold_entry; root.Children["PC_MENU_ENTRY_TRADE_BUY"] = &buy_entry;
    sold_entry.Children["PC_TRADING_SELL_TEXT"] = &sell_text; buy_entry.Children["PC_TRADING_BUY_TEXT"] = &buy_text;
    branch.Children["Text"] = &branch_text;
    menu.Children["PC_UI_INACTIVE_FRAME"] = &frame;
    for (int k = 0; k < 3; ++k) { menu.Children[sub_names[k]] = &subtitle[k]; }
    // fixture draw
    unsigned r = forced ? mask : Next();
    int type = (int)(r % 4);                       // 3: a branch type with no switch arm
    t.BranchType = (CTCInventoryTrade::EItem_Menu_Type)type;
    bool with_cat = ((r >> 2) & 3) != 0;           // the category text exists (FindChild may return null)
    for (int k = 0; k < 3; ++k) menu.Children[cat_names[k]] = with_cat ? &cat_text[k] : 0;
    long quantity = 1 + (long)((r >> 4) % 4);
    long leftover = ((r >> 6) & 1) ? (long)((r >> 7) % 3) + 1 : 0;   // what stays on the entry
    long branch_other = ((r >> 9) & 1) ? (long)((r >> 10) % 4) + 1 : 0;
    long def = (long)((r >> 12) % 16);
    long money0 = (long)((r >> 16) % 40);
    for (int d = 0; d < 16; ++d) {
        unsigned f = Next();
        Defs[d].BuyPrice = 1 + (long)(f % 9); Defs[d].SellPrice = (long)((f >> 4) % 60); Defs[d].Type = (long)((f >> 10) % 4);
        Defs[d].Inv = (long)((f >> 12) % 3); Defs[d].Space = ((f >> 14) & 3) != 0; Defs[d].Special = ((f >> 16) & 3) == 0;
    }
    for (int k = 0; k < 3; ++k) InvPresent[k] = ((r >> (20 + k)) & 3) != 0 || forced;
    InvPresent[(r >> 23) % 3] = ((r >> 25) & 1) != 0 || forced;
    // gold readouts: up to three, a null entry, and a decoy of another name
    int ngold = (int)((r >> 26) % 4);
    for (int k = 0; k < ngold; ++k) { menu.All.push_back(std::make_pair(std::string("PC_GOLD_AMOUNT_TRADE_SCREEN"), (CComponent*)&gold[k])); if (k == 1) menu.All.push_back(std::make_pair(std::string("PC_GOLD_AMOUNT_TRADE_SCREEN"), (CComponent*)0)); }
    menu.All.push_back(std::make_pair(std::string("PC_GOLD_DECOY"), (CComponent*)&decoy));
    // maps
    t.ItemsToSell[&item] = std::make_pair(def, quantity + leftover);
    t.ItemsToSell[&other_entry] = std::make_pair((def + 1) % 16, 4L);
    t.NumSellableThings[&branch] = std::make_pair(CWideString("Branch"), quantity + leftover + branch_other);
    t.SellListsByCategory[&branch] = &sell_list;
    t.TotNumClothesToSell = 30 + (long)(r % 7); t.TotNumWeaponsToSell = 40 + (long)(r % 5); t.TotNumItemsToSell = 50 + (long)(r % 3);
    t.TotNumClothesToBuy = t.TotNumWeaponsToBuy = t.TotNumItemsToBuy = t.TotSpecialItemsToBuy = -777;
    static CComponent buy_keys[6];
    int nbuy = (int)(Next() % 6);
    bool has_def = ((r >> 28) & 1) != 0;
    long buy_def[6], buy_cnt[6];
    for (int k = 0; k < nbuy; ++k) {
        buy_def[k] = (has_def && k == 0) ? def : (long)(Next() % 16);
        buy_cnt[k] = (Next() % 3 == 0) ? 0 : (long)(Next() % 5) + 1;
        t.ItemsToBuy[&buy_keys[k]] = std::make_pair(buy_def[k], buy_cnt[k]);
    }
    Shop = CTCShop(); Stats = CTCHeroStats(); Stats.Money = money0;
    for (int k = 0; k < 3; ++k) Inventories[k] = NInventory::CTCInventoryBase();
    SellPriceQueries = SpaceQueries = SpecialQueries = TypeQueries = PeekQueries = ShopLookups = HeroLookups = InvLookups = 0; Events = 0;
    long cat0[3] = { t.TotNumClothesToSell, t.TotNumWeaponsToSell, t.TotNumItemsToSell };
    unsigned buy_size0 = (unsigned)t.ItemsToBuy.size();

    t.SellItemPC(&item, quantity);

    // ---- the model ----
    long left = leftover, branch_left = leftover + branch_other;
    long cat[3] = { cat0[0], cat0[1], cat0[2] };
    if (type < 3) cat[type] -= quantity;
    CHECK(t.TotNumClothesToSell == cat[0] && t.TotNumWeaponsToSell == cat[1] && t.TotNumItemsToSell == cat[2]);
    CHECK(t.BranchQueries == 1 && t.Subtitles == 1);
    CHECK(t.NumSellableThings[&branch].second == branch_left);
    CHECK(branch_text.Writes == 1 && branch_text.Value.Text == (branch_left > 0 ? "Branch x " + Num(branch_left) : std::string("Branch")));
    for (int k = 0; k < 3; ++k) {
        bool w = type == k && with_cat;
        std::string cap = std::string(cat_heads[k]) + " x " + Num(cat[k]);
        CHECK(cat_text[k].Writes == (w ? 1 : 0) && subtitle[k].Writes == (w ? 1 : 0));
        if (w) CHECK(cat_text[k].Value.Text == cap && subtitle[k].Value.Text == cap);
    }
    CHECK(sell_text.Writes == 1 && sell_text.Value.Text == "Sell x " + Num(cat[0] + cat[1] + cat[2]));
    if (left == 0) {
        CHECK(item.Deleted == 1 && t.ItemsToSell.find(&item) == t.ItemsToSell.end() && t.Names == 0);
        bool gone = branch_left <= 0;
        CHECK(branch.Unavailable == (gone ? 1 : 0) && (t.SellListsByCategory.find(&branch) == t.SellListsByCategory.end()) == gone);
    } else {
        CHECK(item.Deleted == 0 && t.ItemsToSell[&item].second == left && branch.Unavailable == 0 && t.SellListsByCategory.size() == 1);
        CHECK(t.Names == 1 && t.NameEntry == &item && t.NameDef == def && t.NameCount == left);
    }
    CHECK(t.ItemsToSell[&other_entry].second == 4 && other_entry.Deleted == 0);
    long price = Defs[def].BuyPrice * quantity, money = money0 + price;
    CHECK(Shop.BuyQueries == 1 && Shop.Sold == quantity && Shop.LastSold == def && Stats.Calls == 1 && Stats.Money == money);
    CHECK(ShopLookups == 1 && HeroLookups == 1 && InvLookups == 1);
    for (int k = 0; k < 3; ++k) {
        bool mine = k == Defs[def].Inv;
        CHECK(Inventories[k].Calls == (mine ? 1 : 0) && Inventories[k].Removed == (mine ? quantity : 0));
        if (mine) CHECK(Inventories[k].RemovedDef == def);
    }
    CHECK(t.SellUpdates == 1 && t.Descriptions == 1 && t.DescriptionFrame == &frame);
    long buy[3] = { 0, 0, 0 }, special = 0, sell_q = 0, space_q = 0, special_q = 0, type_q = 0, peek_q = 0;
    bool found = false;
    for (int k = 0; k < nbuy; ++k) {
        long d = buy_def[k], n = buy_cnt[k];
        if (d == def) { n += quantity; found = true; }
        CHECK(t.ItemsToBuy[&buy_keys[k]].first == d && t.ItemsToBuy[&buy_keys[k]].second == n);
        if (n == 0) continue;
        ++peek_q;
        ++sell_q;
        if (money < Defs[d].SellPrice || !InvPresent[Defs[d].Inv]) continue;
        ++space_q;
        if (!Defs[d].Space) continue;
        ++special_q;
        if (Defs[d].Special) { special += n; continue; }
        ++type_q;
        if (Defs[d].Type < 3) buy[Defs[d].Type] += n;
    }
    if (!found) {
        CHECK(t.ItemsToBuy.size() == buy_size0 + 1);
        CComponentPairMap::iterator it = t.ItemsToBuy.find((CComponent*)buy_size0);
        CHECK(it != t.ItemsToBuy.end() && it->second.first == def && it->second.second == quantity);
        ++type_q;
        if (Defs[def].Type < 3) buy[Defs[def].Type] += quantity;
    } else CHECK(t.ItemsToBuy.size() == buy_size0);
    CHECK(t.TotNumClothesToBuy == buy[0] && t.TotNumWeaponsToBuy == buy[1] && t.TotNumItemsToBuy == buy[2] && t.TotSpecialItemsToBuy == special);
    CHECK(PeekQueries == peek_q && SellPriceQueries == sell_q && SpaceQueries == space_q && SpecialQueries == special_q && TypeQueries == type_q);
    CHECK(buy_text.Writes == 1 && buy_text.Value.Text == "Buy x " + Num(buy[0] + buy[1] + buy[2]));
    CHECK(menu.FindAlls == 1 && decoy.Writes == 0);
    for (int k = 0; k < 3; ++k) { CHECK(gold[k].Writes == (k < ngold ? 1 : 0)); if (k < ngold) CHECK(gold[k].Value.Text == Num(money)); }
    CHECK(t.EntryRefreshes == 1 && Events == 1);
}
int main() {
    // every low-bit fixture combination (type, category text, quantity, leftover, branch rest), then random draws
    for (unsigned m = 0; m < (1u << 12); ++m) { g_case = (int)m; RunCase(true, m | ((m & 0x3F) << 26) | (m << 16)); }
    for (int c = 0; c < 20000; ++c) { g_case = 10000 + c; RunCase(false, 0); }
    puts("005b01fe_TEST PASS");
    return 0;
}