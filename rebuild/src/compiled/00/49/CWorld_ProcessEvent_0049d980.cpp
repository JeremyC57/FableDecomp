// CWorld::ProcessEvent(const CGameEvent* event) -- run the world's action handler registered for the event's type in the game-event dispatch table, if any
// Leaf: ?ProcessEvent@CWorld_Methods@@IAEXPBVCGameEvent@@@Z
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
#include "engine/CWorld.h"
#include "engine/CGameEvent.h"
struct CWorld_Methods;
typedef void (CWorld_Methods::*TWorldEventFunction)(const CGameEvent* event);
class CGameEventDispatch { public:
    TWorldEventFunction ActionFunction;                  // +0x00
    TWorldEventFunction LocalActionFunction;             // +0x10
    unsigned char       FeedbackFunction[0x10];          // +0x20 CDisplayEngine pointer to member
    unsigned char       ConfirmFunction[0x10];           // +0x30 CDisplayEngine pointer to member
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
struct CWorld_Methods : CWorld {
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const { return &GameEventDispatchTable[type]; }
protected:
    void ProcessEvent(const CGameEvent* event);
};
void CWorld_Methods::ProcessEvent(const CGameEvent* event)
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    if (dispatch->ActionFunction)
        (this->*dispatch->ActionFunction)(event);
}