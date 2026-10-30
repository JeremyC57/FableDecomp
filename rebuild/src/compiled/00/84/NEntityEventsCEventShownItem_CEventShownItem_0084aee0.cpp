// NEntityEvents::CEventShownItem::CEventShownItem(CThing& thingShowingItem, CThing& itemShown) -- a shown-item event: the thing showing the item and the item; the event starts at world frame 0 and both intelligent pointers are pointed at the arguments
// Leaf: ??0CEventShownItem@NEntityEvents@@QAE@AAVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084AEE0, 80 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): NEntityEvents::CEventShownItem::CEventShownItem(CThing&, CThing&); evidence: it stores vtable 0x012723D8 whose RTTI is
//   .?AVCEventShownItem@NEntityEvents@@ : CEventBase@NEntityEvents; Ego_r.pdb ??0CEventShownItem@NEntityEvents@@QAE@AAVCThing@@0@Z; Ego_r.pdb NEntityEvents::CEventShownItem is 24 bytes
//   with CIntelligentPointer<CThing> ThingShowingItem +0x08 and ItemShown +0x10, set from the first and second argument; callers: CCreatureAction_ShowItemToThing::PerformAction 0x00845270.
// Label: the manifest's "CEventDied" is propagated (family J108: five byte-identical two-pointer event constructors; retail has
//   no identical-code folding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (Ego_r.pdb ??0CBaseIntelligentPointer@@QAE@XZ),
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90 (each twice). Vtables: CEventShownItem 0x012723D8,
//   CIntelligentPointer<CThing> 0x01238C6C (one slot: its deleting destructor).
// Shape: the agent951 G038 model (0x0084B080): CEventBase's inline constructor stores WorldFrame = 0 (its vptr store is dead);
//   each member's inline (T*) constructor runs the out-of-line base constructor, stores its own vtable, then SetPItem.
// Model: local -- the event classes have no engine headers and the intelligent-pointer chain needs its real vtables.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };                      // partial view (RTTI CThing <- CClass <- CBaseObject <- CBaseClass)
class CBaseIntelligentPointer {                             // 8 bytes
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};                                                          // implicit destructor (its vtable's one slot)
namespace NEntityEvents {
class CEventBase {                                          // Ego_r.pdb 8 bytes
public:
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase() { }
    long WorldFrame;                                        // +0x04
};
class CEventShownItem : public CEventBase {   // Ego_r.pdb 24 bytes
public:
    CEventShownItem(CThing& thingShowingItem, CThing& itemShown);
    CIntelligentPointer<CThing> ThingShowingItem;                  // +0x08
    CIntelligentPointer<CThing> ItemShown;                  // +0x10
};

CEventShownItem::CEventShownItem(CThing& thingShowingItem, CThing& itemShown)
    : ThingShowingItem(&thingShowingItem), ItemShown(&itemShown)
{
}
}   // namespace NEntityEvents