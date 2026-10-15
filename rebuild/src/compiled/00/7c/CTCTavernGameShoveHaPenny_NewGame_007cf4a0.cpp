// CTCTavernGameShoveHaPenny::NewGame() -- coin-base NewGame, then clears the score (+0x2C) and the shove counter (+0x148) and enters state 7
// Retail: Fable.exe @ 0x007CF4A0, 30 bytes (vtable 0x0126F2F4 slot 31 = NewGame).
// Callees: CTCTavernGameCoinBase::NewGame 0x008EF170 (base slot 31, non-virtual call),
//          CTCTavernGame::SetState(long) 0x005E9B20.
// +0x2C is CTCTavernGame::Score (float; PDB +0x34, retail 8 bytes lower because the retail
// UIComponents vector is 0x0C bytes, so the engine header's offsets do not apply).
// Shape: the float `0.0f` and the integer zero share one `xor eax,eax` register (VC7.1 stores a
//        float zero through an integer register when another zero store needs it; both forms
//        -- long and float Score -- grade RELOCATION_MATCH).
struct CTCTavernGameCoinBase {
    unsigned char _pad[0x2c];
    float Score;                        // +0x2c (CTCTavernGame)
    unsigned char _pad_0x30[0x118];
    long m_numShoves;                   // +0x148
    void NewGame();                     // 0x008EF170
    void SetState(long state);          // 0x005E9B20
};

struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    void NewGame();
};

void CTCTavernGameShoveHaPenny::NewGame()
{
    CTCTavernGameCoinBase::NewGame();
    Score = 0.0f;
    m_numShoves = 0;
    SetState(7);
}