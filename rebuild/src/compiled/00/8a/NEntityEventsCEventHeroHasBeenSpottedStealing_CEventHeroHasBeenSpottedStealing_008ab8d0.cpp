// NEntityEvents::CEventHeroHasBeenSpottedStealing::CEventHeroHasBeenSpottedStealing(CThing* target, CThing* spotter) -- a spotted-stealing event: the target and the spotter; the event starts at world frame 0 and both intelligent pointers are pointed at the arguments
// Leaf: ??0CEventHeroHasBeenSpottedStealing@NEntityEvents@@QAE@PAVCThing@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AB8D0, 80 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): NEntityEvents::CEventHeroHasBeenSpottedStealing::CEventHeroHasBeenSpottedStealing(CThing*, CThing*); evidence: it stores vtable 0x012781E0 whose RTTI is
//   .?AVCEventHeroHasBeenSpottedStealing@NEntityEvents@@ : CEventBase@NEntityEvents; Ego_r.pdb ??0CEventHeroHasBeenSpottedStealing@NEntityEvents@@QAE@PAVCThing@@0@Z; Ego_r.pdb NEntityEvents::CEventHeroHasBeenSpottedStealing is 24 bytes
//   with CIntelligentPointer<CThing> PTarget +0x08 and PSpotter +0x10, set from the first and second argument; callers: CGameScriptInterface::SendEntityEvent 0x008957C0.
// Label: the manifest's "CEventDied" is propagated (family J108: five byte-identical two-pointer event constructors; retail has
//   no identical-code folding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (Ego_r.pdb ??0CBaseIntelligentPointer@@QAE@XZ),
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90 (each twice). Vtables: CEventHeroHasBeenSpottedStealing 0x012781E0,
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
class CEventHeroHasBeenSpottedStealing : public CEventBase {   // Ego_r.pdb 24 bytes
public:
    CEventHeroHasBeenSpottedStealing(CThing* target, CThing* spotter);
    CIntelligentPointer<CThing> PTarget;                  // +0x08
    CIntelligentPointer<CThing> PSpotter;                  // +0x10
};

CEventHeroHasBeenSpottedStealing::CEventHeroHasBeenSpottedStealing(CThing* target, CThing* spotter)
    : PTarget(target), PSpotter(spotter)
{
}
}   // namespace NEntityEvents