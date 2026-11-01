# sub-wave 2e (hand): 0x004159B0 ??0CGameEvent@@QAE@J@Z (Ego_r + FableWin CGameEvent(long)) as a plain initialiser-list ctor
# (agent 626: the init-list ctor matches the retail out-of-line copy, shared `xor cl,cl`), replacing the fake __fastcall model. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import put
put('004159b0', 'src', r'''// CGameEvent::CGameEvent(long type) -- a game event of the type: no player (-1), empty data, valid, not a replacement
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
''')
put('004159b0', 'tst', r'''#include <stdio.h>
#include <string.h>
#include <new>

/* model under test: the CGameEvent(long) initialiser-list ctor of the source */
class CGameEvent {
public:
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};

int main()
{
    unsigned char buf[sizeof(CGameEvent)];
    memset(buf, 0xAA, sizeof(buf));
    CGameEvent* ev = new (buf) CGameEvent(0x12345678);
    int ok = 1;
    if ((void*)ev != (void*)buf) ok = 0;
    if (sizeof(CGameEvent) != 0x28) ok = 0;
    if (ev->Type != 0x12345678) ok = 0;
    if (ev->Player != -1) ok = 0;
    if (ev->EndPos != 0) ok = 0;
    if (ev->Valid != true) ok = 0;
    if (ev->Replacement != false) ok = 0;
    if (ev->Data[0] != 0xAA) ok = 0;   /* data left untouched */
    if (ok) printf("OK_0x004159b0\n");
    else printf("FAIL_0x004159b0\n");
    return ok ? 0 : 1;
}
''')
