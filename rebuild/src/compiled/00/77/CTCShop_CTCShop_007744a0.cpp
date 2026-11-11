// CTCShop::CTCShop(CThing* thing) -- CTCBase(thing), the shop vtable, stock state 1, an empty shopkeeper pointer, shopper and goods-place lists and stock map; then the stock map is filled from the thing's shop def (every stock item copied, its quantity raised to at least the maximum stock) and the last-checked day set 30 days back
// Leaf: ??0CTCShop
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007744A0, 361 bytes (`ret 4`).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CBaseIntelligentPointer() 0x00A01B10 (then the
//          CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8); malloc 0x00BFEA0E (the 0x30-byte map header node);
//          CTCBase::GetDefinitionManager() const 0x006869D0; CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C and
//          CDefClassBase::GetSubDef<CShopDef> 0x005130A0 (pushes "CShopDef"); CShopDef::Test() const 0x007773A0
//          (FableWin.pdb; the catalog name; a walk of the def's maps with its checks compiled out); map<long, CShopItem>::operator[](const long&) 0x00778280;
//          CTCBase::GetWorld 0x00686A70; the defs' slot 1 on release.
// Layout (retail CTCShop, Ego_r.pdb with 12-byte containers): vtable 0x0126A94C; StockState +0x0c, PShopKeeper +0x10,
//          Shoppers +0x18, ShopKeeperAvailable +0x24 (left unset), GoodsPlaces +0x28, Stock +0x34 (STLport tree: header,
//          node count, key compare), SpecialItem +0x40, DaysSinceSpecialItemPlaced +0x44, DayLastChecked +0x48,
//          GoldDonated +0x4c, StallLike +0x50, Alarm +0x54; CThing DefIndex word +0x8c; CShopDef DefaultStock +0x40
//          (STLport vector of 0x48-byte CShopItemDef: Type +0x28, CShopItem +0x2c); CShopItem Quantity +0x00, MaxStock +0x08; the world's game
//          time manager +0x1c, day +0x14.
// Shape: both defs are null-tested CDefPointer locals with GetDef/GetSubDef stand-ins (landed GetName idiom), the shop def
//        out pointer in the dead `thing` argument slot; the stock copy is a 28-byte struct assignment (`rep movsd`) and the
//        quantity is STLport `max` (`a < b ? b : a` over references).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }

class CShopItem { public:                              // 28 bytes
    float Quantity;                             // +0x00
    float PriceMultiplier;                      // +0x04
    float MaxStock;                             // +0x08
    long  RestockPeriod;                        // +0x0c
    float RestockPeriodRandomness;              // +0x10
    float SalesPerDay;                          // +0x14
    float SalesPerDayRandomness;                // +0x18
};
class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                    // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005130A0 GetSubDef<CShopDef> (pushes "CShopDef")
};
class CThingBaseDef : public CDefClassBase { public: };
class CShopItemDef : public CDefClassBase { public:   // 0x48-byte stock record (Ego_r.pdb CShopItemDef)
    unsigned char _pad08[0x28 - 0x08];
    long          Type;                         // +0x28 (item def index or inventory category)
    CShopItem     Item;                         // +0x2c
};
struct CShopItemDefVector {                     // STLport vector<CShopItemDef> (12 bytes)
    CShopItemDef* _M_start;
    CShopItemDef* _M_finish;
    CShopItemDef* _M_end_of_storage;
};
class CShopDef : public CDefClassBase { public:
    unsigned char      _pad08[0x40 - 0x08];
    CShopItemDefVector DefaultStock;            // +0x40
    void Test() const;                          // 0x007773A0 (FableWin.pdb ?Test@CShopDef@@QBEXXZ; catalog CShopDef::Test)
};
void* FindDef_CDefClassBase_GetSubDef_1();      // stand-in callee of the stand-in body below
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed GetName 0x00772610 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef>
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
void* FindDef_CDefinitionManager_GetDef_0();    // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
};
class CGameTimeManager { public:
    unsigned char _pad[0x14];
    long          m_day;                        // +0x14
};
class CWorld { public:
    unsigned char     _pad[0x1c];
    CGameTimeManager* m_pGameTimeManager;       // +0x1c
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct SStockNode {                             // 0x30 bytes: tree links + pair<const long, CShopItem>
    unsigned char m_color;                      // +0x00 (0 = red)
    SStockNode*   m_pParent;                    // +0x04
    SStockNode*   m_pLeft;                      // +0x08
    SStockNode*   m_pRight;                     // +0x0c
    long          m_key;                        // +0x10
    CShopItem     m_value;                      // +0x14
};
struct CStockTreeBase {
    SStockNode* m_pHeader;
    CStockTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SStockNode*)malloc(sizeof(SStockNode));
    }
};
struct CStockTree : CStockTreeBase {
    unsigned long m_nodeCount;
    struct SLess { } m_compare;
    CStockTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};
struct CStockMap : CStockTree {                 // map<long, CShopItem>
    CShopItem& operator[](const long& key);     // 0x00778280
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)
};
class CTCShop : public CTCBase { public:                      // vtable 0x0126A94C
    long          StockState;                   // +0x0c
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;   // +0x10
    CRetailVector<CIntelligentPointer<CThingCreatureBase> > Shoppers;   // +0x18
    bool          ShopKeeperAvailable;          // +0x24
    CRetailVector<long> GoodsPlaces;            // +0x28
    CStockMap     Stock;                        // +0x34
    long          SpecialItem;                  // +0x40
    long          DaysSinceSpecialItemPlaced;   // +0x44
    long          DayLastChecked;               // +0x48
    long          GoldDonated;                  // +0x4c
    bool          StallLike;                    // +0x50
    unsigned long Alarm;                        // +0x54
    CTCShop(CThing* thing);
    virtual void v00();
};

CTCShop::CTCShop(CThing* thing)
    : CTCBase(thing), StockState(1), SpecialItem(-1), DaysSinceSpecialItemPlaced(0), GoldDonated(0), StallLike(false),
      Alarm(0)
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef)) {
        shopDef->Test();
        for (const CShopItemDef* it = shopDef->DefaultStock._M_start; it != shopDef->DefaultStock._M_finish; ++it) {
            CShopItem& item = Stock[it->Type];
            item = it->Item;
            item.Quantity = Max(item.Quantity, item.MaxStock);
        }
    }
    DayLastChecked = GetWorld().m_pGameTimeManager->m_day - 30;
}