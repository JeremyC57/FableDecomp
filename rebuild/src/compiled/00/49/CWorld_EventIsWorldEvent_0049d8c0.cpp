// CWorld::EventIsWorldEvent(const CGameEvent* event) const -- true when the event's type has a world action handler in the game-event dispatch table
// Leaf: ?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D8C0, 23 bytes (int3 padding 0x0049D8D7..DF), thiscall `ret 4`; `this` is never read.
// Identity (proven, two sources): (1) Ego_r.pdb / FableWin.pdb ?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z, Ego_r donor 0x004A6C20
//   immediately followed by ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z 0x004A6C40 -- retail keeps that order
//   (this row, then the landed out-of-line GetEventDispatchTableEntry 0x0049D8E0); (2) the only caller CMainGameComponent::AddGameEvent
//   0x0041649C (call at 0x004164A8) passes `ecx = PWorld` (+0x24) and the event pointer and tests the bool before SendGameEvent.
//   The landed caller binds it as ?EventIsWorldEvent@CWorld_Methods@@... (a stand-in class; NAME_FIXES.tsv).
// Body: the 64-byte CGameEventDispatch entry of the event's Type (`shl eax,6 ; add eax,table`) through the protected
//   GetEventDispatchTableEntry (defined right below, as in retail, and inlined), then the null test of its 16-byte ActionFunction.
// Shape: the dispatch entry is a NAMED local (`const CGameEventDispatch* dispatch = ...`): VC7.1 then tests only the function word
//   (`cmp dword ptr [eax],0 ; setne al`). The single-expression `GetEventDispatchTableEntry(..)->ActionFunction != NULL` copies the
//   whole 16-byte member pointer into a frame temporary first (52v23).
// Layout: engine/CWorld.h retail opt-in (FABLE_RETAIL_CWORLD, hdr/); engine/CGameEvent.h (Type +0x00). CGameEventDispatch is Ego_r's
//   (four 16-byte member pointers; the action pointers are declared while CWorld is still incomplete -- unspecified inheritance,
//   16 bytes -- which gives the 64-byte stride), as the landed 0x0049D8E0 / 0x0049D980.
// Data: CMainGameComponent::EventDispatch 0x013B9288 (Anniversary PDB LF_STMEMBER; see the NF52 note below).
#include "engine/CGameEvent.h"
// NF52 (round 7): the table is the protected static CMainGameComponent::EventDispatch (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
// 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic initializer, landed 0x0121B9B0), read through FableWin.pdb's
// public inline ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z on CMainGameComponent::Get() (overlay
// opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY); CGameEventDispatch is engine/CGameEventDispatch.h (included
// before CWorld.h, so the member pointers keep the 16-byte unspecified-inheritance form). Was a local CGameEventDispatch model and the
// made-up extern GameEventDispatchTable.
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"

bool CWorld::EventIsWorldEvent(const CGameEvent* event) const
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    return dispatch->ActionFunction != NULL;
}

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return CMainGameComponent::Get()->PeekEventDispatchTableEntry(type);
}