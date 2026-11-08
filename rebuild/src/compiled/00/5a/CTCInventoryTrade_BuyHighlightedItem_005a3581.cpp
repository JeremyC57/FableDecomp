// CTCInventoryTrade::BuyHighlightedItem() -- buy the highlighted item of the selected category: when the hero can pay the shop's buy price and the item's inventory can take it, the shop sells it, the hero pays, the item is added and the trade lists are rebuilt; the owned count is refreshed
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#include "fable_compiler.h"
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> present;                                  // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const { return present.test(type); }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };                      // 0x00487DC0
struct CTCInventoryBase;
long __fastcall GetInventoryCategory_005d99d0(long defIndex);       // 0x005D99D0
class CPlayerManager { public: const CPlayer& GetMainPlayer() const; };          // 0x00449960
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1C
};

namespace NInventory {
class CItem { public:
    long DefIndex;                                          // +0x00
    long Count;                                             // +0x04
};
class CCategory { public:
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
class CTCShop { public:
    long GetCurrentBuyPrice_007743b0(long defIndex, long count) const;   // 0x007743B0
    void SellItem(long defIndex);                           // 0x00774730
};
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
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
class CTCInventoryTrade : public CTCInventoryBase { public:
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
            long price = shop->GetCurrentBuyPrice_007743b0(item, 1);
            CThing* hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
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