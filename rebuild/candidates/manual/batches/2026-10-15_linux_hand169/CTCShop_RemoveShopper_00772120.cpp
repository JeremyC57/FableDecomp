// CTCShop::RemoveShopper(CThingCreatureBase* shopper) -- find the shopper in Shoppers, overwrite its slot with the last entry and pop the back
// Leaf: RemoveShopper
// Retail: Fable.exe 0x00772120, 124 bytes (`ret 4`, the pop_back destructor as a `mov [esp+4],0 ; jmp [eax]` tail call).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), SetPItem @ 0x00A01B90, the element's virtual slot 0
// (scalar deleting dtor, flag 0 = STLport _Destroy's explicit ~T()).
// Layout: retail CTCShop (Ego_r.pdb less 4 bytes per debug vector): StockState +0x0C, PShopKeeper +0x10, Shoppers
// (vector<CIntelligentPointer<CThingCreatureBase>>, 8-byte elements) +0x18, ShopKeeperAvailable +0x24.
// Shape: unsigned index against size() recomputed each pass; `v[i] = v[size() - 1]` (begin + n*8 - 8) then pop_back.
struct CBaseObject;
struct CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();             // slot 0
    CBaseObject* m_pItem;                           // +4
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {    // CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CShopperVector {                             // STLport vector<CIntelligentPointer<CThingCreatureBase>>
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    CIntelligentPointer* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    CIntelligentPointer& operator[](unsigned long n) { return *(begin() + n); }
    void pop_back() { --_M_finish; _M_finish->~CIntelligentPointer(); }
};
struct CTCShop {
    virtual void v00();
    void*               ParentThing;                // +0x04 (CTCBase)
    bool                ToBeRemoved;                // +0x08
    bool                IsInGlobalUpdateTCs;        // +0x09
    long                StockState;                 // +0x0c
    CIntelligentPointer PShopKeeper;                // +0x10
    CShopperVector      Shoppers;                   // +0x18
    bool                ShopKeeperAvailable;        // +0x24
    void RemoveShopper(CThingCreatureBase* shopper);
};

void CTCShop::RemoveShopper(CThingCreatureBase* shopper)
{
    for (unsigned long i = 0; i < Shoppers.size(); ++i) {
        if (Shoppers[i].GetPItem() == (CBaseObject*)shopper) {
            Shoppers[i] = Shoppers[Shoppers.size() - 1];
            Shoppers.pop_back();
            return;
        }
    }
}
