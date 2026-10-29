// CTCInventoryTrade::InitialiseSellableItems() -- select category 12, gather every item of the hero's three inventory components (interfaces 0x11, 0x13, 0x12), add each buyable one (positive def index) to the trade inventory and total their counts as the sell count, then refresh the unique count of the selected category and the category count
// Leaf: InitialiseSellableItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059D820, 397 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`push 11h ; pop ebx`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449960 (engine 0x013B86A0 +0x1C), CPlayer::PeekPControlledCreature
//   @ 0x00487DD0, std::bitset<>::test @ 0x00410DE0 and the const CVectorMap LowerBound @ 0x004365B0 (the inlined
//   GetInterface), CTCInventoryBase::GetAllItems(vector<NInventory::CItem>&) @ 0x005BF482 (landed, another TU),
//   CTCBase owner getter @ 0x005BC4C4 (label "CWorld::DrawGetEnvironment" is propagated), CTCInventoryItem::IsBuyable
//   (long) @ 0x005D9B30 (static `__fastcall`), CTCInventoryTrade::AddItemOfTypeToInventory(long, long) @ 0x0059C6B0,
//   NInventory::CTCInventoryBase::GetSelectedCategory @ 0x005BCACE, CCategory::GetUniqueItemCountInCategory
//   @ 0x005BC8AC, the category-count getter @ 0x005BCE2F (label "CWorldMap::DrawGetNoMaps" is propagated), the CRT
//   free thunk @ 0x00BFEA14 (the item vector's storage).
// Shape: the three lookups are the out-parameter GetInterface with the result ignored into one pointer initialised
//   to 0 once (edi doubles as the zero register; an absent interface keeps the previous pointer); `total` is declared
//   before the hero lookup (its zero store precedes the GetMainPlayer call); the owner's HasTC(0x53) is evaluated and discarded; the item vector
//   escapes (GetAllItems is not a TU-mate here), so its start and size are re-read every pass.
// Layout: retail CTCInventoryTrade (Ego_r.pdb order, release containers): CategoryCount +0x160, ItemSellCount +0x164,
//   ItemSellUniqueCount +0x16C, CategoryIdent +0x170; NInventory::CItem 8 bytes (def index, count) as 0x005BF77D.
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CTCBase;
struct CPresenceBits { bool test(long kind) const; };      // bitset<>::test 0x00410DE0
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const { return present.test(type); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (present.test(interfaceType)) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };          // 0x00487DD0
class CPlayerManager { public: const CPlayer& GetMainPlayer() const; };          // 0x00449960
class CEngine { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                          // +0x1C
};
extern CEngine* g_pEngine;                                  // 0x013B86A0

namespace NInventory {
class CItem { public:
    long DefIndex;                                          // +0x00
    long Count;                                             // +0x04
};
class CCategory { public:
    long GetUniqueItemCountInCategory() const;              // 0x005BC8AC
};
}
struct CItemVector {                                        // STLport vector<NInventory::CItem> (12 bytes)
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    NInventory::CItem& operator[](unsigned long n) { return _M_start[n]; }
};
class CTCInventoryItem { public:
    static bool __fastcall IsBuyable(long defIndex);        // 0x005D9B30
};

struct CTCInventoryBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x160 - 0x08];
    CThing* GetOwner() const;                               // 0x005BC4C4
    void GetAllItems(CItemVector& items);                   // 0x005BF482
    NInventory::CCategory* GetSelectedCategory();           // 0x005BCACE
    long GetCategoryCount() const;                          // 0x005BCE2F
};
class CTCInventoryTrade : public CTCInventoryBase { public:
    long          CategoryCount;                            // +0x160
    long          ItemSellCount;                            // +0x164
    long          ItemBuyCount;                             // +0x168
    long          ItemSellUniqueCount;                      // +0x16C
    long          CategoryIdent;                            // +0x170 EInventoryCategoryIdentifier
    bool AddItemOfTypeToInventory(long defIndex, long count);   // 0x0059C6B0
    void InitialiseSellableItems();
};

void CTCInventoryTrade::InitialiseSellableItems()
{
    CategoryIdent = 0xc;
    long total = 0;
    const CThing* hero = g_pEngine->PlayerManager->GetMainPlayer().PeekPControlledCreature();
    CItemVector items;
    CTCInventoryBase* inventory = 0;
    hero->GetInterface(&inventory, 0x11);
    inventory->GetAllItems(items);
    hero->GetInterface(&inventory, 0x13);
    inventory->GetAllItems(items);
    hero->GetInterface(&inventory, 0x12);
    inventory->GetAllItems(items);
    bool isShop = GetOwner()->HasTC(0x53);
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].DefIndex;
        if (defIndex > 0 && CTCInventoryItem::IsBuyable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, items[i].Count);
            total += items[i].Count;
        }
    }
    ItemSellCount = total;
    NInventory::CCategory* category = GetSelectedCategory();
    if (category)
        ItemSellUniqueCount = category->GetUniqueItemCountInCategory();
    CategoryCount = GetCategoryCount();
}