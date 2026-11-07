// CTCInventoryTrade::SellItem(NUISystem::CComponent* item, long count) -- sell `count` of a console-trade list item: update the sell list, category subtitles and totals, pay the hero, move the stock into the buy list and refresh the trade screen
// Grade: DIFFER(2217v2446, md=429) -- frame 0x68 vs retail 0x54: retail shares stack slots between the scoped temporaries and reuses the item / count argument slots for the shop pointer and later CCharString temporaries; call order and branch layout otherwise follow retail
// Next idea: scope each CDeletion / CCountedPointer / CCharString temporary to retail's block, reuse `item` for the shop lookup key as retail does ([ebp+8]), and spell the static amount names through their getters
// Leaf: ?SellItem@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@J@Z
#pragma optimize("s", on)
#pragma optimize("y", off)

class CCharString {
public:
    void* PStringData;
    CCharString();                                                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);                                  // 0x0099EFB0
};

class CWideString {
public:
    void* PStringData;
    CWideString();                                                                     // 0x0099AED0
    CWideString(const CWideString& other);                                             // 0x0099B720
    ~CWideString();                                                                    // 0x0099B510
    const CWideString& operator=(const CWideString& other);                            // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);                           // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);  // 0x0099BE70
CWideString __fastcall GFIntToWideString(long value);                                 // 0x00405F00

class CTCInventoryStats {
public:
    static CWideString __fastcall GetXString();                                        // 0x005CC772
};

namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;                // 0x009C95E0
};
}

#include "engine/CCountedPointer.h"

namespace NUISystem { class CComponent; class CList; class CViewport; }

namespace std {

template <class _Tp> class list {
public:
    void* _M_node;
    ~list();                                                                           // 0x0042ABCA
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x);                  // 0x004292C0
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Tp> class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned char       _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    _Tp& operator[](const key_type& __k);
    iterator find(const key_type& __k);
    unsigned int erase(const key_type& __k);
};

}

namespace NUISystem {

enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };
enum EType { TYPE_0, TYPE_1, TYPE_2, TYPE_3, TYPE_4, TYPE_5, TYPE_6 };

class CDeletion {
public:
    EDeletionMethod                 Method;
    std::list<CComponent*>          AssociatedParents;
    CDeletion();                                                                       // 0x0042BED4
    CDeletion(const CDeletion& other);                                                 // 0x0042CD84
};

class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                                   // slot 3 (+0x0c)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual CComponent* FindFirstChildOfType(EType type);                             // slot 9 (+0x24)
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059();
    virtual void AddChildDynamically(CCountedPointer<CComponent>& child);              // slot 60 (+0xf0)
    virtual void v061();
    virtual void SetDeletion(CDeletion deletion);                                      // slot 62 (+0xf8)
    virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void MakeUnavailableDynamically();                                         // slot 93 (+0x174)
    virtual void v094();
    virtual void MakeAvailableDynamically();                                           // slot 95 (+0x17c)
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);                     // slot 144 (+0x240)
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void SetItemCount(long count);                                             // slot 156 (+0x270)
};

class CManager {
public:
    virtual void Notify(long event);                                                   // slot 0
    static CManager* __fastcall GetInstance();                                         // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, bool flag);                  // 0x0041DB1D
};

}

enum ETCInterfaceType { TCI_INVENTORY = 4, TCI_SHOP = 0x53 };

namespace std {
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                                               // 0x00410DE0
};
}

class CTCBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078();
    virtual void RemoveItemFromInventory(long def_index, long count);                  // slot 79 (+0x13c)
};

struct CInterfaceEntry { ETCInterfaceType Type; CTCBase* Interface; };
class CInterfaceMap {
public:
    CInterfaceEntry* Begin;
    CInterfaceEntry* End;
    CInterfaceEntry* LowerBound(const ETCInterfaceType& type);                         // 0x0040F020
    const CInterfaceEntry* LowerBound(const ETCInterfaceType& type) const;             // 0x004365B0
};

