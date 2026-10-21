// CDisplayEngine::ReceiveFeedbackGameEvent(const CGameEvent*) -- run the display engine's feedback handler registered for the event's type, if any
// Leaf: ?ReceiveFeedbackGameEvent@CDisplayEngine_Methods@@QAEXPBUCGameEvent@@@Z
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
#include "engine/CDisplayEngine.h"
#include "engine/CGameEvent.h"
struct CDisplayEngine_Methods;
typedef void (CDisplayEngine_Methods::*TDisplayEventFunction)(const CGameEvent* event);
struct CGameEventDispatch {
    unsigned char         ActionFunction[0x10];          // +0x00 CWorld pointer to member
    unsigned char         LocalActionFunction[0x10];     // +0x10 CWorld pointer to member
    TDisplayEventFunction FeedbackFunction;              // +0x20
    TDisplayEventFunction ConfirmFunction;               // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
struct CDisplayEngine_Methods : CDisplayEngine {
    void ReceiveFeedbackGameEvent(const CGameEvent* event);
};
void CDisplayEngine_Methods::ReceiveFeedbackGameEvent(const CGameEvent* event)
{
    const CGameEventDispatch& dispatch = GameEventDispatchTable[event->Type];
    if (dispatch.FeedbackFunction)
        (this->*dispatch.FeedbackFunction)(event);
}