// CTCInventoryTrade::InitialiseBuyableItems(vector<pair<long,long> >& items) -- rebuilds the buyable-item map: marks the hero's own tattoo / hair / ability cards the shop deals in as stocked and counts them into BuyableItems, then walks the shop stock map, recording sold-out specials in NotInStock and adding buyable stock to the inventory and to `items`, and stores the item total
// Leaf: InitialiseBuyableItems@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A30A1, 917 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   FableWin.pdb ?InitialiseBuyableItems@CTCInventoryTrade@@QAEXAAV?$vector@U?$pair@JJ@std@@...@std@@@Z
// Callees: map<long,long> clear 0x004DB91F / operator[] 0x005B30BB / find 0x00592F92 / ~ 0x004DD2B1, _Rb_tree increment 0x004292C0,
//   shop-thing getter 0x005BC4C4, bitset test 0x00410DE0, LowerBound 0x0040F020 (shop 0x53) and const 0x004365B0 (hero 0x11),
//   shop stock map by value 0x00774610, CPlayerManager::GetMainPlayer 0x00449960 (g_pMainGameComponent +0x1C), GetControlledCreature
//   0x00487DD0, GetAllItems 0x005BF482, CTCBase::GetDefinitions 0x006869D0 + GetDef 0x0042AF3C, GetSubDef<CAbilityDef> 0x005B1E5A,
//   GetTattooCardDef 0x005D9AA0 / GetHairCardDef 0x005D9A10 (`__fastcall`, by value), shop type tests 0x00772510 / 0x00772490 /
//   0x00772590 (names unproven), GetMaxStock 0x00772720, IsBuyable 0x005D9B50, AddItemOfTypeToInventory 0x0059C6B0,
//   NotInStock clear 0x0042A168 / add 0x0042B687, vector push_back 0x005947B6, free 0x00BFEA14.
// Shape: the key is a second named copy of the def index (address passed to the map, value kept in a register); the hair test is
//   a second statement so each by-value def temporary is released before the next test; the stocked mark is written in each
//   arm with a `goto` past the BuyableItems update (VC7.1 keeps both operator[] calls and merges the `= 1` tail, as retail);
//   `key` is declared before the def pointer (store order before the GetDef call).
// Layout: local view -- NotInStock +0x140, item total +0x168, mode +0x170, BuyableItems map +0x1A4; STLport map is 12 bytes.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CTCBase;
namespace std {
template <class A, class B> struct pair {
    A first; B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    void push_back(const T& v);                         // 0x005947B6
};
}
struct _Rb_tree_node_base {
    long _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
_Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0
struct CLongMapNode : _Rb_tree_node_base { long first; long second; };
struct CLongMap {                                       // std::map<long,long>, 12 bytes
    CLongMapNode* _M_header;
    unsigned int _M_node_count;
    long _M_key_compare;
    struct iterator {
        CLongMapNode* _M_node;
        CLongMapNode* operator->() const { return _M_node; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        bool operator==(CLongMapNode* n) const { return _M_node == n; }
        iterator& operator++() { _M_node = (CLongMapNode*)_M_increment(_M_node); return *this; }
    };
    CLongMap(const CLongMap& other);
    ~CLongMap();                                        // 0x004DD2B1
    void clear();                                       // 0x004DB91F
    long& operator[](const long& key);                  // 0x005B30BB
    CLongMapNode* find(const long& key);                // 0x00592F92
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();
    long m_refCount;
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> struct CDefBasePointer {
    T* Object;
    __forceinline CDefBasePointer(const CDefBasePointer& o) : Object(o.Object) { }
    __forceinline ~CDefBasePointer()
    {
        if (Object) {
            T* p = Object;
            --p->m_refCount;
            if (p->m_refCount == 0)
                p->Destroy();
            Object = 0;
        }
    }
};
class CHairCardDef : public CDefBase { public: };
class CTattooDef : public CDefBase { public: };
CDefBasePointer<CHairCardDef> __fastcall GetHairCardDef(long defIndex);      // 0x005D9A10
CDefBasePointer<CTattooDef> __fastcall GetTattooCardDef(long defIndex);      // 0x005D9AA0
bool __fastcall IsBuyable(long defIndex);               // 0x005D9B50
class CAbilityDef : public CDefBase { public: };
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005b1e5a(CDefPointer<CAbilityDef>* out);      // 0x005B1E5A
};
void* FindDef_a();
void* FindDef_b();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005b1e5a(CDefPointer<CAbilityDef>* out)
{
    void* def = FindDef_a();
    if (!def)
        return false;
    out->Object = (CAbilityDef*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_b();
    if (!def)
        return false;
    out->Object = (CDefClassBase*)def;
    return true;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPresenceBits { bool test(long kind) const; };   // 0x00410DE0
class CThing { public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long kind) const
    {
        if (m_present.test(kind)) {
            long key = kind;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent { public: unsigned char _pad[0x1c]; CPlayerManager* PlayerManager; const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; } };   // +0x1c
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
struct CTCShopkeeper {
    CLongMap GetStockMap() const;                          // 0x00774610
    bool IsTattooShop() const;                          // 0x00772510
    bool IsBarberShop() const;                       // 0x00772490
    bool IsAbilityShop() const;                        // 0x00772590
    long GetMaxStock(long defIndex) const;         // 0x00772720
};
class CTCInventoryItem { public:
    void GetAllItems(std::vector<std::pair<long, long> >& out) const;   // 0x005BF482
};
struct CLongList { void clear(); void push_back(const long& v); };   // 0x0042A168 / 0x0042B687 (labelled LinkedList_PushFront)
typedef std::vector<std::pair<long, long> > CItemVector;
class CTCInventoryTrade { public:
    unsigned char _pad00[0x140];
    CLongList NotInStock;                               // +0x140
    unsigned char _pad144[0x168 - 0x144];
    long TotalBuyable;                                  // +0x168
    unsigned char _pad16c[0x170 - 0x16c];
    long Mode;                                          // +0x170
    unsigned char _pad174[0x1a4 - 0x174];
    CLongMap BuyableItems;                              // +0x1a4
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    CThing* GetShopThing();                             // 0x005BC4C4
    void AddItemOfTypeToInventory(long defIndex, long count);     // 0x0059C6B0
    void InitialiseBuyableItems(CItemVector& items);
};
void CTCInventoryTrade::InitialiseBuyableItems(CItemVector& items)
{
    BuyableItems.clear();
    long total = 0;
    Mode = 0xb;
    CTCShopkeeper* shop = 0;
    GetShopThing()->GetInterface(&shop, 0x53);
    CLongMap stock = shop->GetStockMap();
    CTCInventoryItem* inventory = 0;
    static_cast<const CThing*>(g_pMainGameComponent->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature())->GetInterface(&inventory, 0x11);
    CItemVector heroItems;
    inventory->GetAllItems(heroItems);
    for (unsigned int i = 0; i < heroItems.size(); ++i) {
        long defIndex = heroItems[i].first;
        long key = defIndex;
        CDefPointer<CDefClassBase> def;
        GetDefinitionManager()->GetDef(defIndex, &def);
        CDefPointer<CAbilityDef> ability;
        bool special = GetTattooCardDef(defIndex).Object && shop->IsTattooShop();
        if (!special)
            special = GetHairCardDef(defIndex).Object && shop->IsBarberShop();
        if (special)
            stock[key] = 1;
        else if (def->GetSubDef_005b1e5a(&ability) && shop->IsAbilityShop())
            stock[key] = 1;
        else
            goto next;
        {
            CLongMapNode* it = BuyableItems.find(key);
            if (it == BuyableItems._M_header)
                BuyableItems[key] = heroItems[i].second;
            else
                it->second += heroItems[i].second;
        }
    next:
        ;
    }
    NotInStock.clear();
    for (CLongMapNode* it = (CLongMapNode*)stock._M_header->_M_left; it != stock._M_header; it = (CLongMapNode*)_M_increment(it)) {
        long count = it->second;
        long defIndex = it->first;
        long key = defIndex;
        if (count == 0) {
            bool plain = GetTattooCardDef(defIndex).Object == 0 && GetHairCardDef(defIndex).Object == 0;
            if (plain && shop->GetMaxStock(defIndex))
                NotInStock.push_back(key);
        } else if (IsBuyable(defIndex)) {
            AddItemOfTypeToInventory(defIndex, count);
            total += count;
            items.push_back(std::pair<long, long>(defIndex, count));
        }
    }
    TotalBuyable = total;
}