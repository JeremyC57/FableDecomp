// CMessageEvent::CMessageEvent() -- default constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687270, length 52 bytes (+ int3 padding).
// Callees: CBaseObject::CBaseObject() @ 0x0099A380 (stores vtable 0x0129A7D8 and zeroes +4),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 twice (members at +0x48 and
// +0x50, each followed by the inline CIntelligentPointer vtable store 0x01238C6C).
// Vtable 0x0125BE8C is CMessageEvent (rtti-vtables.tsv); the manifest label
// (CAIStateGroup_BalverineCornered ctor) is propagated garbage. Each pointer member's address
// is kept in edi across its base constructor call.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

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

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {     // vtable 0x01238C6C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CMessageEvent : public CBaseObject { public:        // vtable 0x0125BE8C
    unsigned char _pad[0x40];               // +0x08..+0x47 (message payload)
    CIntelligentPointer<CThing> m_pSender;          // +0x48
    CIntelligentPointer<CThing> m_pTarget;          // +0x50
    CMessageEvent();
};

CMessageEvent::CMessageEvent()
    : m_pSender()
    , m_pTarget()
{
}