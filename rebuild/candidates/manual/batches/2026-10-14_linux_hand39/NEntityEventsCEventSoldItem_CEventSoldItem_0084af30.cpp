// NEntityEvents::CEventSoldItem::CEventSoldItem(void* giver, void* item, bool ownsItem) -- the event base (type 0), this class's vtable, the two intelligent pointers at +8 / +0x10 set to the giver and the item, the ownership byte at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084AF30, length 85 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 twice (the CIntelligentPointer vtable 0x01238C6C stored between
// them each time). Vtable 0x01272378 = NEntityEvents::CEventSoldItem (rtti-vtables.tsv). Shape: the base's type word precedes the vtable, each pointer's address is
// formed with `lea edi`; the byte is loaded into dl last; `ret 0Ch`.
struct __declspec(novtable) CEventBase_NEntityEvents {   // NEntityEvents::CEventBase (vtable 0x0124F2C4, never stored by a derived constructor)
    virtual void v00();
    long m_eventType;                              // +0x04
    CEventBase_NEntityEvents() : m_eventType(0) { }
};

struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    void SetPItem(void* item);                     // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(void* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

namespace NEntityEvents {
struct CEventSoldItem : CEventBase_NEntityEvents {        // vtable 0x01272378
    CIntelligentPointer m_giver;                   // +0x08
    CIntelligentPointer m_item;                    // +0x10
    bool                m_ownsItem;                // +0x18
    virtual void v00();
    CEventSoldItem(void* giver, void* item, bool ownsItem);
};
}

NEntityEvents::CEventSoldItem::CEventSoldItem(void* giver, void* item, bool ownsItem)
    : CEventBase_NEntityEvents()
    , m_giver(giver)
    , m_item(item)
    , m_ownsItem(ownsItem)
{
}