class CThing {
public:
    unsigned char           _pad_0x00[0x20];
    std::bitset<274>        InterfacePresence;                                         // +0x20
    CInterfaceMap           Interfaces;                                                // +0x44
    CTCBase* GetInterface(ETCInterfaceType type)
    {
        if (!InterfacePresence.test(type))
            return 0;
        CInterfaceEntry* it = Interfaces.LowerBound(type);
        if (it == Interfaces.End || type < it->Type)
            it = Interfaces.End;
        return it->Interface;
    }
};

class CPlayer { public: CThing* GetPControlledCreature(); };                         // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };                            // 0x00449960
class CMainGameComponent {
public:
    unsigned char             _pad_0x00[0x14];
    NGameText::CDataBank*     TextBank;                                                // +0x14
    unsigned char             _pad_0x18[0x1c - 0x18];
    CPlayerManager*           PlayerManager;                                           // +0x1c
    static CMainGameComponent* PMainGameComponent;                                     // 0x013B86A0
    static CMainGameComponent* Get() { return PMainGameComponent; }
};

class CTCShop : public CTCBase {
public:
    long GetCurrentBuyPrice(long def_index, long count);                               // 0x007742C0
    long GetCurrentSellPrice(long def_index, long count);                              // 0x007743B0
    void AddItemSold(long def_index);                                                  // 0x00774D20
};

class CTCHeroStats : public CTCBase {
public:
    unsigned char _pad_0x04[0x3c - 0x04];
    long          Money;                                                               // +0x3c
    void AddMoney(long amount, bool show);                                             // 0x0057B327
};

namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
    bool CanAddItem(long def_index);                                                   // 0x005BF2ED
};
}

class CTCInventoryItem {
public:
    static ETCInterfaceType __fastcall GetInventoryCategory(long def_index);           // 0x005D99D0
};

enum EItem_Menu_Type { ITEM_MENU_CLOTHING, ITEM_MENU_WEAPONS, ITEM_MENU_ITEMS };

typedef std::map<NUISystem::CComponent*, std::pair<long, long> > CComponentPairMap;
typedef std::map<NUISystem::CComponent*, std::pair<CWideString, long> > CComponentStringMap;
typedef std::map<NUISystem::CComponent*, NUISystem::CList*> CComponentListMap;

extern CCharString g_SellClothingAmountName;                                          // 0x013B8B74
extern CCharString g_SellWeaponsAmountName;                                           // 0x013B8B70
extern CCharString g_SellItemsAmountName;                                             // 0x013B8B78
extern CCharString g_SellTotalAmountName;                                             // 0x013B8B68

class CTCInventoryTrade {
public:
    void*                   __vftable;                                                 // +0x000
    unsigned char           _pad_0x004[0x00c - 0x004];
    CCountedPointer<NUISystem::CComponent>* Components;                                // +0x00c (vector _M_start)
    unsigned char           _pad_0x010[0x144 - 0x010];
    CComponentPairMap       ItemsToBuy;                                                // +0x144
    CComponentPairMap       ItemsToSell;                                               // +0x150
    unsigned char           _pad_0x15c[0x180 - 0x15c];
    CComponentListMap       SellListsByCategory;                                       // +0x180
    CComponentListMap       BuyListsByCategory;                                        // +0x18c
    unsigned char           _pad_0x198[0x1bc - 0x198];
    CComponentStringMap     NumSellableThings;                                         // +0x1bc
    unsigned char           _pad_0x1c8[0x1d4 - 0x1c8];
    long                    TotNumClothesToSell;                                       // +0x1d4
    long                    TotNumWeaponsToSell;                                       // +0x1d8
    long                    TotNumItemsToSell;                                         // +0x1dc

    CTCBase* GetTradingThingInterface();                                               // 0x005BC4C4
    NUISystem::CComponent* GetMenuBranchFromDefIndex(long def_index, NUISystem::CComponent* menu, EItem_Menu_Type& type);   // 0x0059B79A
    void RefreshSubtitles(NUISystem::CComponent* component, CComponentStringMap& subtitles);   // 0x0059E108
    void RefreshNameAndTitle(NUISystem::CComponent* component, long def_index, long count);    // 0x0059CFBD
    bool IsSpecialCase(long def_index);                                                // 0x0059C983
    bool AddBuyItemToList(long def_index, long count, NUISystem::CList* list, NUISystem::CViewport* viewport, bool refresh);   // 0x0059FEE3
    void UpdateBuyActionsList();                                                       // 0x0059DE04
    void UpdateSellActionsList();                                                      // 0x0059DE76
    void RefreshDescriptions();                                                        // 0x005AE6AF
    void RefreshBuyPossibilityNumbers();                                               // 0x005A3734
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilities(NUISystem::CComponent* component,
                                                                                   CComponentListMap& lists);   // 0x0059DB82
    void SellItem(NUISystem::CComponent* item, long count);
};

