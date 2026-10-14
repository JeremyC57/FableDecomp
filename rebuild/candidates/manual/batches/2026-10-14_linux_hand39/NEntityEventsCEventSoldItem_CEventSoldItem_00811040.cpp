// NEntityEvents::CEventSoldItem::~CEventSoldItem() -- this class's vtable; when the event owns the item (+0x18) and the item at +0x10 is live and not flagged (bit 0 of +0x91), kill it; then the two intelligent pointers at +0x10 / +8 destroyed out of line and the CEventBase vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00811040, length 88 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on +0x10: out of line each time), CThing::Kill @ 0x004C9B80 (on the item, 0),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 twice. Vtables 0x01272378 = NEntityEvents::CEventSoldItem, 0x0124F2C4 = CEventBase (rtti-vtables.tsv).
// Shape: the ownership byte through al before the vtable store's `je`, the pointer address kept in edi across the three accessor calls; members destroyed in
// reverse order, the inlined base destructor last.
struct CThing_00811040 {
    unsigned char _pad[0x91];
    unsigned char m_flags;                         // +0x91
    void Kill(long reason);                        // 0x004C9B80
};

struct CBaseIntelligentPointer {                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                 // +0x04
    CThing_00811040* GetPItem();                      // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
};

namespace NEntityEvents {
struct CEventBase {                                // vtable 0x0124F2C4
    virtual void v00();
    long m_eventType;                              // +0x04
    ~CEventBase() { }
};

struct CEventSoldItem : CEventBase {                      // vtable 0x01272378
    CIntelligentPointer m_giver;                   // +0x08
    CIntelligentPointer m_item;                    // +0x10
    bool                m_ownsItem;                // +0x18
    virtual void v00();
    ~CEventSoldItem();
};
}

NEntityEvents::CEventSoldItem::~CEventSoldItem()
{
    if (m_ownsItem) {
        if (m_item.GetPItem()) {
            if (!(m_item.GetPItem()->m_flags & 1))
                m_item.GetPItem()->Kill(0);
        }
    }
}
