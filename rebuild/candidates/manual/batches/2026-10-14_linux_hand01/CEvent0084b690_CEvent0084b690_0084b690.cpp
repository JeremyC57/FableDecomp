// CEvent_0084b690::~CEvent_0084b690() -- releases the counted pointee at +0x10, destroys the intelligent pointer at +8, then the inlined NEntityEvents::CEventBase destructor restores its vtable (0x0124F2C4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B690, length 43 bytes.
// Callees: CIVCountedPointeeBase slot 1 (Delete) at zero count; CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// The manifest label (C3DAnimationBlendState::~C3DAnimationBlendState) is a propagated name: the
// vtable written at the end is NEntityEvents::CEventBase's (rtti-vtables.tsv), so this is the
// destructor of an event class derived from it. The most-derived class stores no vtable of its
// own (novtable); its members are destroyed in reverse declaration order -- the CIVCountedPointer
// (+0x10, inline release) then the CBaseIntelligentPointer (+8, out of line) -- and finally the
// inline empty base destructor writes the base vtable.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1
    unsigned long IVRefCount;              // +4
};

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

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                         // +4
    ~CBaseIntelligentPointer();            // 0x00A01C10
};

namespace NEntityEvents {
struct CEventBase {                        // vtable 0x0124F2C4
    virtual void v00();
    long m_eventType;                      // +4
    ~CEventBase() { }
};
}

struct __declspec(novtable) CEvent_0084b690 : NEntityEvents::CEventBase {
    CBaseIntelligentPointer m_target;      // +8
    CIVCountedPointer m_data;              // +0x10
    ~CEvent_0084b690();
};

CEvent_0084b690::~CEvent_0084b690()
{
}
