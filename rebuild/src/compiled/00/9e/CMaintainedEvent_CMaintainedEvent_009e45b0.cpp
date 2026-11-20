// CMaintainedEvent::CMaintainedEvent() -- base CBaseClass(), own vtable, then the CInputEvent member Event at +4 built by its out-of-line constructor
// Leaf: ??0CMaintainedEvent@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E45B0, length 26 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0, CInputEvent::CInputEvent @ 0x00A04410 (??0CInputEvent@@QAE@XZ, Ego_r.pdb + FableWin.pdb;
// it zeroes the event; its manifest label "CDefaultEngineMemoryInit" is a propagated name). This class's vtable is 0x0129C0E0
// (rtti-vtables.tsv: CMaintainedEvent). Base, vtable, member, return `this`.
// agentNF182 (2026-11-20, q4302): ported onto the shared engine/CMaintainedEvent.h (Ego_r layout: CBaseClass +0, CInputEvent Event +4,
// opt-in FABLE_RETAIL_CMAINTAINEDEVENT_CTOR declares the constructor); was a local CMaintainedEvent with a CEventRecord stand-in member.
#define FABLE_RETAIL_CMAINTAINEDEVENT_CTOR
#include "engine/CMaintainedEvent.h"                   // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0); CInputEvent 0x00A04410

CMaintainedEvent::CMaintainedEvent()
    : CBaseClass()
    , Event()
{
}
