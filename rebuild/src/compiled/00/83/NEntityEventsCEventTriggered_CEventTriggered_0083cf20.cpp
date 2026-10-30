// NEntityEvents::CEventTriggered::CEventTriggered(CThing&) -- the event starts at world frame 0 and its TriggeredBy intelligent pointer is pointed at the argument
// Leaf: ??0CEventTriggered@NEntityEvents@@QAE@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083CF20, 52 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NEntityEvents::CEventTriggered::CEventTriggered(CThing&) -- proven; evidence: it stores vtable 0x01273014 whose RTTI is
//   .?AVCEventTriggered@NEntityEvents@@ : CEventBase@NEntityEvents; FableWin.pdb name set holds ??0CEventTriggered@NEntityEvents@@QAE@AAVCThing@@@Z;
//   Ego_r.pdb NEntityEvents::CEventTriggered is 16 bytes with CIntelligentPointer<CThing> TriggeredBy at +8. Family G038 (8 event
//   constructors that differ in the class, its vtable and the member).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90. Vtables: CEventTriggered 0x01273014, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: CEventBase's inline constructor stores WorldFrame = 0; its own vtable store is dead (overwritten by the class
//   vtable), so WorldFrame lands before the class vptr. The member's inline (T*) constructor runs the out-of-line base
//   constructor, stores its own vtable, then SetPItem.
// Model: local -- the generated engine headers flatten the vptr into a word, which cannot carry these vtables.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer() {}
    virtual T* GetPItem() const;
};
namespace NEntityEvents {
class CEventBase {
public:
    CEventBase() : WorldFrame(0) {}
    virtual ~CEventBase() {}
    long WorldFrame;                                         // +0x04
};
class CEventTriggered : public CEventBase {
public:
    CEventTriggered(CThing& thing);
    CIntelligentPointer<CThing> TriggeredBy;            // +0x08
};

CEventTriggered::CEventTriggered(CThing& thing)
    : TriggeredBy(&thing)
{
}
}