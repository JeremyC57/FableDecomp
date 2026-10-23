// CTCInventoryTrade::BuyHighlightedItem() -- buy the highlighted item of the selected category: when the hero can pay the shop's buy price and the item's inventory can take it, the shop sells it, the hero pays, the item is added and the trade lists are rebuilt; the owned count is refreshed
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
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
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

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
struct CTCInventoryBase;
long __fastcall GetInventoryCategory_005d99d0(long defIndex);       // 0x005D99D0
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
    long GetCurrentBuyPrice(long defIndex, long count) const;   // 0x007743B0
    void SellItem(long defIndex);                           // 0x00774730
};
struct CTCHeroStats {
    unsigned char _pad00[0x3c];
    long Money;                                             // +0x3C
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
    virtual void v80();
    virtual void AddItem(long item, long count, bool a, bool b, long price, bool c);   // slot 81 (+0x144)
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x160 - 0x08];
    CThing* GetOwner() const;                               // 0x005BC4C4
    NInventory::CCategory* GetSelectedCategory();           // 0x005BCACE
    long GetHighlightedItem() const;                        // 0x005BCAE3
    void ResetInventoryAnim(long item);                     // 0x005BCD2D
    void SetNumberOfSelectedItemOwned(long item);           // 0x005BE284
    bool CanAddItem(long item) const;                       // 0x005BF2ED
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
    void BuyHighlightedItem();
};

void CTCInventoryTrade::BuyHighlightedItem()
{
    NInventory::CCategory* category = GetSelectedCategory();
    if (category) {
        long item = category->GetItemInSlotWithIndex(category->HighlightedIndex);
        if (item > 0) {
            CThing* owner = GetOwner();
            CTCShop* shop = 0;
            owner->GetInterface(&shop, 0x53);
            long price = shop->GetCurrentBuyPrice(item, 1);
            CThing* hero = g_pEngine->PlayerManager->GetMainPlayer()->GetHeroCreature();
            CTCHeroStats* stats = 0;
            hero->GetInterface(&stats, 4);
            if (stats->Money >= price) {
                CTCInventoryBase* inventory = 0;
                hero->GetInterface(&inventory, GetInventoryCategory_005d99d0(item));
                if (inventory->CanAddItem(item)) {
                    shop->SellItem(item);
                    stats->AddMoney(-price, true);
                    inventory->AddItem(item, 1, false, true, price, false);
                    ResetTradeLists();
                    CPairVector buyable;
                    InitialiseBuyableItems(buyable);
                    InitialiseSellableItems();
                    ResetInventoryAnim(GetHighlightedItem());
                }
                SetNumberOfSelectedItemOwned(GetHighlightedItem());
            }
        }
    }
}