// CTCTavernGameSpotTheAddition::EndRound() -- end-of-game override: tavern-game hook, score = the time taken, and a won game with no best time yet (0) or a faster time flags a new best score; enters state 8
// Retail: Fable.exe @ 0x007D7BA0, 101 bytes; CTCTavernGameSpotTheAddition vtable 0x0126F814 slot 33 (same slot as the landed
//         CTCTavernGameShoveHaPenny::vfunc_33 0x007CF4C0).
// Callees: CTCTavernGame::vfunc_33 0x005E9AA0 (non-virtual base call; manifest "CXMVPlaybackInfo" ctor label is a
//          propagation); CTCTavernGame::GetHeroStats(CHeroStats**) 0x005EB4B0 (out parameter); CTCTavernGame::SetState(long)
//          0x005E9B20. Float pool 0x0122DEDC = 0.0f.
// Layout: CTCTavernGame (retail): GameWon +0x21, Score +0x2c, NewBestScore +0xa0; retail CTCTavernGameSpotTheAddition::
//         CurrentTime +0xf4 (PDB +0x10c); the hero stats record holds the best spot-the-addition time at +0x190.
// Shape: the stats out pointer is UNinitialised (no zero store); `GameWon == true` compares against the dl=1 that is
//        reused for the `true` store; `best == 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`,
//        `time < best` is `fld time ; fcomp best ; test ah,5 ; jp`.
struct CHeroStats {
    unsigned char _pad[0x190];
    float m_bestSpotTheAdditionTime;    // +0x190
};

struct CTCTavernGame {
    unsigned char _pad[0x21];
    bool GameWon;                       // +0x21
    unsigned char _pad_0x22[0xa];
    float Score;                        // +0x2c
    unsigned char _pad_0x30[0x70];
    bool NewBestScore;                  // +0xa0
    unsigned char _pad_0xa1[0x53];
    void EndRound();                    // 0x005E9AA0
    void GetHeroStats(CHeroStats** stats);      // 0x005EB4B0
    void SetState(long state);          // 0x005E9B20
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    float CurrentTime;                  // +0xf4
    void EndRound();
};

void CTCTavernGameSpotTheAddition::EndRound()
{
    CTCTavernGame::EndRound();
    CHeroStats* stats;
    GetHeroStats(&stats);
    Score = CurrentTime;
    if (GameWon == true) {
        if (stats->m_bestSpotTheAdditionTime == 0.0f || CurrentTime < stats->m_bestSpotTheAdditionTime)
            NewBestScore = true;
    }
    SetState(8);
}