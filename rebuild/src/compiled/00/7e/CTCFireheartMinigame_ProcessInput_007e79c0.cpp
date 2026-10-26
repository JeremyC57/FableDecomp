// CTCFireheartMinigame::ProcessInput(EFireheartMinigameInput) -- unless a move is under way, step the hero one cell up/right/down/left on the 4x4 board (clamped to 0..3) and move him there
// Leaf: ProcessInput
//
// Retail 0x007E79C0, 116 bytes including its four-entry jump table (`ret 4`). Ego_r.pdb `?ProcessInput@
// CTCFireheartMinigame@@QAEXW4EFireheartMinigameInput@@@Z`. Callee: CTCFireheartMinigame::SetHeroPos(
// long, long, bool) @ 0x007E7760.
// Layout = Ego_r.pdb CTCFireheartMinigame: XPos +0x68, YPos +0x6c, Moving +0x70 (no retail shift
// before the vector at +0x8c).
// Shape: a switch over 0..3 (input 0 = y-1, 1 = x+1, 2 = y+1, 3 = x-1) on copies of the position in
// edx/eax; each axis clamped with `if (v < 0) v = 0; else if (v > 3) v = 3;`; VC duplicates the
// SetHeroPos tail into both y-clamp arms.
enum EFireheartMinigameInput { FIREHEART_UP, FIREHEART_RIGHT, FIREHEART_DOWN, FIREHEART_LEFT };

class CThing;

class CTCFireheartMinigame { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x68 - 0x08];
    long          XPos;                                     // +0x68
    long          YPos;                                     // +0x6c
    bool          Moving;                                   // +0x70
    void SetHeroPos(long x, long y, bool instant);          // 0x007E7760
    void ProcessInput(EFireheartMinigameInput input);
};

void CTCFireheartMinigame::ProcessInput(EFireheartMinigameInput input)
{
    if (Moving)
        return;
    long x = XPos;
    long y = YPos;
    switch (input)
    {
    case FIREHEART_UP:    --y; break;
    case FIREHEART_RIGHT: ++x; break;
    case FIREHEART_DOWN:  ++y; break;
    case FIREHEART_LEFT:  --x; break;
    }
    if (x < 0)
        x = 0;
    else if (x > 3)
        x = 3;
    if (y < 0)
        y = 0;
    else if (y > 3)
        y = 3;
    SetHeroPos(x, y, false);
}