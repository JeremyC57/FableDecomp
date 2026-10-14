// CMessageEvent::CMessageEvent() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687270, length 52 bytes (+ int3 padding).
// Callees: CBaseObject::CBaseObject() @ 0x0099A380 (stores vtable 0x0129A7D8 and zeroes +4),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 twice (members at +0x48 and
// +0x50, each followed by the inline CIntelligentPointer vtable store 0x01238C6C).
// Vtable 0x0125BE8C is CMessageEvent (rtti-vtables.tsv); the manifest label
// (CAIStateGroup_BalverineCornered ctor) is propagated garbage. Each pointer member's address
// is kept in edi across its base constructor call.
struct CBaseObject {                        // 8 bytes: vptr + the reference count zeroed by the ctor
    virtual void v00();
    long m_refCount;                        // +4
    CBaseObject();                          // 0x0099A380
};

struct CBaseIntelligentPointer {         // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {     // vtable 0x01238C6C
    CIntelligentPointer() { }
};

struct CMessageEvent : CBaseObject {        // vtable 0x0125BE8C
    unsigned char _pad[0x40];               // +0x08..+0x47 (message payload)
    CIntelligentPointer m_pSender;          // +0x48
    CIntelligentPointer m_pTarget;          // +0x50
    CMessageEvent();
};

CMessageEvent::CMessageEvent()
    : m_pSender()
    , m_pTarget()
{
}