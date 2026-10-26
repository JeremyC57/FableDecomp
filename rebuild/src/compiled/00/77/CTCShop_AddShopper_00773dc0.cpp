// CTCShop::AddShopper(CThingCreatureBase*) -- (re)registers a shopper: removes any earlier entry, then appends an intelligent pointer to the shopper to the shopper list at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00773DC0, length 74 bytes (+6 int3 padding).
// Callees: CTCShop::RemoveShopper(CThingCreatureBase*) @ 0x00772120, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10, the shopper container's push_back
// (const CIntelligentPointer&) @ 0x0074DA40; the temporary's vtable is the CIntelligentPointer copy 0x0123E7D8.
// Shape: the shopper argument is cached in edi for both uses; the temporary is a named stack CIntelligentPointer.
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThingCreatureBase;

struct CShopperList {
    void PushBack(const CIntelligentPointer& shopper);   // 0x0074DA40
};

class CTCShop { public:
    unsigned char _pad[0x18];
    CShopperList  m_shoppers;                // +0x18
    void RemoveShopper(CThingCreatureBase* shopper);     // 0x00772120
    void AddShopper(CThingCreatureBase* shopper);
};

void CTCShop::AddShopper(CThingCreatureBase* shopper)
{
    RemoveShopper(shopper);
    CIntelligentPointer pointer;
    pointer.SetPItem((CBaseObject*)shopper);
    m_shoppers.PushBack(pointer);
}