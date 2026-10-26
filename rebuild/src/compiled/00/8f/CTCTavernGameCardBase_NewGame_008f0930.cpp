// CTCTavernGameCardBase::NewGame() -- base NewGame (pay the bet, clear the round flags), then reset the card selection state and the counter and go to state 7
// Leaf: ?NewGame@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0930, length 48 bytes. Virtual slot 31 (NewGame; 0x008F0930 sits in
// CTCTavernGameCardPairs' vtable 0x0126F6FC slot 31, inherited from CTCTavernGameCardBase). Ego_r.pdb names
// `?NewGame@CTCTavernGameCardBase@@UAEXXZ`.
// Callees: CTCTavernGame::NewGame() @ 0x005E9A70 (called non-virtually; landed), CTCTavernGame::SetState(ETavernGameStates)
// @ 0x005E9B20.
// Layout (retail offsets; Ego_r.pdb minus the shrunken vectors): CTCTavernGame::GameFinished +0x28 (PDB +0x30; the
// retail UIComponents vector is 8 bytes shorter); CTCTavernGameCardBase (PDB base 0x108 -> retail 0xf0, its own vectors
// at PDB +0x140/+0x150/+0x168 are 12 bytes): Counter +0x154, RoundFinished +0x158, CardOrder +0x304, SelectedCard +0x308,
// HighlightedCard +0x30c, CurrentMovingCard +0x310. No engine header exists for the card classes; local model.
// Shape: the stores follow the source order and share the eax zero (the float Counter included); `push 7` is hoisted
// above them.
typedef long ETavernGameStates;
class CCard;

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0x24];
    bool          GameFinished;              // +0x28
    unsigned char _pad_0x29[0xc7];
    virtual void NewGame();                  // 0x005E9A70
    void SetState(ETavernGameStates state);  // 0x005E9B20
};

struct CTCTavernGameCardBase : CTCTavernGame {
    unsigned char _pad_0xf0[0x64];
    float         Counter;                   // +0x154
    bool          RoundFinished;             // +0x158
    unsigned char _pad_0x159[0x1ab];
    long          CardOrder;                 // +0x304
    CCard*        SelectedCard;              // +0x308
    CCard*        HighlightedCard;           // +0x30c
    long          CurrentMovingCard;         // +0x310
    virtual void NewGame();
};

void CTCTavernGameCardBase::NewGame()
{
    CTCTavernGame::NewGame();
    CardOrder = 0;
    GameFinished = false;
    SelectedCard = 0;
    HighlightedCard = 0;
    Counter = 0.0f;
    SetState(7);
}