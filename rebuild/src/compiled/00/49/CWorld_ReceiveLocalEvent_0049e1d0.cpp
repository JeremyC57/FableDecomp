// CWorld::ReceiveLocalEvent(const CGameEvent*) -- run the world's local-action handler registered for the event's type, if any
// Leaf: ?ReceiveLocalEvent@CWorld_Methods@@QAEXPBUCGameEvent@@@Z
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
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
struct CWorld_Methods;
struct CDisplayEngine;
typedef void (CWorld_Methods::*TWorldEventFunction)(const CGameEvent* event);
struct CGameEventDispatch {
    TWorldEventFunction ActionFunction;                  // +0x00
    TWorldEventFunction LocalActionFunction;             // +0x10
    unsigned char       FeedbackFunction[0x10];          // +0x20 CDisplayEngine pointer to member
    unsigned char       ConfirmFunction[0x10];           // +0x30 CDisplayEngine pointer to member
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
struct CWorld_Methods : CWorld {
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const { return &GameEventDispatchTable[type]; }
    void ReceiveLocalEvent(const CGameEvent* event);
};
void CWorld_Methods::ReceiveLocalEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->LocalActionFunction)
        (this->*dispatch->LocalActionFunction)(event);
}