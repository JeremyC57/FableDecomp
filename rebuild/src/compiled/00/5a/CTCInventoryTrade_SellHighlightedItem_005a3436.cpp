// CTCInventoryTrade::SellHighlightedItem() -- in the selected category's highlighted slot, sell the item (positive def index) to the owner's shop: take the shop's buy price for one, let the shop record the sale, pay the hero and remove one from the trade list; then rebuild the buyable and sellable lists and refresh the animation and owned count for the highlighted item
// Leaf: SellHighlightedItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005A3436, 331 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 53h ; pop edi`).
// Callees: NInventory::CTCInventoryBase::GetSelectedCategory @ 0x005BCACE, CCategory::GetItemInSlotWithIndex
//   @ 0x005BC9B0, CTCBase owner getter @ 0x005BC4C4, std::bitset<>::test @ 0x00410DE0 and CVectorMap LowerBound
//   @ 0x0040F020 (inlined GetInterface, types 0x53 and 4), CTCShop::GetCurrentBuyPrice(long, long) @ 0x007742C0 and
//   its sale record @ 0x00774D20, CPlayerManager::GetMainPlayer @ 0x00449960 (engine 0x013B86A0 +0x1C),
//   CPlayer::GetHeroCreature @ 0x00487DC0, CTCHeroStats::AddMoney(long, bool) @ 0x0057B327, this's virtual slot 79
//   (+0x13C, (item, 1)), the trade-list reset @ 0x0059D9AD (label "CMemoryAllocator::ClearPerformanceStats" is
//   propagated), InitialiseBuyableItems(vector<pair<long, long> >&) @ 0x005A30A1, InitialiseSellableItems
//   @ 0x0059D820, GetHighlightedItem @ 0x005BCAE3, ResetInventoryAnim(long) @ 0x005BCD2D,
//   SetNumberOfSelectedItemOwned(long) @ 0x005BE284, the CRT free thunk @ 0x00BFEA14.
// Shape: the hero is fetched before its stats pointer is zeroed (`and [ebp-8],0` after GetHeroCreature); both lookups are the out-parameter GetInterface with the bool ignored into a pointer initialised to 0 (the
//   shop pointer lives in edi; the stats pointer shares the frame word of the 0x04 lookup key, as 0x00789E60).
// Layout: retail CTCInventoryTrade CategoryIdent +0x170 (as InitialiseSellableItems 0x0059D820); CCategory highlighted
//   slot index +0x24.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const { return present.test(type); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CPlayer { CThing* GetHeroCreature(); };                      // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer() const; };          // 0x00449960
struct CEngine {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1C
};
extern CEngine* g_pEngine;                                  // 0x013B86A0

namespace NInventory {
struct CItem {
    long DefIndex;                                          // +0x00
    long Count;                                             // +0x04
};
struct CCategory {
    unsigned char _pad00[0x24];
    long HighlightedIndex;                                  // +0x24
    long GetItemInSlotWithIndex(long index) const;          // 0x005BC9B0
};
}
struct CPairVector {                                        // STLport vector<pair<long, long> > (12 bytes)
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CPairVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CPairVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
struct CTCShop {
    long GetCurrentBuyPrice(long defIndex, long count) const;   // 0x007742C0
    void SellHighlightedItem(long defIndex);                // 0x00774D20
};
struct CTCHeroStats {
    void AddMoney(long amount, bool showMessage);           // 0x0057B327
};

struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); 
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); 
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); 
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); 
    virtual void RemoveItem(long item, long count);         // slot 79 (+0x13C)
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x160 - 0x08];
    CThing* GetOwner() const;                               // 0x005BC4C4
    NInventory::CCategory* GetSelectedCategory();           // 0x005BCACE
    long GetHighlightedItem() const;                        // 0x005BCAE3
    void ResetInventoryAnim(long item);                     // 0x005BCD2D
    void SetNumberOfSelectedItemOwned(long item);           // 0x005BE284
};
struct CTCInventoryTrade : CTCInventoryBase {
    long          CategoryCount;                            // +0x160
    long          ItemSellCount;                            // +0x164
    long          ItemBuyCount;                             // +0x168
    long          ItemSellUniqueCount;                      // +0x16C
    long          CategoryIdent;                            // +0x170 EInventoryCategoryIdentifier
    void ResetTradeLists();                                 // 0x0059D9AD
    void InitialiseBuyableItems(CPairVector& items);        // 0x005A30A1
    void InitialiseSellableItems();                         // 0x0059D820
    void SellHighlightedItem();
};

void CTCInventoryTrade::SellHighlightedItem()
{
    CategoryIdent = 0xc;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category) {
        long item = category->GetItemInSlotWithIndex(category->HighlightedIndex);
        if (item > 0) {
            CTCShop* shop = 0;
            GetOwner()->GetInterface(&shop, 0x53);
            long price = shop->GetCurrentBuyPrice(item, 1);
            shop->SellHighlightedItem(item);
            CThing* hero = g_pEngine->PlayerManager->GetMainPlayer()->GetHeroCreature();
            CTCHeroStats* stats = 0;
            hero->GetInterface(&stats, 4);
            stats->AddMoney(price, true);
            RemoveItem(item, 1);
        }
    }
    ResetTradeLists();
    CPairVector buyable;
    InitialiseBuyableItems(buyable);
    InitialiseSellableItems();
    ResetInventoryAnim(GetHighlightedItem());
    SetNumberOfSelectedItemOwned(GetHighlightedItem());
}