// CTCInventoryTrade::SellItem(NUISystem::CComponent* item, long count) -- sell `count` of a console-trade list item: update the sell list, category subtitles and totals, pay the hero, move the stock into the buy list and refresh the trade screen
// Grade: DIFFER(2446v2446, md=76, 52 bytes) -- frame 0x58 v retail 0x54: one 4-byte slot is not shared (ours packs the bottom CDeletion / CCountedPointer / CCharString temporaries 4 bytes deeper: retail [ebp-0x54..-0x48], ours [ebp-0x58..-0x4c]); every remaining line is that shift
// Next idea: VC7.1 shares an address-taken slot only when every callee the address reaches is visible in the TU and does not capture it (micro probes m5/m6): the same-TU STLport map bodies (md 349 -> 80) and the landed GetMenuBranchFromDefIndex body (menu_type slot, -> 76) are that lever; the last slot is probably another earlier-defined TCInventoryTrade.cpp callee taking an address (RefreshNameAndTitle / RefreshSubtitles / AddBuyItemToList / RefreshEntriesAmountsAndAvailabilities, all landed): add their bodies above SellItem the same way
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
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
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
    _Rb_tree_node_base* _M_lower_bound(const key_type& __k) const;
    _Rb_tree_node_base* _M_upper_bound(const key_type& __k) const;
    _Rb_tree_node_base* _M_find(const key_type& __k);
    iterator find(const key_type& __k) { return iterator(_M_find(__k)); }
    iterator insert(iterator __position, const value_type& __x);
    void erase(iterator __first, iterator __last);
    unsigned int erase(const key_type& __k);
};

#pragma auto_inline(off)
template <class _Key, class _Tp> _Rb_tree_node_base* map<_Key, _Tp>::_M_lower_bound(const _Key& __k) const
{
    _Rb_tree_node_base* __y = _M_header;
    _Rb_tree_node_base* __x = _M_header->_M_parent;
    while (__x != 0)
        if (!(((_Rb_tree_node<value_type>*)__x)->_M_value_field.first < __k)) { __y = __x; __x = __x->_M_left; }
        else __x = __x->_M_right;
    return __y;
}
template <class _Key, class _Tp> _Rb_tree_node_base* map<_Key, _Tp>::_M_upper_bound(const _Key& __k) const
{
    _Rb_tree_node_base* __y = _M_header;
    _Rb_tree_node_base* __x = _M_header->_M_parent;
    while (__x != 0)
        if (__k < ((_Rb_tree_node<value_type>*)__x)->_M_value_field.first) { __y = __x; __x = __x->_M_left; }
        else __x = __x->_M_right;
    return __y;
}
template <class _Key, class _Tp> _Rb_tree_node_base* map<_Key, _Tp>::_M_find(const _Key& __k)
{
    _Rb_tree_node_base* __y = _M_lower_bound(__k);
    if (__y == _M_header || __k < ((_Rb_tree_node<value_type>*)__y)->_M_value_field.first) __y = _M_header;
    return __y;
}
template <class _Key, class _Tp> _Tp& map<_Key, _Tp>::operator[](const _Key& __k)
{
    iterator __i(_M_lower_bound(__k));
    if (__i == end() || __k < (*__i).first) {
        __i = insert(__i, value_type(__k, _Tp()));
    }
    return (*__i).second;
}
template <class _Key, class _Tp> unsigned int map<_Key, _Tp>::erase(const _Key& __k)
{
    iterator __first(_M_lower_bound(__k)), __last(_M_upper_bound(__k));
    unsigned int __n = 0;
    for (iterator __it = __first; __it != __last; ++__it) ++__n;
    erase(__first, __last);
    return __n;
}
#pragma auto_inline(on)

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

