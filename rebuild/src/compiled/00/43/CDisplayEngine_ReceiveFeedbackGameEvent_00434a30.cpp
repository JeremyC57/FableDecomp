// CDisplayEngine::ReceiveFeedbackGameEvent(const CGameEvent*) -- run the display engine's feedback handler registered for the event's type, if any
// Leaf: ?ReceiveFeedbackGameEvent@CDisplayEngine@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00434A30, 36 bytes. Catalog label CWorld::ProcessEvent is wrong (ICF/neighbour propagation): the
// caller CMainGameComponent (0x004164D6) passes ecx = PDisplayEngine (+0x28), and the handler read is FeedbackFunction
// (+0x20) of the CGameEventDispatch table at 0x013B9288. It sits 0x30 bytes before landed
// CDisplayEngine::ConfirmFeedbackGameEventPackage (0x00434A60), exactly as Ego_r.pdb
// ?ReceiveFeedbackGameEvent@CDisplayEngine@@QAEXPBVCGameEvent@@@Z (0x00446820) sits before ConfirmFeedbackGameEventPackage
// (0x00446850).
// Layout: engine/CDisplayEngine.h + engine/CGameEvent.h (Type +0x00). Ego_r.pdb CGameEventDispatch: four 16-byte
// pointers to member (ActionFunction +0x00, LocalActionFunction +0x10 on CWorld; FeedbackFunction +0x20, ConfirmFunction
// +0x30 on CDisplayEngine).
// Shape: the 16-byte size is the unspecified-inheritance representation of a pointer to member of a class only
// forward-declared at the typedef; at the call the class is complete with no virtual bases, so VC7.1 reads only
// {function, this-adjust} (`mov edx,[eax+20h] ... mov ecx,[eax+24h] ; add ecx,esi`), as 0x0049E1D0.
// NF52 (round 7): the table is the protected static CMainGameComponent::EventDispatch (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
// 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic initializer, landed 0x0121B9B0), read through FableWin.pdb's
// public inline ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z on CMainGameComponent::Get() (overlay
// opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY; no PMainGameComponent load survives, as in retail).
// CGameEventDispatch is engine/CGameEventDispatch.h (included before CDisplayEngine.h). Was a local CGameEventDispatch model and the
// made-up extern GameEventDispatchTable. The CDisplayEngine_Methods self stand-in is pre-existing (NAME_FIXES.tsv).
// NF56 (round 7, queue q10444): Ego_r.pdb 0x00446820 / FableWin.pdb 0x01ADE350 ?ReceiveFeedbackGameEvent@CDisplayEngine@@QAEXPBVCGameEvent@@@Z;
//   defined on CDisplayEngine itself (engine/CDisplayEngine.h overlay opt-in FABLE_RETAIL_CDISPLAYENGINE_RECEIVEFEEDBACKGAMEEVENT), no longer on
//   the stand-in CDisplayEngine_Methods.
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CDISPLAYENGINE_RECEIVEFEEDBACKGAMEEVENT
#include "engine/CDisplayEngine.h"
#include "engine/CGameEvent.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"
void CDisplayEngine::ReceiveFeedbackGameEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = CMainGameComponent::Get()->PeekEventDispatchTableEntry(event->Type);
    if (dispatch->FeedbackFunction)
        (this->*dispatch->FeedbackFunction)(event);
}
