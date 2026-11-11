// CTCEntityEvents::FrameUpdate() -- drop the entity events whose lifetime has run out at the current world frame
// Leaf: ?FrameUpdate@CTCEntityEvents@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811450, 24 bytes, plain `ret`.
// Identity (agentNF14 2026-11-11; the catalog name, landed as a free __fastcall stub over invented CBaseEntityEvents / CSubObject
// stand-ins): Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCEntityEvents@@UAEXXZ (the CTCBase slot-9 override); its one callee is the landed
// ??$remove_if@VCPred_EventExpired@NEntityEvents@@@?$list@V?$CCountedPointer@VCEventBase@NEntityEvents@@@@... 0x00812030 on Events
// +0x0c (FableWin.pdb names that instantiation), its one-word predicate (Ego_r.pdb CPred_EventExpired { long WorldFrame; },
// ??0CPred_EventExpired@NEntityEvents@@QAE@J@Z inline) built from the world frame.
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused), CWorld::GetFrame() @ 0x0049D870 (static __fastcall),
// list<CCountedPointer<NEntityEvents::CEventBase> >::remove_if<NEntityEvents::CPred_EventExpired> @ 0x00812030.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_FRAMEUPDATE
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CWorld.h"

namespace NEntityEvents {
class CPred_EventExpired {                                  // Ego_r.pdb: WorldFrame +0 (operator() is out of line in 0x00812030)
public:
    CPred_EventExpired(long worldFrame) : WorldFrame(worldFrame) { }   // ??0CPred_EventExpired@NEntityEvents@@QAE@J@Z (inline)
    bool operator()(CCountedPointer<CEventBase>& event);
    long WorldFrame;
};
}

void CTCEntityEvents::FrameUpdate()
{
    Events.remove_if(NEntityEvents::CPred_EventExpired(GetWorld().GetFrame()));
}
