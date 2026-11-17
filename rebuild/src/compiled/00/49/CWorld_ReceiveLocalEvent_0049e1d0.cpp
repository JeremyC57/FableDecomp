// CWorld::ReceiveLocalEvent(const CGameEvent*) -- run the world's local-action handler registered for the event's type, if any
// Leaf: ?ReceiveLocalEvent@CWorld_Methods@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x0049E1D0, 36 bytes; Ego_r.pdb ?ReceiveLocalEvent@CWorld@@QAEXPBVCGameEvent@@@Z (public). Identity
//   (sweep1, 2026-10-25; FINDINGS chunk 467): the landed row was CWorld::ProcessEvent, but the PDB's protected ProcessEvent
//   (?ProcessEvent@CWorld@@IAEXPBVCGameEvent@@@Z) is chunk 467's 0x0049D980, which calls ActionFunction (+0x00); this body
//   calls LocalActionFunction (+0x10), and its retail neighbours GetThingManager 0x0049E1B0 / PeekThingManager / GetChecksum
//   0x0049E200 / LoadWorldFromFile 0x0049E220 follow the donor PDB order PeekThingManager, ReceiveLocalEvent, GetChecksum,
//   LoadWorldFromFile (Ego_r 0x004A7370..0x004A73C0). Called from
// CMainGameComponent (0x004164CD) with ecx = PWorld (+0x24). Indexes the 64-byte CGameEventDispatch table at 0x013B9288 by
// the event's Type (`shl eax,6`) -- Ego_r.pdb CWorld::GetEventDispatchTableEntry(long) const -- and, when
// LocalActionFunction (+0x10) is set, calls it as a {function, this-adjust} pointer to member (`add ecx,esi ; call edx`).
// Layout: engine/CWorld.h + engine/CGameEvent.h (Type +0x00). Ego_r.pdb CGameEventDispatch holds four 16-byte
// pointers to member (ActionFunction +0x00, LocalActionFunction +0x10, FeedbackFunction +0x20, ConfirmFunction +0x30).
// Shape: the 16-byte size is the unspecified-inheritance representation VC gives a pointer to member of a class that is
// only forward-declared when the typedef is seen; at the call the class is complete with no virtual bases, so VC7.1 reads
// only {function, this-adjust} -- exactly the retail `mov edx,[eax+10h] ... mov ecx,[eax+14h] ; add ecx,esi`.
// NF52 (round 7): the table is the protected static CMainGameComponent::EventDispatch (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
// 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic initializer, landed 0x0121B9B0), read through CWorld's protected
// GetEventDispatchTableEntry (Ego_r / FableWin ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z, out of line at 0x0049D8E0
// and inlined here, defined in this TU as in retail's) over FableWin.pdb's public inline
// ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z (overlay opt-in
// FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY); CGameEventDispatch is engine/CGameEventDispatch.h (included before
// CWorld.h). Was a local CGameEventDispatch model and the made-up extern GameEventDispatchTable. The CWorld_Methods self stand-in is
// pre-existing (NAME_FIXES.tsv).
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return CMainGameComponent::Get()->PeekEventDispatchTableEntry(type);
}

struct CWorld_Methods : CWorld {
    void ReceiveLocalEvent(const CGameEvent* event);
};
void CWorld_Methods::ReceiveLocalEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->LocalActionFunction)
        (this->*dispatch->LocalActionFunction)(event);
}
