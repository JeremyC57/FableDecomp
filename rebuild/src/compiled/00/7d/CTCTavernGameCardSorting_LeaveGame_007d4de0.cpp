// CTCTavernGameCardSorting::LeaveGame() -- card-base LeaveGame, then kills the game's spawned card thing and clears the pointer
// Retail: Fable.exe @ 0x007D4DE0, 34 bytes (CTCTavernGameCardSorting vtable 0x0126F5B4 slot 30, the LeaveGame slot;
//         byte-identical to the CTCTavernGameCardPairs override at 0x007D5F60).
// Callees: CTCTavernGameCardBase::LeaveGame 0x008F20F0, CTCTavernGame::KillThing 0x005E9A60
//          (a member that never reads `this`: `thing->Kill(1)`, ret 4).
// Shape: plain base call through the qualified name (non-virtual `call`), member read pushed, then
//        the zero store after the call. The retail card games are far larger than the PDB's
//        CTCTavernGame (0x108), so the base is a local model padded up to the derived member at +0x140.
class CThing;

struct CTCTavernGameCardBase {
    unsigned char _pad[0x140];
    void LeaveGame();                   // 0x008F20F0
    void KillThing(CThing* thing);      // 0x005E9A60
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    CThing* m_pCardThing;               // +0x140
    void LeaveGame();
};

void CTCTavernGameCardSorting::LeaveGame()
{
    CTCTavernGameCardBase::LeaveGame();
    KillThing(m_pCardThing);
    m_pCardThing = 0;
}