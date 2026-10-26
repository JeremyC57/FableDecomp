// CTCTavernGameCardBase::NewRound() -- base NewRound (game in session), then clear the round-finished flag and the table bounds and go to state 18
// Leaf: ?NewRound@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F0960, length 51 bytes. CTCTavernGameCardBase vtable slot 32 (NewRound;
// CTCTavernGame's slot 32 is 0x005E9A90, `mov byte ptr [ecx+29h],1`). Ego_r.pdb names `?NewRound@CTCTavernGameCardBase@@UAEXXZ`.
// The catalog label "CTCTavernGameCardBase::NewGame" is the neighbouring slot's name and is corrected here.
// Callees: CTCTavernGame::NewRound() @ 0x005E9A90 (called non-virtually; manifest "CXMVPlaybackInfo" is propagated),
// CTCTavernGame::SetState(ETavernGameStates) @ 0x005E9B20.
// Layout (retail offsets, see NewGame 0x008F0930): MinX +0x118, MaxX +0x11c, MinY +0x120, MaxY +0x124 (PDB +0x130..+0x13c),
// RoundFinished +0x158.
// Shape: the bool first, then the four bounds from MaxY down to MinX, all from the eax zero; `push 12h` hoisted.
typedef long ETavernGameStates;

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0xec];
    virtual void NewRound();                 // 0x005E9A90
    void SetState(ETavernGameStates state);  // 0x005E9B20
};

struct CTCTavernGameCardBase : CTCTavernGame {
    unsigned char _pad_0xf0[0x28];           // GamePositionRHSet, GamePositionPos, GamePositionActionPointIndex
    float         MinX;                      // +0x118
    float         MaxX;                      // +0x11c
    float         MinY;                      // +0x120
    float         MaxY;                      // +0x124
    unsigned char _pad_0x128[0x30];
    bool          RoundFinished;             // +0x158
    virtual void NewRound();
};

void CTCTavernGameCardBase::NewRound()
{
    CTCTavernGame::NewRound();
    RoundFinished = false;
    MaxY = 0.0f;
    MinY = 0.0f;
    MaxX = 0.0f;
    MinX = 0.0f;
    SetState(18);
}