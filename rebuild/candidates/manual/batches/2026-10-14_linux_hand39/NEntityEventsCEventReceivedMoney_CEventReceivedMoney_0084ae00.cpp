// NEntityEvents::CEventReceivedMoney::CEventReceivedMoney(void* giver, long amount) -- the event base (type 0), this class's vtable, the intelligent pointer at +8 set to the giver, the amount at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084AE00, length 59 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 (on +8; the CIntelligentPointer vtable 0x01238C6C stored between
// them). Vtable 0x01274210 = NEntityEvents::CEventReceivedMoney (rtti-vtables.tsv). Shape: the base's type word precedes the vtable; `ret 8`.
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
struct CEventReceivedMoney : CEventBase_NEntityEvents {   // vtable 0x01274210
    CIntelligentPointer m_giver;                   // +0x08
    long                m_amount;                  // +0x10
    virtual void v00();
    CEventReceivedMoney(void* giver, long amount);
};
}

NEntityEvents::CEventReceivedMoney::CEventReceivedMoney(void* giver, long amount)
    : CEventBase_NEntityEvents()
    , m_giver(giver)
    , m_amount(amount)
{
}
