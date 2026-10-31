// CGameEvent::CGameEvent(long type) -- a game event of the type: no player (-1), empty data, valid, not a replacement
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004159B0 (`ret 4`, returns `this`).
// Leaf: ??0CGameEvent@@QAE@J@Z
// Name: Ego_r.pdb + FableWin.pdb ??0CGameEvent@@QAE@J@Z. Shape: the plain initialiser-list ctor (chunk 626 research: it matches the
//   retail out-of-line copy, the two zero bytes from one `xor cl,cl`); landed before naming wave 12 as a fake __fastcall function.
// Layout: engine/CGameEvent.h (Type +0x00, Player +0x04, Data[32] +0x05, EndPos +0x25, Valid +0x26, Replacement +0x27); the
//   header class has no ctor, so it is included under a rename and the class re-declared with the same members plus the ctor.
#define CGameEvent CGameEventLayout
#include "engine/CGameEvent.h"
#undef CGameEvent
class CGameEvent {
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type);
};
FABLE_STATIC_ASSERT(sizeof(CGameEvent) == sizeof(CGameEventLayout));
FABLE_STATIC_ASSERT(offsetof(CGameEvent, Replacement) == offsetof(CGameEventLayout, Replacement));

CGameEvent::CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false)
{
}
