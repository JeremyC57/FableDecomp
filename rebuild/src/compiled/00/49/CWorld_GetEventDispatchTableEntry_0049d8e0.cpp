// CWorld::GetEventDispatchTableEntry(long type) const -- the 64-byte game-event dispatch table entry for an event type
// Leaf: ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z
// Retail: Fable.exe @ 0x0049D8E0, 15 bytes, __fastcall `ret 4`: `mov eax,[esp+4] ; shl eax,6 ; add eax,0x013B9288`.
// Identity (proven): Ego_r.pdb ?GetEventDispatchTableEntry@CWorld@@IBEPBVCGameEventDispatch@@J@Z @ 0x004A6C40, just ahead of
// ProcessEvent (Ego 0x004A6CD0 / retail 0x0049D980, landed, which inlines this same lookup); retail keeps the same order
// (0x0049D8E0 before SetAsPaused 0x0049D8F0 .. ProcessEvent 0x0049D980). The out-of-line copy has no caller (gap-scan).
// Data: GameEventDispatchTable @ 0x013B9288 (64-byte CGameEventDispatch, Ego_r.pdb layout; as the landed 0x0049E1D0).
#define CWorld DonorCWorld
#include "engine/CWorld.h"
#undef CWorld
class CGameEvent;
class CWorld;
typedef void (CWorld::*TWorldEventFunction)(const CGameEvent* event);
class CGameEventDispatch { public:                       // Ego_r.pdb, 64 bytes
    TWorldEventFunction ActionFunction;                  // +0x00
    TWorldEventFunction LocalActionFunction;             // +0x10
    unsigned char       FeedbackFunction[0x10];          // +0x20 CDisplayEngine pointer to member
    unsigned char       ConfirmFunction[0x10];           // +0x30 CDisplayEngine pointer to member
};
extern CGameEventDispatch GameEventDispatchTable[];      // 0x013B9288
class CWorld : public DonorCWorld {                      // engine/CWorld.h
protected:
    const CGameEventDispatch* GetEventDispatchTableEntry(long type) const;
};

const CGameEventDispatch* CWorld::GetEventDispatchTableEntry(long type) const
{
    return &GameEventDispatchTable[type];
}