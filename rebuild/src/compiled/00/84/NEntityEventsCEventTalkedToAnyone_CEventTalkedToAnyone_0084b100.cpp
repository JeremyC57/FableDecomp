// NEntityEvents::CEventTalkedToAnyone::CEventTalkedToAnyone(CThing&) -- the event starts at world frame 0 and its PThingTalkedTo intelligent pointer is pointed at the argument
// Leaf: ??0CEventTalkedToAnyone@NEntityEvents@@QAE@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B100, 52 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NEntityEvents::CEventTalkedToAnyone::CEventTalkedToAnyone(CThing&) -- proven; evidence: it stores vtable 0x01274270 whose RTTI is
//   .?AVCEventTalkedToAnyone@NEntityEvents@@ : CEventBase@NEntityEvents; FableWin.pdb name set holds ??0CEventTalkedToAnyone@NEntityEvents@@QAE@AAVCThing@@@Z;
//   Ego_r.pdb NEntityEvents::CEventTalkedToAnyone is 16 bytes with CIntelligentPointer<CThing> PThingTalkedTo at +8. Family G038 (8 event
//   constructors that differ in the class, its vtable and the member).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90. Vtables: CEventTalkedToAnyone 0x01274270, CIntelligentPointer<CThing> 0x01238C6C.
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
class CEventTalkedToAnyone : public CEventBase {
public:
    CEventTalkedToAnyone(CThing& thing);
    CIntelligentPointer<CThing> PThingTalkedTo;            // +0x08
};

CEventTalkedToAnyone::CEventTalkedToAnyone(CThing& thing)
    : PThingTalkedTo(&thing)
{
}
}