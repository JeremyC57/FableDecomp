// CTCShopKeeper::AcquireNewCustomer() -- look for the nearest potential customer in the 30m square around the keeper (nearest to the shop); latch it as the current customer and remember it, else when nobody (not even an existing shopper) is around, empty the customer list
// Leaf: ?AcquireNewCustomer@CTCShopKeeper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799900, 351 bytes (plain `ret`).
// Callees: CTCShopKeeper::GetShopTC 0x00798F50; CThing::GetPosition 0x004C73D0 (x2); __ftol2 0x00BFEA70 (x2);
//          CThingFilter_IsPotentialCustomer(CTCShopKeeper*, bool) 0x00799D80 (x2); CTCBase::GetThingSearchTools 0x00686D90;
//          CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer, CThingCompare_Nearest,
//          CThingCreatureBase> 0x0079A050; CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_IsPotentialCustomer,
//          CThingCreatureBase> 0x0079A280 (manifest label is a propagated twin); CBaseIntelligentPointer ctor 0x00A01B10 /
//          SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThingCreatureBase> vtable
//          0x0123E7D8); CTCShopKeeper::AddNewCustomer(CIntelligentPointer) 0x00798D90; CTCShopKeeper::EmptyCustomerList 0x00798E10.
// Layout (Ego_r.pdb CTCShopKeeper, 72 bytes; retail as the landed GetShopTC/AddNewCustomer): ParentThing +0x04 (CTCBase),
//          Customer +0x0c; CThingFilter_IsPotentialCustomer 24 bytes; CThingCompare_Nearest 12 bytes (Pos).
// Shape: the shop position is a named copy taken before the keeper's own position; the box is a named C2DBoxI built from
//          two truncated coordinates (+/-15); the best-thing result is ignored and the out pointer latched through a
//          temporary; the by-value AddNewCustomer argument is copy-constructed in its stack slot.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
class CBaseObject;
class CThing { public:
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
class CThingCreatureBase;
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +4
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    void SetPItem(CBaseObject* item);              // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    void operator=(const CIntelligentPointerCreature& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); }
};
class CTCShop { public:
    void* __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
};
class CTCShopKeeper;
class CThingFilter_IsPotentialCustomer { public:
    CThing* ShopperContainer;                      // +0x00
    C3DVector Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                   // +0x10
    bool AllowExistingShopper;                     // +0x14
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper);   // 0x00799D80
};
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools { public:
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_IsPotentialCustomer& filter,
                                  const CThingCompare_Nearest& compare, CThingCreatureBase** out);   // 0x0079A050
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_IsPotentialCustomer& filter,
                                   CThingCreatureBase** out);                                         // 0x0079A280
};
class CTCShopKeeper { public:
    void* __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CIntelligentPointerCreature Customer;          // +0x0c
    CThingSearchTools* GetThingSearchTools() const;   // 0x00686D90 (CTCBase)
    CTCShop* GetShopTC();                          // 0x00798F50
    void AddNewCustomer(CIntelligentPointerCreature customer);   // 0x00798D90
    void EmptyCustomerList();                      // 0x00798E10
    void AcquireNewCustomer();
};

void CTCShopKeeper::AcquireNewCustomer()
{
    CTCShop* shop = GetShopTC();
    if (!shop)
        return;
    C3DVector shopPos = shop->ParentThing->GetPosition();
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 15, y - 15, x + 15, y + 15);
    CThingCreatureBase* customer = 0;
    GetThingSearchTools()->GetPBestThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, false),
        CThingCompare_Nearest(shopPos), &customer);
    Customer = CIntelligentPointerCreature(customer);
    if (Customer.GetPItem())
        AddNewCustomer(Customer);
    else if (!GetThingSearchTools()->GetPFirstThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, true), &customer))
        EmptyCustomerList();
}