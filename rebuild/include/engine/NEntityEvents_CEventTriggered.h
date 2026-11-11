#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTTRIGGERED_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTTRIGGERED_H
// NEntityEvents::CEventTriggered (new, agentNF12, 2026-11-11, row 0x0083DDC0 CThingPhysicalSwitch::TriggerMe): RTTI
// .?AVCEventTriggered@NEntityEvents@@ : NEntityEvents::CEventBase (ghidra_out/rtti_bases.tsv), vtable 0x01273014. Ego_r.pdb
// struct_layouts_egor.tsv: 16 bytes, TriggeredBy (CIntelligentPointer<CThing>) at +8 after CEventBase { vptr, WorldFrame } (retail
// `push 10h ; call operator new`, 0x0083DE0A). Constructor: Ego_r.pdb / FableWin.pdb ??0CEventTriggered@NEntityEvents@@QAE@AAVCThing@@@Z
// (retail 0x0083CF20, out of line). Needs FABLE_RETAIL_CINTELLIGENTPOINTER.
#include "rebuild_abi.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NEntityEvents {
class CEventTriggered : public CEventBase {
public:
    CEventTriggered(CThing& triggered_by);                   // 0x0083CF20
    virtual ~CEventTriggered();
    CIntelligentPointer<CThing> TriggeredBy;                 // +0x08
};
} // namespace NEntityEvents
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventTriggered) == 0x10);
#endif // FABLE_ENGINE_NENTITYEVENTS_CEVENTTRIGGERED_H
