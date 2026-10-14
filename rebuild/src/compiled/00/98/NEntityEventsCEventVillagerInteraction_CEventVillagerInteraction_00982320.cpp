// NEntityEvents::CEventVillagerInteraction::CEventVillagerInteraction(void* villager, const CIVCountedPointer& data, long a) -- the event base (type 0), this class's vtable, a cleared byte at +8, the intelligent pointer at +9 (the class is byte-packed) set to the villager, the counted data copied at +0x14, a cleared byte at +0x18, the sentinel 0x7fffffff at +0x1c, `a` at +0x20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982320, length 88 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 and SetPItem @ 0x00A01B90 (on +9; the CIntelligentPointer vtable 0x0123E7E0 stored between
// them). Vtable 0x012995A8 = NEntityEvents::CEventVillagerInteraction (rtti-vtables.tsv). Shape: `lea edi,[esi+9]`: the intelligent pointer sits at an odd offset
// (`#pragma pack(1)`), the counted copy is the inline add-ref, the trailing stores are immediates; `ret 0Ch`. The chunk label "CEventExpressionPerformedTo" is
// propagated.
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

#pragma pack(push, 1)
namespace NEntityEvents {
struct CEventVillagerInteraction : CEventBase_NEntityEvents {   // vtable 0x012995A8; byte-packed
    unsigned char       m_flag;                    // +0x08
    CIntelligentPointer m_villager;                // +0x09 .. +0x11
    unsigned char       _pad11[3];                 // +0x11 .. +0x14 (not initialised)
    CIVCountedPointer   m_data;                    // +0x14
    unsigned char       m_state;                   // +0x18
    unsigned char       _pad19[3];                 // +0x19 .. +0x1c (not initialised)
    long                m_sentinel;                // +0x1c
    long                m_a;                       // +0x20
    virtual void v00();
    CEventVillagerInteraction(void* villager, const CIVCountedPointer& data, long a);
};
}
#pragma pack(pop)

NEntityEvents::CEventVillagerInteraction::CEventVillagerInteraction(void* villager, const CIVCountedPointer& data, long a)
    : CEventBase_NEntityEvents()
    , m_flag(0)
    , m_villager(villager)
    , m_data(data)
    , m_state(0)
    , m_sentinel(0x7fffffff)
    , m_a(a)
{
}