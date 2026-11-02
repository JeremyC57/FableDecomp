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
// Data: GameEventDispatchTable 0x013B9288 (no PDB data symbol; the landed 0x0049D8E0 spelling).
#include "engine/CGameEvent.h"
class CWorld;
typedef void (CWorld::*TWorldEventFunction)(const CGameEvent* event);
class CGameEventDispatch { public:                       // Ego_r.pdb, 64 bytes
    TWorldEventFunction ActionFunction;                  // +0x00
    TWorldEventFunction LocalActionFunction;             // +0x10
    unsigned char       FeedbackFunction[0x10];          // +0x20 CDisplayEngine pointer to member
    unsigned char       ConfirmFunction[0x10];           // +0x30 CDisplayEngine pointer to member
};
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch) == 0x40);
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"

bool CWorld::EventIsWorldEvent(const CGameEvent* event) const
{
    const CGameEventDispatch* dispatch = GetEventDispatchTableEntry(event->Type);
    return dispatch->ActionFunction != NULL;
}

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return &GameEventDispatchTable[type];
}