void CTCInventoryTrade::SellItem(NUISystem::CComponent* item, long count)
{
    NGameText::CDataBank* text_bank = CMainGameComponent::Get()->TextBank;
    std::pair<long, long>& sell_entry = ItemsToSell[item];
    long def_index = sell_entry.first;
    CWideString x_string = CTCInventoryStats::GetXString();
    sell_entry.second -= count;
    NUISystem::CComponent* sell_menu = Components[0]->FindChild(CCharString("UI_TRADING_MENU_SELL_ENTRY"));
    EItem_Menu_Type menu_type;
    NUISystem::CComponent* branch = GetMenuBranchFromDefIndex(def_index, sell_menu, menu_type);
    NumSellableThings[branch].second -= count;
    NUISystem::CComponent* branch_text = branch->FindFirstChildOfType(NUISystem::TYPE_6);
    CWideString subtitle(NumSellableThings[branch].first);
    if (NumSellableThings[branch].second > 0) {
        subtitle += CTCInventoryStats::GetXString();
        subtitle += GFIntToWideString(NumSellableThings[branch].second);
    }
    branch_text->SetText(subtitle, true, true);
    RefreshSubtitles(sell_menu->FindChild(CCharString("UI_TRADING_GOODS_LIST")), NumSellableThings);

    CWideString text;
    CCharString amount_name;
    NUISystem::CComponent* category_text;
    long category_total;
    switch (menu_type) {
    case ITEM_MENU_CLOTHING:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_CLOTHING_TEXT"));
        category_total = TotNumClothesToSell -= count;
        text = text_bank->GetTextBySymbol(CCharString("TEXT_GUI_MENU_CLOTHING")) + x_string;
        amount_name = g_SellClothingAmountName;
        break;
    case ITEM_MENU_WEAPONS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_WEAPONS_TEXT"));
        category_total = TotNumWeaponsToSell -= count;
        text = text_bank->GetTextBySymbol(CCharString("TEXT_GUI_MENU_WEAPONS")) + x_string;
        amount_name = g_SellWeaponsAmountName;
        break;
    case ITEM_MENU_ITEMS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_ITEMS_TEXT"));
        category_total = TotNumItemsToSell -= count;
        text = text_bank->GetTextBySymbol(CCharString("TEXT_GUI_MENU_ITEMS")) + x_string;
        amount_name = g_SellItemsAmountName;
        break;
    default:
        goto totals;
    }
    if (category_text != 0) {
        text += GFIntToWideString(category_total);
        category_text->SetText(text, true, true);
        sell_menu->FindChild(amount_name)->SetText(text, true, true);
    }
totals:
    text = text_bank->GetTextBySymbol(CCharString("TEXT_GUI_MENU_SELL")) + x_string;
    text += GFIntToWideString(TotNumItemsToSell + TotNumWeaponsToSell + TotNumClothesToSell);
    sell_menu->FindChild(CCharString("UI_TRADING_SELL_TEXT"))->SetText(text, true, true);
    sell_menu->FindChild(g_SellTotalAmountName)->SetText(text, true, true);

    NUISystem::CViewport* text_area = (NUISystem::CViewport*)Components[0]->FindChild(CCharString("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if (sell_entry.second == 0) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::DELETION_METHOD_1;
        item->SetDeletion(deletion);
        ItemsToSell.erase(item);
        if (NumSellableThings[branch].second <= 0) {
            branch->MakeUnavailableDynamically();
            NUISystem::CComponent* arrow = branch->FindChild(CCharString("UI_SELECTION_ARROW_RIGHT"));
            if (arrow != 0) {
                NUISystem::CDeletion arrow_deletion;
                arrow_deletion.Method = NUISystem::DELETION_METHOD_1;
                arrow->SetDeletion(arrow_deletion);
            }
            SellListsByCategory.erase(branch);
        }
    } else
        RefreshNameAndTitle(item, def_index, sell_entry.second);

    CTCShop* shop = 0;
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer()->GetPControlledCreature();
    CTCHeroStats* hero_stats = 0;
    CThing* trader = (CThing*)GetTradingThingInterface();
    shop = (CTCShop*)trader->GetInterface(TCI_SHOP);
    hero_stats = (CTCHeroStats*)hero->GetInterface(TCI_INVENTORY);
    long price = shop->GetCurrentBuyPrice(def_index, count);
    for (long i = 0; i < count; ++i)
        shop->AddItemSold(def_index);
    hero_stats->AddMoney(price, true);
    hero->GetInterface(CTCInventoryItem::GetInventoryCategory(def_index))->RemoveItemFromInventory(def_index, count);

    for (CComponentPairMap::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        if ((*it).second.second > 0) {
            NInventory::CTCInventoryBase* inventory =
                (NInventory::CTCInventoryBase*)hero->GetInterface(CTCInventoryItem::GetInventoryCategory((*it).second.first));
            if ((*it).first->FindChild(CCharString("UI_LIST_BUY_ACTIONS")) == 0
                && shop->GetCurrentSellPrice((*it).second.first, 1) <= hero_stats->Money
                && (IsSpecialCase((*it).second.first) || inventory->CanAddItem((*it).second.first))) {
                (*it).first->MakeAvailableDynamically();
                CCountedPointer<NUISystem::CComponent> arrow(
                    NUISystem::CManager::GetInstance()->CreateComponent(CCharString("UI_SELECTION_ARROW_RIGHT"), false));
                (*it).first->AddChildDynamically(arrow);
            }
        }
    }

    EItem_Menu_Type buy_type;
    NUISystem::CComponent* buy_branch =
        GetMenuBranchFromDefIndex(def_index, Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY")), buy_type);
    CComponentPairMap::iterator buy_it = ItemsToBuy.begin();
    for (; buy_it != ItemsToBuy.end(); ++buy_it)
        if (def_index == (*buy_it).second.first)
            break;
    if (buy_it != ItemsToBuy.end()) {
        long old_count = (*buy_it).second.second;
        (*buy_it).second.second += count;
        if (old_count == 0) {
            (*buy_it).first->MakeAvailableDynamically();
            CCountedPointer<NUISystem::CComponent> arrow(
                NUISystem::CManager::GetInstance()->CreateComponent(CCharString("UI_SELECTION_ARROW_RIGHT"), false));
            (*buy_it).first->AddChildDynamically(arrow);
        }
        RefreshNameAndTitle((*buy_it).first, (*buy_it).second.first, (*buy_it).second.second);
    } else {
        CComponentListMap::iterator list_it = BuyListsByCategory.find(buy_branch);
        if (list_it != BuyListsByCategory.end())
            AddBuyItemToList(def_index, count, (*list_it).second, text_area, true);
        else {
            CCountedPointer<NUISystem::CComponent> sub_list(
                NUISystem::CManager::GetInstance()->CreateComponent(CCharString("UI_SUB_LIST"), false));
            NUISystem::CComponent* list = sub_list.Get();
            list->SetItemCount(1);
            AddBuyItemToList(def_index, count, (NUISystem::CList*)list, text_area, false);
            buy_branch->AddChildDynamically(sub_list);
            BuyListsByCategory[buy_branch] = (NUISystem::CList*)list;
        }
    }
    UpdateBuyActionsList();
    UpdateSellActionsList();
    RefreshDescriptions();
    RefreshBuyPossibilityNumbers();
    NUISystem::CComponent* gold = Components[0]->FindChild(CCharString("UI_GOLD_AMOUNT_TRADE_SCREEN"));
    if (gold != 0)
        gold->SetText(GFIntToWideString(hero_stats->Money), true, true);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY")), BuyListsByCategory);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_SELL_ENTRY")), SellListsByCategory);
    NUISystem::CManager::GetInstance()->Notify(5);
}
