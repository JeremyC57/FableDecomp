// CEvent_009825c0::~CEvent_009825c0() -- releases the counted pointee at +0x14, destroys the (byte-packed) intelligent pointer at +9, then the inlined NEntityEvents::CEventBase destructor restores its vtable (0x0124F2C4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009825C0, length 43 bytes.
// Callees: CIVCountedPointeeBase slot 1 (Delete) at zero count; CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Same shape as 0x0084B690 (the manifest label is propagated), but this event is byte-packed:
// the CBaseIntelligentPointer sits at +9 behind a one-byte member (`lea ecx,[esi+9]`), three
// more bytes follow it and the CIVCountedPointer is at +0x14. The real layout therefore needs
// `#pragma pack(1)`; the members are named descriptively. The most-derived class is novtable
// (no vtable store of its own); members are destroyed in reverse order, then the inline empty
// base destructor writes the base vtable.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

#pragma pack(push, 1)
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    ~CIVCountedPointer()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

namespace NEntityEvents {
class CEventBase { public:                        // vtable 0x0124F2C4
    virtual void v00();
    long m_eventType;                      // +4
    ~CEventBase() { }
};
}

struct __declspec(novtable) CEvent_009825c0 : NEntityEvents::CEventBase {
    unsigned char m_flag;                  // +8
    CBaseIntelligentPointer m_target;      // +9 (packed)
    unsigned char m_kind[3];               // +0x11
    CIVCountedPointer m_data;              // +0x14
    ~CEvent_009825c0();
};
#pragma pack(pop)

CEvent_009825c0::~CEvent_009825c0()
{
}