#include "engine/ETCInterfaceType.h"

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
    template <class T> __forceinline bool GetTC(ETCInterfaceType id, T** ptc)
    {
        if (InterfacePresence.test(id)) {
            ETCInterfaceType type = id;
            CInterfaceMap* map = &Interfaces;
            CInterfaceEntry* it = map->LowerBound(type);
            *ptc = (T*)((it == map->End || id < it->Type) ? map->End : it)->Interface;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool PeekTC(ETCInterfaceType id, const T** ptc) const
    {
        if (InterfacePresence.test(id)) {
            ETCInterfaceType type = id;
            const CInterfaceMap* map = &Interfaces;
            const CInterfaceEntry* it = map->LowerBound(type);
            *ptc = (const T*)((it == map->End || id < it->Type) ? map->End : it)->Interface;
            return true;
        }
        return false;
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
    long GetMoney() const { return Money; }
    void AddMoney(long amount, bool show);                                             // 0x0057B327
};

namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
    bool IsSpaceInInventoryForItem(long def_index) const;                                                   // 0x005BF2ED
};
}

class CTCInventoryItem {
public:
    static long __fastcall GetInventoryCategory(long def_index);           // 0x005D99D0
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

    CThing* GetTradingThing();                                               // 0x005BC4C4
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

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;                     // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                   // 0x0044C6B0
};
long __fastcall GetInventoryCategory(long def_index);                                 // 0x005D97B0
long __fastcall GetInventoryTabState(long def_index);                                 // 0x005D96F0

