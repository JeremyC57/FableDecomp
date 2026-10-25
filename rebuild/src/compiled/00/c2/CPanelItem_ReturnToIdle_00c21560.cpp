// CPanelItem::ReturnToIdle(CPanel* owner) -- leave whatever active state the item is in (2..6) through that state's own exit transition, then check that the item ended in the idle state 1
// Leaf: ReturnToIdle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21560, 180 bytes including its table (`ret 4`, five-entry embedded jump table at 0x00C21600).
//   Descriptive names (no PDB covers this middleware library; chunk 431 names the 0xB4-byte state-machine item
//   CPanelItem, the landed 0x00C21620 calls it CSteeringAgent_c21620 and this function "Attach"): the manifest's
//   "NUISystem_CScrollableList_OnStateChange" is propagated. Callers 0x00C21644 (OnCreate 0x00C21620), 0x00C21B4B,
//   0x00C21B76.
// Callees: CPanelItem::ChangeState @ 0x00C20720 (-1 first = any state, 1 at the end), the per-state exit
//   transitions (each checks its own state, moves the item to state 1): state 2 @ 0x00C20920, state 6 @ 0x00C20B50,
//   state 5 @ 0x00C20BB0, state 3 @ 0x00C209C0, state 4 @ 0x00C208E0 (landed as "EnterStateAndAttach").
// Shape: the closing `ChangeState(1)` is tail-duplicated into every case (each with its own `pop esi ; ret 4`), the
//   last case falls into the shared copy that the `ja` default also reaches. Case bodies in source order: retail's
//   table (2 -> 0xC21580, 3 -> 0xC215CB, 4 -> 0xC215E4, 5 -> 0xC215B2, 6 -> 0xC21599) puts 2, 6, 5, 3, 4 in that
//   order; each case/callee pairing verified against the table by hand. The stand-in ChangeState body lets VC7.1
//   keep `this` in esi only (no reload after the first call is needed).

struct CPanel;

struct CPanelItem {                             // 0xB4-byte state-machine item
    unsigned char  _pad00[0xac];
    unsigned short m_state;                     // +0xAC
    void  ChangeState(long state);              // 0x00C20720
    void* LeaveState2(CPanel* owner);      // 0x00C20920
    void* LeaveState3(CPanel* owner);      // 0x00C209C0
    void* LeaveState4(CPanel* owner);      // 0x00C208E0
    void* LeaveState5(CPanel* owner);      // 0x00C20BB0
    void* LeaveState6(CPanel* owner);      // 0x00C20B50
    void  ReturnToIdle(CPanel* owner);
};

#pragma auto_inline(off)
// Same-TU stand-in for the retail state helper 0x00C20720 (retail: a byte-indexed jump table over the state word
// whose every case is empty; it touches only eax): compiled above the caller so VC7.1 knows it preserves ecx.
extern long g_panelItemRequestedState;          // stand-in only (not a retail global)
__declspec(noinline) void CPanelItem::ChangeState(long state)
{
    g_panelItemRequestedState = state;
}
#pragma auto_inline(on)

void CPanelItem::ReturnToIdle(CPanel* owner)
{
    ChangeState(-1);
    switch (m_state) {
    case 2: LeaveState2(owner); break;
    case 6: LeaveState6(owner); break;
    case 5: LeaveState5(owner); break;
    case 3: LeaveState3(owner); break;
    case 4: LeaveState4(owner); break;
    }
    ChangeState(1);
}