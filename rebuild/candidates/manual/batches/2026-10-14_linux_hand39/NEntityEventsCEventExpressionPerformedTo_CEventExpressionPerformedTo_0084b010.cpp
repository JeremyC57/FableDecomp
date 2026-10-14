// NEntityEvents::CEventExpressionPerformedTo::CEventExpressionPerformedTo(void* target, long expression, const CIVCountedPointer& data, long a, long b) -- the event base (type 0), this class's vtable, the intelligent pointer at +8 set to the target, the counted data copied at +0x10 (reference added), the three words at +0x14 / +0x18 / +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B010, length 87 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 (on +8; the CIntelligentPointer vtable 0x0123E7D8 stored between
// them). Vtable 0x01274240 = NEntityEvents::CEventExpressionPerformedTo (rtti-vtables.tsv). Shape: the base's type word precedes the vtable, the counted copy is
// the inline `test ; mov ; je ; inc [eax+4]`, the three trailing words are loaded together; `ret 14h`.
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
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +0x04
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
        }
    }
};

namespace NEntityEvents {
struct CEventExpressionPerformedTo : CEventBase_NEntityEvents {   // vtable 0x01274240
    CIntelligentPointer m_target;                  // +0x08
    CIVCountedPointer   m_data;                    // +0x10
    long                m_expression;              // +0x14
    long                m_a;                       // +0x18
    long                m_b;                       // +0x1c
    virtual void v00();
    CEventExpressionPerformedTo(void* target, long expression, const CIVCountedPointer& data, long a, long b);
};
}

NEntityEvents::CEventExpressionPerformedTo::CEventExpressionPerformedTo(void* target, long expression, const CIVCountedPointer& data, long a, long b)
    : CEventBase_NEntityEvents()
    , m_target(target)
    , m_data(data)
    , m_expression(expression)
    , m_a(a), m_b(b)
{
}