#pragma auto_inline(off)
NUISystem::CComponent* CTCInventoryTrade::GetMenuBranchFromDefIndex(long def_index, NUISystem::CComponent* menu, EItem_Menu_Type& type)
{
    long melee, ranged, headwear, torso, gloves, legwear, footwear, suits;
    { CCharString name("INVENTORY_CATEGORY_FOOD", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_MELEE_WEAPONS", -1); melee = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_RANGED_WEAPONS", -1); ranged = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_HEADWEAR", -1); headwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_TORSO_WEAR", -1); torso = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_GLOVES", -1); gloves = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_LEGWEAR", -1); legwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_FOOTWEAR", -1); footwear = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_CLOTHES_SUITS", -1); suits = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_GIFTS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_TROPHIES", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    { CCharString name("INVENTORY_CATEGORY_POTIONS", -1); CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name); }
    NUISystem::CComponent* branch;
    long category = GetInventoryCategory(def_index);
    if (category == melee) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_MELEE_ENTRY", -1)); type = ITEM_MENU_WEAPONS; }
    else if (category == ranged) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_RANGED_ENTRY", -1)); type = ITEM_MENU_WEAPONS; }
    else if (category == headwear) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_HEAD_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (category == torso) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_TORSO_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (category == gloves) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_HANDS_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (category == legwear) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_LEGS_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (category == footwear) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_FEET_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (category == suits) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_SUITS_ENTRY", -1)); type = ITEM_MENU_CLOTHING; }
    else if (GetInventoryTabState(def_index) == 0) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_TROPHIES_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    else if (GetInventoryTabState(def_index) == 1) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_POTIONS_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    else if (GetInventoryTabState(def_index) == 2) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_GIFTS_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    else if (GetInventoryTabState(def_index) == 3) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_PRODUCE_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    else if (GetInventoryTabState(def_index) == 4) { branch = menu->FindChild(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    else { branch = menu->FindChild(CCharString("UI_TRADING_MENU_OTHERS_ENTRY", -1)); type = ITEM_MENU_ITEMS; }
    return branch;
}
#pragma auto_inline(on)

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
        text = text_bank->GetTextBySymbol("TEXT_GUI_MENU_CLOTHING") + x_string;
        amount_name = g_SellClothingAmountName;
        break;
    case ITEM_MENU_WEAPONS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_WEAPONS_TEXT"));
        category_total = TotNumWeaponsToSell -= count;
        text = text_bank->GetTextBySymbol("TEXT_GUI_MENU_WEAPONS") + x_string;
        amount_name = g_SellWeaponsAmountName;
        break;
    case ITEM_MENU_ITEMS:
        category_text = sell_menu->FindChild(CCharString("UI_TRADING_ITEMS_TEXT"));
        category_total = TotNumItemsToSell -= count;
        text = text_bank->GetTextBySymbol("TEXT_GUI_MENU_ITEMS") + x_string;
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
    text = text_bank->GetTextBySymbol("TEXT_GUI_MENU_SELL") + x_string;
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

    {
    CTCShop* shop = 0;
    CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer()->GetPControlledCreature();
    CTCHeroStats* hero_stats = 0;
    GetTradingThing()->GetTC(TCI_SHOP, &shop);
    hero->GetTC(TCI_HERO_STATS, &hero_stats);
    long price = shop->GetCurrentBuyPrice(def_index, count);
    for (long i = 0; i < count; ++i)
        shop->AddItemSold(def_index);
    hero_stats->AddMoney(price, true);
    CTCBase* inventory = 0;
    hero->GetTC((ETCInterfaceType)CTCInventoryItem::GetInventoryCategory(def_index), &inventory);
    inventory->RemoveItemFromInventory(def_index, count);

    for (CComponentPairMap::iterator it = ItemsToBuy.begin(); it != ItemsToBuy.end(); ++it) {
        if ((*it).second.second > 0) {
            const NInventory::CTCInventoryBase* inventory = 0;
            hero->PeekTC((ETCInterfaceType)CTCInventoryItem::GetInventoryCategory((*it).second.first), &inventory);
            if ((*it).first->FindChild(CCharString("UI_LIST_BUY_ACTIONS")) == 0
                && shop->GetCurrentSellPrice((*it).second.first, 1) <= hero_stats->GetMoney()
                && (IsSpecialCase((*it).second.first) || inventory->IsSpaceInInventoryForItem((*it).second.first))) {
                (*it).first->MakeAvailableDynamically();
                CCountedPointer<NUISystem::CComponent> arrow(
                    NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
                (*it).first->AddChildDynamically(arrow);
            }
        }
    }

    EItem_Menu_Type buy_type;
    NUISystem::CComponent* buy_menu = Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY"));
    NUISystem::CComponent* buy_branch = GetMenuBranchFromDefIndex(def_index, buy_menu, buy_type);
    CComponentPairMap::iterator buy_it;
    for (buy_it = ItemsToBuy.begin(); buy_it != ItemsToBuy.end(); ++buy_it) {
        if (def_index == (*buy_it).second.first) {
            long old_count = (*buy_it).second.second;
            (*buy_it).second.second += count;
            if (old_count == 0) {
                (*buy_it).first->MakeAvailableDynamically();
                CCountedPointer<NUISystem::CComponent> arrow(
                    NUISystem::CManager::GetInstance()->CreateComponent("UI_SELECTION_ARROW_RIGHT", false));
                (*buy_it).first->AddChildDynamically(arrow);
            }
            RefreshNameAndTitle((*buy_it).first, (*buy_it).second.first, (*buy_it).second.second);
            goto refresh;
        }
    }
    {
        CComponentListMap::iterator list_it = BuyListsByCategory.find(buy_branch);
        if (list_it != BuyListsByCategory.end())
            AddBuyItemToList(def_index, count, (*list_it).second, text_area, true);
        else {
            CCountedPointer<NUISystem::CComponent> sub_list(
                NUISystem::CManager::GetInstance()->CreateComponent("UI_SUB_LIST", false));
            NUISystem::CComponent* list = sub_list.Get();
            list->SetItemCount(1);
            AddBuyItemToList(def_index, count, (NUISystem::CList*)list, text_area, false);
            buy_branch->AddChildDynamically(sub_list);
            BuyListsByCategory[buy_branch] = (NUISystem::CList*)list;
        }
    }
refresh:
    UpdateBuyActionsList();
    UpdateSellActionsList();
    RefreshDescriptions();
    RefreshBuyPossibilityNumbers();
    NUISystem::CComponent* gold = Components[0]->FindChild(CCharString("UI_GOLD_AMOUNT_TRADE_SCREEN"));
    if (gold != 0)
        gold->SetText(GFIntToWideString(hero_stats->GetMoney()), true, true);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_BUY_ENTRY")), BuyListsByCategory);
    RefreshEntriesAmountsAndAvailabilities(Components[0]->FindChild(CCharString("UI_TRADING_MENU_SELL_ENTRY")), SellListsByCategory);
    NUISystem::CManager::GetInstance()->Notify(5);
    }
}
