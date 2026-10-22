// CTCTavernGameShoveHaPenny::EndRound() -- end-of-game override: coin-base hook, then if the game was won and the hero has no best score yet (0) or beat it, flags a new best score; enters state 8
// Retail: Fable.exe @ 0x007CF4C0, 96 bytes (vtable 0x0126F2F4 slot 33).
// Callees: CTCTavernGameCoinBase::vfunc_33 0x008EF190 (non-virtual base call),
//          CTCTavernGame::GetHeroStats(CHeroStats**) 0x005EB4B0 (out parameter),
//          CTCTavernGame::SetState(long) 0x005E9B20.
// Members (retail = PDB - 8 / - 0x1C, see CTCTavernGame.h): GameWon +0x21, Score +0x2C,
// NewBestScore +0xA0; the hero stats record holds the best shove-ha'penny score at +0x18C.
// Shape: the stats pointer is a null-initialised out-parameter local (`mov [esp+8],0` after the
//        push); `best == 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp` (the literal
//        is a pool constant -- 0x0122DEDC), `Score > best` is `fld ; fcomp ; test ah,41h ; jne`.
struct CHeroStats {
    unsigned char _pad[0x18c];
    float m_bestShoveHaPennyScore;      // +0x18c
};

struct CTCTavernGameCoinBase {
    unsigned char _pad[0x21];
    bool GameWon;                       // +0x21
    unsigned char _pad_0x22[0xa];
    float Score;                        // +0x2c
    unsigned char _pad_0x30[0x70];
    bool NewBestScore;                  // +0xa0
    void EndRound();                    // 0x008EF190
    void GetHeroStats(CHeroStats** stats);      // 0x005EB4B0
    void SetState(long state);          // 0x005E9B20
};

struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    void EndRound();
};

void CTCTavernGameShoveHaPenny::EndRound()
{
    CTCTavernGameCoinBase::EndRound();
    CHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (GameWon) {
        if (stats->m_bestShoveHaPennyScore == 0.0f || Score > stats->m_bestShoveHaPennyScore)
            NewBestScore = true;
    }
    SetState(8);
}