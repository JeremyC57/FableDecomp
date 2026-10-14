// CMaintainedEvent::CMaintainedEvent() -- base CBase(), own vtable, the 0x2c-byte event record at +4 cleared by its out-of-line constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E45B0, length 26 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, the member constructor @ 0x00A04410 (zeroes eleven dwords
// +0x00..+0x28 of the record; its manifest label "CDefaultEngineMemoryInit" is a propagated
// name, so the member is named by what it does). This class's vtable is 0x0129C0E0
// (rtti-vtables.tsv: CMaintainedEvent). Base, vtable, member, return `this`.
struct CBase {
    virtual void v00();
    CBase();                                // 0x0099A2D0
};

struct CEventRecord {                       // 0x2c bytes, zeroed out of line
    unsigned char _body[0x2c];
    CEventRecord();                         // 0x00A04410
};

struct CMaintainedEvent : CBase {           // vtable 0x0129C0E0
    CEventRecord m_record;                  // +0x04
    virtual void v00();
    CMaintainedEvent();
};

CMaintainedEvent::CMaintainedEvent()
    : CBase()
    , m_record()
{
}
