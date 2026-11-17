// CDisplayEngine::ConfirmFeedbackGameEventPackage(const CGameEventPackage*) -- dispatch every event of the package to the handler registered for its type
// Leaf: ?ConfirmFeedbackGameEventPackage@CDisplayEngine@@QAEXPBVCGameEventPackage@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434A60, length 74 bytes.
// Callees: CGameEventPackage::GetNoEvents() @ 0x009F16E0 (called for every iteration), CGameEventPackage::GetEvent(unsigned long)
// @ 0x009F16D0; the handler is a multiple-inheritance pointer to member {function, this-adjust} at +0x30 of the 64-byte
// entries of the handler table @ 0x013B9288, indexed by the event's type (`shl ecx,6`), called through `add ecx,ebx ; call edx`
// when the function pointer is set. The manifest labels of the callees (CWorld::DrawGetEnvironment / GetBlankTexture) are
// propagated across identical bodies. Shape: an unsigned counted loop (`jbe` / `jb`) with the count re-read each iteration.
// NF52 (round 7): the table is the protected static CMainGameComponent::EventDispatch (?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B,
// 0x013B9288; Anniversary PDB LF_STMEMBER, the Ego_r / FableWin dynamic initializer, landed 0x0121B9B0), read through FableWin.pdb's
// public inline ?PeekEventDispatchTableEntry@CMainGameComponent@@QBEPBVCGameEventDispatch@@J@Z on CMainGameComponent::Get() (overlay
// opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY). The row is on the shared headers: CDisplayEngine.h
// (FABLE_RETAIL_CDISPLAYENGINE_EVENTPACKAGES declares the PDB signature, Ego_r / FableWin
// ?ConfirmFeedbackGameEventPackage@CDisplayEngine@@QAEXPBVCGameEventPackage@@@Z, the spelling the caller 0x0041726D already binds),
// CGameEventPackage.h (overlay opt-in FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS: the PDB const GetNoEvents / GetEvent), CGameEvent.h
// and CGameEventDispatch.h (16-byte member pointers; the call reads {function, this-adjust} at +0x30 / +0x34). Were local models of
// CGameEvent / CGameEventPackage / CDisplayEngine and the made-up SGameEventHandler g_GameEventHandlers.
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CGAMEEVENTPACKAGE
#define FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS
#include "engine/CGameEventPackage.h"
#define FABLE_RETAIL_CDISPLAYENGINE_EVENTPACKAGES
#include "engine/CDisplayEngine.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY
#include "engine/CMainGameComponent.h"

void CDisplayEngine::ConfirmFeedbackGameEventPackage(const CGameEventPackage* package)
{
    for (unsigned long i = 0; i < package->GetNoEvents(); ++i) {
        const CGameEvent* event = package->GetEvent(i);
        const CGameEventDispatch* dispatch = CMainGameComponent::Get()->PeekEventDispatchTableEntry(event->Type);
        if (dispatch->ConfirmFunction)
            (this->*dispatch->ConfirmFunction)(event);
    }
}
