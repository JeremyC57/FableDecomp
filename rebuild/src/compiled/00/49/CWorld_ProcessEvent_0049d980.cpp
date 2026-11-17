// CWorld::ProcessEvent(const CGameEvent* event) -- run the world's action handler registered for the event's type in the game-event dispatch table, if any
// Leaf: ?ProcessEvent@CWorld@@IAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D980, 35 bytes (+13 int3 padding), thiscall, `ret 4`.
//   Ego_r.pdb ?ProcessEvent@CWorld@@IAEXPBVCGameEvent@@@Z (0x004A6CD0, between PostLoadInit and GetSampleBank -- retail
//   0x0049D970 / 0x0049D9B0, both landed). The landed 0x0049E1D0 (also named ProcessEvent) is the PDB's
//   ReceiveLocalEvent (0x004A7380, between PeekThingManager and GetChecksum): it calls LocalActionFunction (+0x10),
//   this row calls ActionFunction (+0x00).
// Data: the 64-byte CGameEventDispatch table 0x013B9288, indexed by the event's Type (`shl eax,6`) through
//   CWorld::GetEventDispatchTableEntry(long) const (Ego_r.pdb 0x004A6C40, inlined).
// Layout: engine/CWorld.h + engine/CGameEvent.h (Type +0x00); CGameEventDispatch as the landed 0x0049E1D0.
// Shape: the 16-byte unspecified-inheritance pointer to member (class only forward-declared at the typedef); at the
//   call the class is complete with no virtual bases, so only {function, this-adjust} is read (`add ecx,esi ; call edx`).
// NF52 (round 7): the table is the protected static CMainGameComponent::EventDispatch (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
// 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic initializer, landed 0x0121B9B0), read through CWorld's protected
// GetEventDispatchTableEntry (Ego_r / FableWin ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z, out of line at 0x0049D8E0
// and inlined here, defined in this TU as in retail's) over FableWin.pdb's public inline
// ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z (overlay opt-in
// FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY); CGameEventDispatch is engine/CGameEventDispatch.h (included before
// CWorld.h). Was a local CGameEventDispatch model and the made-up extern GameEventDispatchTable. The self function was the stand-in
// CWorld_Methods::ProcessEvent; it is now the PDB's protected CWorld::ProcessEvent (Ego_r / FableWin ?ProcessEvent@CWorld@@IAEXPBVCGameEvent@@@Z,
// overlay opt-in FABLE_RETAIL_CWORLD_PROCESSEVENT); retail has no direct caller (Update 0x0049DFB0 expands it).
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PROCESSEVENT
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"

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
