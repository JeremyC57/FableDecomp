// CTCTavernGameCardBlackjack::NewGame() -- card-base NewGame, then clear this round's winnings, the hands won and the draw flag
// Leaf: ?NewGame@CTCTavernGameCardBlackjack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA050, length 30 bytes. CTCTavernGameCardBlackjack vtable slot 31;
// Ego_r.pdb `?NewGame@CTCTavernGameCardBlackjack@@UAEXXZ`.
// Callee: CTCTavernGameCardBase::NewGame() @ 0x008F0930 (called non-virtually).
// Layout (retail offsets, see EnterGame 0x007DA020): AmountWonThisRound +0x36c, NumHandsWonThisRound +0x370;
// CTCTavernGame::Draw +0xa1 (PDB +0xbd, 0x1c lower in retail).
// Shape: source-order stores from one eax zero.
struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0x9d];
    bool          Draw;                      // +0xa1 (CTCTavernGame)
    unsigned char _pad_0xa2[0x276];
    virtual void NewGame();                  // 0x008F0930
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    unsigned char _pad_0x318[0x54];
    long          AmountWonThisRound;        // +0x36c
    long          NumHandsWonThisRound;      // +0x370
    virtual void NewGame();
};

void CTCTavernGameCardBlackjack::NewGame()
{
    CTCTavernGameCardBase::NewGame();
    AmountWonThisRound = 0;
    NumHandsWonThisRound = 0;
    Draw = false;
}