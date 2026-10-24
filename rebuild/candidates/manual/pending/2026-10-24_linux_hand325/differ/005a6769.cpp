// CTCInventoryTrade::ConstructNormalShopListPC(const std::vector<std::pair<long,long> >& buyItems, const std::vector<std::pair<long,long> >& sellItems) -- create the PC top-level trade list and fill its sell / buy / wanted entries with the localised caption plus the number of clothes, weapons and other items in each category (sellable stock the shop deals in, buyable items the hero can afford and carry, the shop's wanted list); returns the list
// Grade: DIFFER(1336v1336) -- operand order only: the wanted total loads TotNumWeaponsWanted then adds Items, Clothes in retail (as the matched sell/buy totals); ours loads Items first whatever the source order (tried W+I+C, I+W+C, C+W+I, `+= 1` counters)
// Leaf: ConstructNormalShopListPC@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A6769, 1336 bytes, `ret 8`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, component slots 3 (+0x0C, FindChild by value)
//   and 144 (+0x240, SetText), CTCInventoryBase::GetTradingThing 0x005BC4C4, std::bitset::test 0x00410DE0 + LowerBound 0x0040F020
//   (shop, type 0x53) / const 0x004365B0 (the hero's tab, keyed by GetInventoryCategory 0x005D99D0), CTCShop::HasDefaultStockItem
//   0x007723D0 / GetWantedItemsList 0x00774100 (vector<long> by hidden pointer), CTCInventoryTrade::GetTypeFromDefIndexPC
//   0x0059C447 / IsItemAffordable 0x0059DA10 / IsSpecialCase 0x0059C983 / AlreadyHaveSpecial 0x0059FC17, the tab's
//   IsItemAllowed 0x005BF2ED, CPlayerManager::GetMainPlayer 0x00449960 + GetControlledCreature 0x00487DD0,
//   CDataBank::GetTextBySymbol 0x009C95E0, GetXString 0x005CC772, IntToWideString 0x00405F00, CWideString operator+
//   0x0099BE70 / operator= 0x0099B7D0 / += 0x0099B8D0 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0, free 0x00BFEA14.
// Shape: the landed ConstructSpecialShopListPC 0x005A8A5D model (same TU): the text bank is read first (before the register
//   pushes), the front end goes through a named local; the per-category totals are zeroed by chained assignment (last
//   member first) and bumped through a three-case switch on the item type; the buy section only probes the shop's presence
//   bit (a bare test); the sell caption's " x " suffix is a named GetXString local, the wanted one a temporary; the wanted loop sits
//   in an `if (!wanted.empty())` guard (VC threads the guard into the loop test).
// Layout: retail CTCInventoryTrade (landed ctor 0x0059EC48): NotInStock +0x140, TotNumClothes/Weapons/ItemsToSell
//   +0x1D4/+0x1D8/+0x1DC, ToBuy +0x1E0/+0x1E4/+0x1E8, Wanted +0x1EC/+0x1F0/+0x1F4.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GetXString();                                                // 0x005CC772
CWideString __fastcall IntToWideString(long value);                                 // 0x00405F00
CWideString __fastcall operator+(const CWideString& left, const CWideString& right); // 0x0099BE70
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CPresenceBits { bool test(long kind) const; };  // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    bool HasInterface(long kind) const { return m_present.test(kind); }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
namespace std {
template <class A, class B> struct pair { A first; B second; };
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector() { if (_M_start) free(_M_start); }
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
}
struct CTCShop {                                        // interface type 0x53
    bool HasDefaultStockItem(long defIndex);            // 0x007723D0
    std::vector<long> GetWantedItemsList() const;       // 0x00774100
};
struct CTCInventoryTab {                                // the hero's inventory component for an item category
    bool IsItemAllowed(long defIndex);                  // 0x005BF2ED
};
struct CPlayer { CThing* GetControlledCreature(); };  // 0x00487DD0
struct CPlayerManager { CPlayer* GetMainPlayer() const; };   // 0x00449960
struct CMainGameComponent {
    unsigned char   _pad00[0x14];
    CDataBank*      TextBank;                           // +0x14
    unsigned char   _pad18[0x1c - 0x18];
    CPlayerManager* PlayerManager;                      // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
long __fastcall GetInventoryCategory(long defIndex);  // 0x005D99D0
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
struct CFrontEndManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
struct SListNode { SListNode* _M_next; SListNode* _M_prev; };
struct CLongList {
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
typedef std::vector<std::pair<long, long> > CItemVector;
struct CTCInventoryTrade {
    unsigned char _pad000[0x140];
    CLongList     NotInStock;                           // +0x140
    unsigned char _pad144[0x1d4 - 0x144];
    long TotNumClothesToSell;                           // +0x1d4
    long TotNumWeaponsToSell;                           // +0x1d8
    long TotNumItemsToSell;                             // +0x1dc
    long TotNumClothesToBuy;                            // +0x1e0
    long TotNumWeaponsToBuy;                            // +0x1e4
    long TotNumItemsToBuy;                              // +0x1e8
    long TotNumClothesWanted;                           // +0x1ec
    long TotNumWeaponsWanted;                           // +0x1f0
    long TotNumItemsWanted;                             // +0x1f4
    CThing* GetTradingThing();                          // 0x005BC4C4 (CTCInventoryBase)
    long GetTypeFromDefIndexPC(long defIndex);          // 0x0059C447
    bool IsSpecialCase(long defIndex);                  // 0x0059C983
    bool IsItemAffordable(long defIndex, long count);   // 0x0059DA10
    bool AlreadyHaveSpecial(long defIndex);             // 0x0059FC17
    NUISystem::CComponent* ConstructNormalShopListPC(const CItemVector& buyItems, const CItemVector& sellItems);
};
NUISystem::CComponent* CTCInventoryTrade::ConstructNormalShopListPC(const CItemVector& buyItems, const CItemVector& sellItems)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* list = frontEnd->CreateComponent("PC_TOP_LEVEL_TRADE_LIST", false);
    NUISystem::CComponent* entry = list->FindChild("PC_MENU_ENTRY_TRADE_SELL");
    TotNumClothesToSell = TotNumWeaponsToSell = TotNumItemsToSell = 0;
    if (!sellItems.empty()) {
        CTCShop* shop = 0;
        GetTradingThing()->GetInterface(&shop, 0x53);
        for (const std::pair<long, long>* it = sellItems.begin(); it != sellItems.end(); ++it) {
            if (shop->HasDefaultStockItem(it->first)) {
                switch (GetTypeFromDefIndexPC(it->first)) {
                case 0: TotNumClothesToSell += it->second; break;
                case 1: TotNumWeaponsToSell += it->second; break;
                case 2: TotNumItemsToSell += it->second; break;
                }
            }
        }
    }
    CWideString x = GetXString();
    CWideString caption = bank->GetTextBySymbol("TEXT_GUI_MENU_SELL") + x;
    caption += IntToWideString(TotNumWeaponsToSell + TotNumItemsToSell + TotNumClothesToSell);
    entry->FindChild("PC_TRADING_SELL_TEXT")->SetText(caption, true, true);
    entry = list->FindChild("PC_MENU_ENTRY_TRADE_BUY");
    TotNumClothesToBuy = TotNumWeaponsToBuy = TotNumItemsToBuy = 0;
    if (!buyItems.empty() || !NotInStock.empty()) {
        GetTradingThing()->HasInterface(0x53);
        for (const std::pair<long, long>* it = buyItems.begin(); it != buyItems.end(); ++it) {
            long defIndex = it->first;
            long count = it->second;
            CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetControlledCreature();
            CTCInventoryTab* tab = 0;
            static_cast<const CThing*>(hero)->GetInterface(&tab, GetInventoryCategory(defIndex));
            if (IsItemAffordable(defIndex, 1) && (tab->IsItemAllowed(defIndex) || IsSpecialCase(defIndex)) && count > 0
                && !AlreadyHaveSpecial(defIndex)) {
                switch (GetTypeFromDefIndexPC(defIndex)) {
                case 0: TotNumClothesToBuy += it->second; break;
                case 1: TotNumWeaponsToBuy += it->second; break;
                case 2: TotNumItemsToBuy += it->second; break;
                }
            }
        }
    }
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_BUY") + x;
    caption += IntToWideString(TotNumWeaponsToBuy + TotNumItemsToBuy + TotNumClothesToBuy);
    entry->FindChild("PC_TRADING_BUY_TEXT")->SetText(caption, true, true);
    CTCShop* shop = 0;
    GetTradingThing()->GetInterface(&shop, 0x53);
    std::vector<long> wanted = shop->GetWantedItemsList();
    NUISystem::CComponent* wantedEntry = list->FindChild("PC_MENU_ENTRY_TRADE_WANTED");
    TotNumClothesWanted = TotNumWeaponsWanted = TotNumItemsWanted = 0;
    if (!wanted.empty()) {
        for (const long* w = wanted.begin(); w != wanted.end(); ++w) {
            switch (GetTypeFromDefIndexPC(*w)) {
            case 0: ++TotNumClothesWanted; break;
            case 1: ++TotNumWeaponsWanted; break;
            case 2: ++TotNumItemsWanted; break;
            }
        }
    }
    caption = bank->GetTextBySymbol("TEXT_GUI_MENU_WANTED") + GetXString();
    caption += IntToWideString(TotNumItemsWanted + TotNumWeaponsWanted + TotNumClothesWanted);
    wantedEntry->FindChild("PC_TRADING_WANTED_TEXT")->SetText(caption, true, true);
    return list;
}
