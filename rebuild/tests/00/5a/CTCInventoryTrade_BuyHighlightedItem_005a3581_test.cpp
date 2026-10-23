// Behaviour test for 005a3581: src/005a3581.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCInventoryTrade::BuyHighlightedItem
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
    long GetCurrentBuyPrice_007743b0(long defIndex, long count) const;   // 0x007743B0
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
            long price = shop->GetCurrentBuyPrice_007743b0(item, 1);
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

// ---- stubs ----
#pragma optimize("", on)
bool CPresenceBits::test(long kind) const { return (((const unsigned long*)this)[kind >> 5] >> (kind & 31)) & 1; }
static CThing* g_hero; static CThing* g_owner;
CThing* CPlayer::GetHeroCreature() { return g_hero; }
long __fastcall GetInventoryCategory_005d99d0(long defIndex) { return 0x30; }
static CPlayer g_player; CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
static CPlayerManager g_pm; static CEngine g_engine; CEngine* g_pEngine = &g_engine;
static long g_slotItem; static long g_slotIndex;
long NInventory::CCategory::GetItemInSlotWithIndex(long index) const { g_slotIndex = index; return g_slotItem; }
static long g_price = 50; static long g_priceItem, g_priceCount;
long CTCShop::GetCurrentBuyPrice_007743b0(long defIndex, long count) const { g_priceItem = defIndex; g_priceCount = count; return g_price; }
static int g_sold; static long g_soldItem; static CTCShop* g_soldShop;
void CTCShop::SellItem(long defIndex) { ++g_sold; g_soldItem = defIndex; g_soldShop = this; }
static int g_paid; static long g_paidAmount; static bool g_paidMsg;
void CTCHeroStats::AddMoney(long amount, bool showMessage) { ++g_paid; g_paidAmount = amount; g_paidMsg = showMessage; Money += amount; }
void CTCInventoryBase::v00() {}
void CTCInventoryBase::v01() {}
void CTCInventoryBase::v02() {}
void CTCInventoryBase::v03() {}
void CTCInventoryBase::v04() {}
void CTCInventoryBase::v05() {}
void CTCInventoryBase::v06() {}
void CTCInventoryBase::v07() {}
void CTCInventoryBase::v08() {}
void CTCInventoryBase::v09() {}
void CTCInventoryBase::v10() {}
void CTCInventoryBase::v11() {}
void CTCInventoryBase::v12() {}
void CTCInventoryBase::v13() {}
void CTCInventoryBase::v14() {}
void CTCInventoryBase::v15() {}
void CTCInventoryBase::v16() {}
void CTCInventoryBase::v17() {}
void CTCInventoryBase::v18() {}
void CTCInventoryBase::v19() {}
void CTCInventoryBase::v20() {}
void CTCInventoryBase::v21() {}
void CTCInventoryBase::v22() {}
void CTCInventoryBase::v23() {}
void CTCInventoryBase::v24() {}
void CTCInventoryBase::v25() {}
void CTCInventoryBase::v26() {}
void CTCInventoryBase::v27() {}
void CTCInventoryBase::v28() {}
void CTCInventoryBase::v29() {}
void CTCInventoryBase::v30() {}
void CTCInventoryBase::v31() {}
void CTCInventoryBase::v32() {}
void CTCInventoryBase::v33() {}
void CTCInventoryBase::v34() {}
void CTCInventoryBase::v35() {}
void CTCInventoryBase::v36() {}
void CTCInventoryBase::v37() {}
void CTCInventoryBase::v38() {}
void CTCInventoryBase::v39() {}
void CTCInventoryBase::v40() {}
void CTCInventoryBase::v41() {}
void CTCInventoryBase::v42() {}
void CTCInventoryBase::v43() {}
void CTCInventoryBase::v44() {}
void CTCInventoryBase::v45() {}
void CTCInventoryBase::v46() {}
void CTCInventoryBase::v47() {}
void CTCInventoryBase::v48() {}
void CTCInventoryBase::v49() {}
void CTCInventoryBase::v50() {}
void CTCInventoryBase::v51() {}
void CTCInventoryBase::v52() {}
void CTCInventoryBase::v53() {}
void CTCInventoryBase::v54() {}
void CTCInventoryBase::v55() {}
void CTCInventoryBase::v56() {}
void CTCInventoryBase::v57() {}
void CTCInventoryBase::v58() {}
void CTCInventoryBase::v59() {}
void CTCInventoryBase::v60() {}
void CTCInventoryBase::v61() {}
void CTCInventoryBase::v62() {}
void CTCInventoryBase::v63() {}
void CTCInventoryBase::v64() {}
void CTCInventoryBase::v65() {}
void CTCInventoryBase::v66() {}
void CTCInventoryBase::v67() {}
void CTCInventoryBase::v68() {}
void CTCInventoryBase::v69() {}
void CTCInventoryBase::v70() {}
void CTCInventoryBase::v71() {}
void CTCInventoryBase::v72() {}
void CTCInventoryBase::v73() {}
void CTCInventoryBase::v74() {}
void CTCInventoryBase::v75() {}
void CTCInventoryBase::v76() {}
void CTCInventoryBase::v77() {}
void CTCInventoryBase::v78() {}
void CTCInventoryBase::v80() {}
void CTCInventoryBase::RemoveItem(long, long) {}
static int g_added; static CTCInventoryBase* g_addThis; static long g_addItem, g_addCount, g_addPrice; static bool g_addA, g_addB, g_addC;
void CTCInventoryBase::AddItem(long item, long count, bool a, bool b, long price, bool c)
{ ++g_added; g_addThis = this; g_addItem = item; g_addCount = count; g_addA = a; g_addB = b; g_addPrice = price; g_addC = c; }
CThing* CTCInventoryBase::GetOwner() const { return g_owner; }
static NInventory::CCategory* g_category;
NInventory::CCategory* CTCInventoryBase::GetSelectedCategory() { return g_category; }
long CTCInventoryBase::GetHighlightedItem() const { return 77; }
static int g_anim; static long g_animItem; void CTCInventoryBase::ResetInventoryAnim(long item) { ++g_anim; g_animItem = item; }
static int g_owned; static long g_ownedItem; void CTCInventoryBase::SetNumberOfSelectedItemOwned(long item) { ++g_owned; g_ownedItem = item; }
static bool g_canAdd; static long g_canAddItem; bool CTCInventoryBase::CanAddItem(long item) const { g_canAddItem = item; return g_canAdd; }
static int g_reset, g_buyable, g_sellable;
void CTCInventoryTrade::ResetTradeLists() { ++g_reset; }
void CTCInventoryTrade::InitialiseBuyableItems(CPairVector& items)
{ ++g_buyable; items._M_start = items._M_finish = (NInventory::CItem*)malloc(16); items._M_end_of_storage = items._M_start + 2; }
void CTCInventoryTrade::InitialiseSellableItems() { ++g_sellable; }
static void Reset()
{
    g_slotIndex = -1; g_priceItem = g_priceCount = 0; g_sold = 0; g_soldItem = 0; g_soldShop = 0; g_paid = 0; g_paidAmount = 0; g_paidMsg = false;
    g_added = 0; g_addThis = 0; g_anim = 0; g_owned = 0; g_ownedItem = 0; g_animItem = 0; g_reset = g_buyable = g_sellable = 0; g_canAddItem = 0;
}
static long g_tbuf[2][sizeof(CThing) / 4 + 1];
static void SetBit(CThing* t, long k) { ((unsigned long*)&t->present)[k >> 5] |= 1UL << (k & 31); }
int main()
{
    g_engine.PlayerManager = &g_pm;
    CThing* owner = (CThing*)g_tbuf[0]; CThing* hero = (CThing*)g_tbuf[1];
    static CTCShop shop; static CTCHeroStats stats; static CTCInventoryTrade trade; static CTCInventoryBase heroInv;
    CInterfaceMap::SEntry oe[] = { { 2, 0 }, { 0x53, (CTCBase*)&shop } };
    SetBit(owner, 2); SetBit(owner, 0x53); owner->m_interfaces.first = oe; owner->m_interfaces.last = oe + 2;
    CInterfaceMap::SEntry he[] = { { 4, (CTCBase*)&stats }, { 0x30, (CTCBase*)&heroInv }, { 0x60, 0 } };
    SetBit(hero, 4); SetBit(hero, 0x30); SetBit(hero, 0x60); hero->m_interfaces.first = he; hero->m_interfaces.last = he + 3;
    g_owner = owner; g_hero = hero;
    static NInventory::CCategory cat; cat.HighlightedIndex = 3; g_category = &cat;
    // can pay, can add: full purchase
    Reset(); g_slotItem = 12; stats.Money = 80; g_canAdd = true;
    trade.BuyHighlightedItem();
    CHECK(g_slotIndex == 3 && g_priceItem == 12 && g_priceCount == 1);
    CHECK(g_sold == 1 && g_soldItem == 12 && g_soldShop == &shop && g_paid == 1 && g_paidAmount == -50 && g_paidMsg && stats.Money == 30);
    CHECK(g_added == 1 && g_addThis == &heroInv && g_addItem == 12 && g_addCount == 1 && !g_addA && g_addB && g_addPrice == 50 && !g_addC);
    CHECK(g_reset == 1 && g_buyable == 1 && g_sellable == 1 && g_anim == 1 && g_animItem == 77 && g_owned == 1 && g_ownedItem == 77 && g_canAddItem == 12);
    // can pay, cannot add: only the owned count
    Reset(); stats.Money = 80; g_canAdd = false;
    trade.BuyHighlightedItem();
    CHECK(g_sold == 0 && g_paid == 0 && g_added == 0 && g_reset == 0 && g_owned == 1 && stats.Money == 80);
    // cannot pay
    Reset(); stats.Money = 49; g_canAdd = true;
    trade.BuyHighlightedItem();
    CHECK(g_sold == 0 && g_owned == 0 && g_canAddItem == 0);
    // exact money
    Reset(); stats.Money = 50;
    trade.BuyHighlightedItem();
    CHECK(g_sold == 1 && stats.Money == 0 && g_owned == 1);
    // no item / no category
    Reset(); g_slotItem = 0;
    trade.BuyHighlightedItem();
    CHECK(g_priceItem == 0 && g_owned == 0 && g_slotIndex == 3);
    Reset(); g_category = 0;
    trade.BuyHighlightedItem();
    CHECK(g_slotIndex == -1);
    if (fails == 0) printf("005a3581_TEST PASS\n"); else printf("005a3581_TEST FAIL\n");
    return fails;
}