// CTCInventoryTrade::~CTCInventoryTrade() -- delete every UI component the inventory owns and empty the list, then destroy the trade members in reverse order and both bases
// Leaf: ??1CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059EC48, length 250 bytes (tail jump into the primary base dtor).
// Vtables: CTCInventoryTrade primary 0x01253034, its NHeroInformationScreens::CBase side 0x0125301C at +0x130 (both stored
// first), the CBase vtable 0x0123A3E0 re-stored by its inline empty dtor before the jump.
// Callees: the Components erase(first, last) @ 0x00485AA7 (catalog "CGuiControlListBox::SelectEntryFromData" is
// propagated), the STLport container dtors (catalog "CActiveFile::OnReadFinished", propagated byte twins): the
// CComponent-to-CComponent maps @ 0x005B21F4, ~CWideString @ 0x0099B510, the CComponent-to-(CWideString, long) maps
// @ 0x005B2847, map<long, long> @ 0x004DD2B1, the CComponent-to-CList maps @ 0x005B2160, the CComponent-to-(long, long) maps
// @ 0x005B211F, list<long> @ 0x0042AB73, vector<CEnginePrimitiveHandle> @ 0x00440C40;
// NInventory::CTCInventoryBase::~CTCInventoryBase @ 0x005C00A0.
// Shape: size-optimised TU (`#pragma optimize("s", on)`, as the landed CTCInventoryClothing dtor 0x005B7049, whose model
// this follows: under speed the loop entry test is `test ; jbe` instead of the sar flags' `je`); the body walks the
// base's Components vector by index (`size()` recomputed each pass, `delete` = null test + virtual scalar deleting dtor
// with 1) and empties it through an inline clear() = erase(begin(), end()) (the vector address is the `this` of the
// call, so the range is pushed straight from [edi+4] / [edi]).
// Layout: Ego_r.pdb CTCInventoryTrade at retail offsets (CTCInventoryBase is 0x130 bytes in retail, containers 12 bytes,
// list 4): TradeAnimPrimitiveHandles +0x134, NotInStock +0x140, ItemsToBuy/ItemsToSell +0x144/+0x150, Sell/Buy/Wanted
// ListsByCategory +0x180/+0x18c/+0x198, SpecialItemsToDelete +0x1a4, NumBuyable/Sellable/WantedThings +0x1b0/+0x1bc/+0x1c8,
// SpecialItemString +0x1fc, ItemBuyLists/ItemSellLists +0x200/+0x20c; the base's Components vector at +0x0c.
// engine/CTCInventoryTrade.h is the flattened PDB layout (16-byte containers), so the class is modelled locally.
#pragma optimize("s", on)
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();
};
}
struct CComponentVector {                                     // vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long i) { return _M_start[i]; }
    NUISystem::CComponent** begin() { return _M_start; }
    NUISystem::CComponent** end() { return _M_finish; }
    void erase(NUISystem::CComponent** first, NUISystem::CComponent** last);   // 0x00485AA7
    void clear() { erase(begin(), end()); }
};
struct CPrimitiveHandleVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; ~CPrimitiveHandleVector(); };   // 0x00440C40
struct CLongList { void* _M_node; ~CLongList(); };                                                              // 0x0042AB73
struct CComponentPairMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; ~CComponentPairMap(); };     // 0x005B211F
struct CComponentListMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; ~CComponentListMap(); };     // 0x005B2160
struct CLongLongMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; ~CLongLongMap(); };               // 0x004DD2B1
struct CComponentStringMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; ~CComponentStringMap(); }; // 0x005B2847
struct CComponentMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; ~CComponentMap(); };             // 0x005B21F4
struct CWideString { void* m_pData; ~CWideString(); };                                                         // 0x0099B510

namespace NInventory {
class CTCInventoryBase {                                      // retail sizeof 0x130
public:
    virtual ~CTCInventoryBase();                              // 0x005C00A0
    unsigned char    _pad04[0x0c - 0x04];
    CComponentVector Components;                              // +0x0c
    unsigned char    _pad18[0x130 - 0x18];
};
}
namespace NHeroInformationScreens {
class CBase {                                                 // vtable 0x0123A3E0
public:
    virtual ~CBase() {}
};
}
class CTCInventoryTrade : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CPrimitiveHandleVector TradeAnimPrimitiveHandles;         // +0x134
    CLongList              NotInStock;                        // +0x140
    CComponentPairMap      ItemsToBuy;                        // +0x144
    CComponentPairMap      ItemsToSell;                       // +0x150
    bool  IsShoppingValid;                                    // +0x15c
    long  CategoryCount;                                      // +0x160
    long  ItemSellCount;                                      // +0x164
    long  ItemBuyCount;                                       // +0x168
    long  ItemSellUniqueCount;                                // +0x16c
    long  CategoryIdent;                                      // +0x170
    long  PreviousScreenType;                                 // +0x174
    bool  WarningNeeded;                                      // +0x178
    long  TrophyUseNeeded;                                    // +0x17c
    CComponentListMap      SellListsByCategory;               // +0x180
    CComponentListMap      BuyListsByCategory;                // +0x18c
    CComponentListMap      WantedListsByCategory;             // +0x198
    CLongLongMap           SpecialItemsToDelete;              // +0x1a4
    CComponentStringMap    NumBuyableThings;                  // +0x1b0
    CComponentStringMap    NumSellableThings;                 // +0x1bc
    CComponentStringMap    NumWantedThings;                   // +0x1c8
    long  TotNumClothesToSell;                                // +0x1d4
    long  TotNumWeaponsToSell;                                // +0x1d8
    long  TotNumItemsToSell;                                  // +0x1dc
    long  TotNumClothesToBuy;                                 // +0x1e0
    long  TotNumWeaponsToBuy;                                 // +0x1e4
    long  TotNumItemsToBuy;                                   // +0x1e8
    long  TotNumClothesWanted;                                // +0x1ec
    long  TotNumWeaponsWanted;                                // +0x1f0
    long  TotNumItemsWanted;                                  // +0x1f4
    long  TotSpecialItemsToBuy;                               // +0x1f8
    CWideString            SpecialItemString;                 // +0x1fc
    CComponentMap          ItemBuyLists;                      // +0x200
    CComponentMap          ItemSellLists;                     // +0x20c
    bool  InventoryTradeOpen;                                 // +0x218
    bool  InventoryTradeInitialised;                          // +0x219
    NUISystem::CComponent* PMouse_Pointer;                    // +0x21c
    NUISystem::CComponent* PTop_Level_List;                   // +0x220
    NUISystem::CComponent* PCurrentOpenTradeMenu;             // +0x224
    float AccumFadeTime;                                      // +0x228
    virtual ~CTCInventoryTrade();
};

CTCInventoryTrade::~CTCInventoryTrade()
{
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
}
