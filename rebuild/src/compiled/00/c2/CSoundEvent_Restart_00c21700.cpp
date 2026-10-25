// CSoundEvent::Restart(CSoundEventManager* owner) -- an event in state 3 or 6 is first taken back to idle through that state's exit transition, then the event enters state 4 from idle
// Leaf: ?Restart@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CPanelItem_Restart_00c21700.cpp).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21700, 74 bytes (`ret 4`). Descriptive names (no PDB covers this middleware
//   library): the manifest's "Inventory_DispatchEvent" is propagated. Caller 0x00C122C3 (the manager code of chunk 431).
// Callees: CSoundEvent::ChangeState @ 0x00C20720 (-1 = any state), the exit transitions of state 3 @ 0x00C209C0 and of
//   state 6 @ 0x00C20B50, the idle -> 4 transition @ 0x00C20880 (it checks state 1 and stores 4).
// Shape: the state word is compared as a 16-bit value (`mov ax ; cmp ax,3`, an if/else chain, not a switch); the
//   argument load is scheduled between the compare and its `jne`; the state-3 arm calls its exit transition with
//   ecx still holding `this` from before the ChangeState call (VC7.1 knows the same-TU stand-in body preserves ecx)
//   and gets its own copy of the closing call and epilogue.

struct CSoundEventManager;

struct CSoundEvent {                             // 0xB4-byte state-machine event
    unsigned char  _pad00[0xac];
    unsigned short m_state;                     // +0xAC
    void  ChangeState(long state);              // 0x00C20720
    void* EnterState4(CSoundEventManager* owner);      // 0x00C20880
    void* LeaveState3(CSoundEventManager* owner);      // 0x00C209C0
    void* LeaveState6(CSoundEventManager* owner);      // 0x00C20B50
    void  Restart(CSoundEventManager* owner);
};

#pragma auto_inline(off)
// Same-TU stand-in for the retail state helper 0x00C20720 (retail: a byte-indexed jump table over the state word
// whose every case is empty; it touches only eax): compiled above the caller so VC7.1 knows it preserves ecx.
extern long g_soundEventRequestedState;          // stand-in only (not a retail global)
__declspec(noinline) void CSoundEvent::ChangeState(long state)
{
    g_soundEventRequestedState = state;
}
#pragma auto_inline(on)

void CSoundEvent::Restart(CSoundEventManager* owner)
{
    ChangeState(-1);
    if (m_state == 3)
        LeaveState3(owner);
    else if (m_state == 6)
        LeaveState6(owner);
    EnterState4(owner);
}