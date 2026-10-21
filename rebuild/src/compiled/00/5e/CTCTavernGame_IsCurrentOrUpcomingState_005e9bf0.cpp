// CTCTavernGame::IsCurrentOrUpcomingState(ETavernGameStates) -- true when state is the current state or the Yes/Alt follow-up state
// Leaf: ?IsCurrentOrUpcomingState@CTCTavernGame@@QAE_NW4ETavernGameStates@1@@Z
// Retail: Fable.exe @ 0x005E9BF0, 41 bytes; Ego_r.pdb same symbol. Compares TavernGameState (+0xc0), YesState (+0xa8)
// and AltState (+0xb0) with the argument; NoState is not consulted.
// Layout: Ego_r.pdb CTCTavernGame YesState +0xc4 / NoState +0xc8 / AltState +0xcc / TavernGameState +0xdc; retail sits
// 0x1c lower (STLport containers and the three CCountedPointer font banks before them). No engine/ header.
// Shape: one `||` return expression (false block first, then `mov eax,1`), as 0x004496B0.
class CTCTavernGame {
public:
    enum ETavernGameStates { TAVERN_GAME_STATE_NONE };
    bool IsCurrentOrUpcomingState(ETavernGameStates state);
    unsigned char     _pad00[0xa8];
    ETavernGameStates YesState;                          // +0xa8
    ETavernGameStates NoState;                           // +0xac
    ETavernGameStates AltState;                          // +0xb0
    void (*YesFunc)();                                   // +0xb4
    void (*NoFunc)();                                    // +0xb8
    void (*AltFunc)();                                   // +0xbc
    ETavernGameStates TavernGameState;                   // +0xc0
};
bool CTCTavernGame::IsCurrentOrUpcomingState(ETavernGameStates state)
{
    return TavernGameState == state || YesState == state || AltState == state;
}