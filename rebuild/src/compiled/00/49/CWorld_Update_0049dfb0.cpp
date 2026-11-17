// CWorld::Update(const CGameEventPackage& events, bool processType1Events) -- times the world update: publishes the thing manager, dispatches every event except type 1 through its action handler, then (when asked) the type-1 events, and records the update duration
// Leaf: ?Update@CWorld@@QAEXABVCGameEventPackage@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DFB0, 198 bytes (padded to 208), `ret 8`.
//   PDB name ?Update@CWorld@@QAEXABVCGameEventPackage@@_N@Z.
// Callees: GFGetTime 0x009E1BC0 (`__fastcall`, double), CWorld virtual slot 3 GetThingManager 0x0049E1B0 (returns a
//   reference), SetCurrentThingManager 0x004C74F0 (`__fastcall`), CGameEventPackage::GetNoEvents 0x009F16E0 / GetEvent
//   0x009F16D0, the event's ActionFunction pointer to member from the 64-byte dispatch table 0x013B9288.
// Shape: the inline ProcessEvent (`GetEventDispatchTableEntry(type)->ActionFunction`, an 8-byte {function, this-adjust}
//   pointer to member called when set) is expanded in both loops; in the second loop the type is known to be 1, so the
//   entry address folds to 0x013B92C8; the start time is a double local kept in the frame (`fst`), LastUpdateTime stored
//   from the same value.
// Layout (engine/CWorld.h, offsets below +0xac match retail): two polymorphic bases (vtables 0x012390F0 / 0x012390DC),
//   LastUpdateTimeLength +0x90, LastUpdateTime +0x98.
// NF52 (round 7): moved onto the shared headers. The table is the protected static CMainGameComponent::EventDispatch
// (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B, 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic
// initializer, landed 0x0121B9B0) read through CWorld's protected GetEventDispatchTableEntry (0x0049D8E0) over FableWin.pdb's public inline
// ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z (overlay opt-in
// FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY); the inline ProcessEvent is the PDB's protected CWorld::ProcessEvent
// (0x0049D980, overlay opt-in FABLE_RETAIL_CWORLD_PROCESSEVENT); both are defined in this TU, in retail order, and expanded here. The
// thing-manager setter is Ego_r's ?SetCurrentThingManager@CThing@@SIXAAVCThingManager@@@Z (engine/CThing.h opt-in) and GFGetTime is
// engine/GFGetTime.h. CGameEventPackage's const GetNoEvents / GetEvent are the PDB spellings (overlay opt-in
// FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS). Were local CWorld / CGameEvent / CGameEventPackage models, the made-up
// SGameEventHandler g_GameEventHandlers, and free stand-ins for SetCurrentThingManager.
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTPACKAGES
#define FABLE_RETAIL_CWORLD_PROCESSEVENT
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
#define FABLE_RETAIL_CGAMEEVENTPACKAGE
#define FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS
#include "engine/CGameEventPackage.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"
#include "engine/GFGetTime.h"
#define FABLE_RETAIL_CTHING_SETCURRENTTHINGMANAGER
#include "engine/CThing.h"

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return CMainGameComponent::Get()->PeekEventDispatchTableEntry(type);
}

void CWorld::ProcessEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->ActionFunction)
        (this->*dispatch->ActionFunction)(event);
}

void CWorld::Update(const CGameEventPackage& events, bool processType1Events)
{
    double start = GFGetTime();
    LastUpdateTime = start;
    CThing::SetCurrentThingManager(GetThingManager());
    for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
        const CGameEvent* event = events.GetEvent(i);
        if (event->Type != 1)
            ProcessEvent(event);
    }
    if (processType1Events) {
        for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
            const CGameEvent* event = events.GetEvent(i);
            if (event->Type == 1)
                ProcessEvent(event);
        }
    }
    LastUpdateTimeLength = GFGetTime() - start;
}
