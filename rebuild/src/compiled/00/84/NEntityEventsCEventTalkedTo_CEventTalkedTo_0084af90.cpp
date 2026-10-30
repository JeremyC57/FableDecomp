// NEntityEvents::CEventTalkedTo::CEventTalkedTo(CThing&) -- the event starts at world frame 0 and its TalkedToBy intelligent pointer is pointed at the argument
// Leaf: ??0CEventTalkedTo@NEntityEvents@@QAE@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084AF90, 52 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NEntityEvents::CEventTalkedTo::CEventTalkedTo(CThing&) -- proven; evidence: it stores vtable 0x01274228 whose RTTI is
//   .?AVCEventTalkedTo@NEntityEvents@@ : CEventBase@NEntityEvents; FableWin.pdb name set holds ??0CEventTalkedTo@NEntityEvents@@QAE@AAVCThing@@@Z;
//   Ego_r.pdb NEntityEvents::CEventTalkedTo is 16 bytes with CIntelligentPointer<CThing> TalkedToBy at +8. Family G038 (8 event
//   constructors that differ in the class, its vtable and the member).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90. Vtables: CEventTalkedTo 0x01274228, CIntelligentPointer<CThing> 0x01238C6C.
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
class CEventTalkedTo : public CEventBase {
public:
    CEventTalkedTo(CThing& thing);
    CIntelligentPointer<CThing> TalkedToBy;            // +0x08
};

CEventTalkedTo::CEventTalkedTo(CThing& thing)
    : TalkedToBy(&thing)
{
}
}