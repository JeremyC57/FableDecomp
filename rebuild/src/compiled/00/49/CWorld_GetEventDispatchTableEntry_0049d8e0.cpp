// CWorld::GetEventDispatchTableEntry(long type) const -- the 64-byte game-event dispatch table entry for an event type
// Leaf: ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z
// Retail: Fable.exe @ 0x0049D8E0, 15 bytes, __fastcall `ret 4`: `mov eax,[esp+4] ; shl eax,6 ; add eax,0x013B9288`.
// Identity (proven): Ego_r.pdb ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z @ 0x004A6C40, just ahead of
// ProcessEvent (Ego 0x004A6CD0 / retail 0x0049D980, landed, which inlines this same lookup); retail keeps the same order
// (0x0049D8E0 before SetAsPaused 0x0049D8F0 .. ProcessEvent 0x0049D980). The out-of-line copy has no caller (gap-scan).
// Data: CMainGameComponent::EventDispatch @ 0x013B9288 (64-byte CGameEventDispatch, Ego_r.pdb layout).
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

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return CMainGameComponent::Get()->PeekEventDispatchTableEntry(type);
}
