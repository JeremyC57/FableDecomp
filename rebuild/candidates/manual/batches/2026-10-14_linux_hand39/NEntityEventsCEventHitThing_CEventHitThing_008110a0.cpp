// NEntityEvents::CEventHitThing::CEventHitThing(void*, void* thing, bool a, bool b, bool c, void*, bool d, bool e) -- the event base (type 0), this class's vtable, the intelligent pointer at +8 set to the thing, five flag bytes at +0x10..+0x14 from the third, fourth, fifth, seventh and eighth arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008110A0, length 85 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 (on +8; the CIntelligentPointer vtable 0x01238C6C stored between
// them). Vtable 0x01272390 = NEntityEvents::CEventHitThing (rtti-vtables.tsv). Shape: the five bytes are loaded into cl/dl/al in pairs and stored in order; the
// first and sixth arguments are unused; `ret 20h`.
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
struct CEventHitThing : CEventBase_NEntityEvents { // vtable 0x01272390
    CIntelligentPointer m_thing;                   // +0x08
    bool                m_a;                       // +0x10
    bool                m_b;                       // +0x11
    bool                m_c;                       // +0x12
    bool                m_d;                       // +0x13
    bool                m_e;                       // +0x14
    virtual void v00();
    CEventHitThing(void* unused1, void* thing, bool a, bool b, bool c, void* unused6, bool d, bool e);
};
}

NEntityEvents::CEventHitThing::CEventHitThing(void* unused1, void* thing, bool a, bool b, bool c, void* unused6, bool d, bool e)
    : CEventBase_NEntityEvents()
    , m_thing(thing)
    , m_a(a), m_b(b), m_c(c), m_d(d), m_e(e)
{
}
