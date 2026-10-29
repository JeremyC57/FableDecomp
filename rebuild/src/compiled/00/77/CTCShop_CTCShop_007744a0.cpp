// CTCShop::CTCShop(CThing* thing) -- CTCBase(thing), the shop vtable, stock state 1, an empty shopkeeper pointer, shopper and goods-place lists and stock map; then the stock map is filled from the thing's shop def (every stock item copied, its quantity raised to at least the maximum stock) and the last-checked day set 30 days back
// Leaf: ??0CTCShop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007744A0, 361 bytes (`ret 4`).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CBaseIntelligentPointer() 0x00A01B10 (then the
//          CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8); malloc 0x00BFEA0E (the 0x30-byte map header node);
//          CTCBase::GetDefinitions() const 0x006869D0; CDefinitionManager::GetDef(long, CDefPointer*) 0x0042AF3C and
//          CDefClassBase::GetSubDef(CDefPointer<const CShopDef>*) 0x005130A0 (manifest labels are propagated byte twins; the
//          landed GetName 0x00772610 names them); the shop def's stock-list preparation 0x007773A0 (manifest
//          "CTCShop_ClearLists", name unproven); map<long, CShopItem>::operator[](const long&) 0x00778280;
//          CTCBase::GetWorld 0x00686A70; the defs' slot 1 on release.
// Layout (retail CTCShop, Ego_r.pdb with 12-byte containers): vtable 0x0126A94C; StockState +0x0c, PShopKeeper +0x10,
//          Shoppers +0x18, ShopKeeperAvailable +0x24 (left unset), GoodsPlaces +0x28, Stock +0x34 (STLport tree: header,
//          node count, key compare), SpecialItem +0x40, DaysSinceSpecialItemPlaced +0x44, DayLastChecked +0x48,
//          GoldDonated +0x4c, StallLike +0x50, Alarm +0x54; CThing DefIndex word +0x8c; CShopDef stock items +0x40/+0x44
//          (0x48-byte records: key +0x28, CShopItem +0x2c); CShopItem Quantity +0x00, MaxStock +0x08; the world's game
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
class CStockItemDef { public:                          // 0x48 bytes
    unsigned char _pad00[0x28];
    long          Key;                          // +0x28
    CShopItem     Item;                         // +0x2c
};

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char  _pad08[0x40 - 0x08];
    CStockItemDef* StockItemsFirst;             // +0x40
    CStockItemDef* StockItemsLast;              // +0x44
    void PrepareStockItems();                   // 0x007773A0 (name unproven)
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CShopDef>* out); // 0x005130A0 (CDefPointer<const CShopDef>*)
};
void* FindDef_CDefClassBase_GetSubDef_1();      // stand-in callee of the stand-in body below
// Stand-in bodies (the real ones are out of line in retail): they write the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed GetName 0x00772610 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C (CDefPointer<const CThingDef>*)
};
void* FindDef_CDefinitionManager_GetDef_0();    // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
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

struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0123E7D8 (CThingCreatureBase)
    virtual void v00();
};
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
    CDefinitionManager* GetDefinitions() const; // 0x006869D0
protected:
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
    CDefPointer<CDefClassBase> thingDef;
    GetDefinitions()->GetDef(ParentThing->DefIndex, &thingDef);
    CDefPointer<CShopDef> shopDef;
    if (thingDef->GetSubDef(&shopDef)) {
        shopDef->PrepareStockItems();
        for (CStockItemDef* it = shopDef->StockItemsFirst; it != shopDef->StockItemsLast; ++it) {
            CShopItem& item = Stock[it->Key];
            item = it->Item;
            item.Quantity = Max(item.Quantity, item.MaxStock);
        }
    }
    DayLastChecked = GetWorld().m_pGameTimeManager->m_day - 30;
}