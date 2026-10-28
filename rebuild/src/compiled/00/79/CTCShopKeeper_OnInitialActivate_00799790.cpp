// CTCShopKeeper::OnInitialActivate() -- point the keeper at its creature's shop thing; when that gives no shop component, take the first shop in the world whose shopkeeper is missing or dead; then register as that shop's keeper and run the CTCBase activation
// Leaf: ?OnInitialActivate@CTCShopKeeper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799790, 354 bytes (plain `ret`); CTCShopKeeper vtable 0x0126C154.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CBaseIntelligentPointer() 0x00A01B10 (then the
//          CIntelligentPointer<CThing> vtable 0x01238C6C), ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer 0x00A01C10;
//          CTCShopKeeper::GetShopTC() const 0x00798F50; CTCBase::GetThingSearchTools() const 0x00686D90 (manifest label is
//          a propagated STL name); CThingSearchTools::GetAllThings<CThing, vector<CThing*>, CThingFilter_HasTC> 0x00799FB0
//          (manifest "<CThingMarker, ..., CThingFilter_HasScriptName>" is a propagated twin); free 0x00BFEA14 (the vector
//          storage); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x53 = word 2 bit 19, the shop);
//          CTCShop::SetShopKeeper(CThingCreatureBase*) 0x007751A0; CTCBase::OnInitialActivate 0x00686870 (manifest "$E2").
// Layout (retail, as the landed GetShopTC 0x00798F50): ParentThing +0x04, Shop (CIntelligentPointer<CThing>) +0x14; the
//          creature's shop-thing pointer +0x170 (name unverified); CTCShop PShopKeeper +0x10; the thing's interface bitset
//          +0x20, map +0x44, dead flag bit 0 of +0x91.
// Shape: `Shop = CIntelligentPointer<CThing>(x)` builds the temporary in the frame (ctor, vtable, SetPItem) and assigns it
//        through an inline operator=; the HasTC filter, the lookup key and the loop's temporary share that frame word; the
//        found shop leaves the loop by `break` and both "no shop" exits skip SetShopKeeper.
extern "C" void free(void* p);                  // 0x00BFEA14
class CTCBase;
class CBaseObject;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing;
struct CBaseIntelligentPointer {                // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                       // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    void operator=(const CIntelligentPointer& other) { CBaseObject* item = (CBaseObject*)other.GetPItem(); SetPItem(item); }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x170 - 0x92];
    CBaseIntelligentPointer ShopThing;          // +0x170 (creature's shop thing; name unverified)
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingCreatureBase;

template <class T> struct vector {              // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) free(_M_start); }
};
class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingSearchTools { public:
    void GetAllThings(const CThingFilter_HasTC& filter, vector<CThing*>& out);   // 0x00799FB0
};

class CTCShop { public:
    virtual void v00();
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    bool                    ToBeRemoved;        // +0x08
    bool                    IsInGlobalUpdateTCs;// +0x09
    long                    StockState;         // +0x0c
    CBaseIntelligentPointer PShopKeeper;        // +0x10
    void SetShopKeeper(CThing* keeper);         // 0x007751A0 (CThingCreatureBase*)
};

class CTCShopKeeper { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<CThingCreatureBase> Customer;   // +0x0c
    CIntelligentPointer<CThing> Shop;           // +0x14
    CTCShop* GetShopTC() const;                 // 0x00798F50
    CThing* GetCreature() const { return ParentThing; }
    CThingSearchTools* GetThingSearchTools() const;   // 0x00686D90 (CTCBase)
    void CTCBase_OnInitialActivate();           // 0x00686870 (CTCBase::OnInitialActivate)
    void OnInitialActivate();
};

void CTCShopKeeper::OnInitialActivate()
{
    Shop = CIntelligentPointer<CThing>(GetCreature()->ShopThing.GetPItem());
    CTCShop* shop = GetShopTC();
    if (!shop) {
        vector<CThing*> shops;
        GetThingSearchTools()->GetAllThings(CThingFilter_HasTC(0x53), shops);
        for (CThing** it = shops._M_start; it != shops._M_finish; ++it) {
            CTCShop* candidate = 0;
            if ((*it)->HasTC(0x53))
                candidate = (CTCShop*)(*it)->GetTC(0x53);
            CThing* keeper = candidate->PShopKeeper.GetPItem();
            if (!keeper || (keeper->m_flags91 & 1)) {
                Shop = CIntelligentPointer<CThing>(*it);
                shop = candidate;
                break;
            }
        }
    }
    if (shop)
        shop->SetShopKeeper(ParentThing);
    CTCBase_OnInitialActivate();
}