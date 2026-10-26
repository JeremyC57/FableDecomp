// CTCTavernGameCoinGolf::EndRound() -- coin-base EndRound; after a won round on the Oakvale or Snowspire coin-golf table a first or lower score than the hero's best for that table flags a new best score; enters state 8
// Leaf: ?EndRound@CTCTavernGameCoinGolf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D2920, 203 bytes (plain `ret`); CTCTavernGameCoinGolf vtable 0x0126F49C
// slot 33. Ego_r.pdb `?EndRound@CTCTavernGameCoinGolf@@UAEXXZ`.
// Callees: CTCTavernGameCoinBase::EndRound() 0x008EF190 (non-virtual base call; Ego_r.pdb source order LeaveGame,
//          NewGame, NewRound, EndRound); CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0; CThing::GetDefName()
//          const 0x004C7CC0 (CDefString through the hidden pointer); CDefString::operator==(const char*) const 0x005B3440
//          ("OBJECT_TAVERN_TABLE_COIN_GOLF" @ 0x0126F3F4, "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE" @ 0x0126F3CC);
//          CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20. Float pool 0x0122DEDC = 0.0f.
// Layout: CTCTavernGame (retail): ParentThing +0x04, GameWon +0x21, Score +0x2c, NewBestScore +0xa0; retail CTCHeroStats
//         (Ego_r.pdb - 0x18 here): BestScoreCoinGolfOakVale +0x184, BestScoreCoinGolfSnowSpire +0x188.
// Shape: the stats out pointer is zero-initialised; each table test is `name && (best == 0.0f || score < best)` in an
//        if / else-if pair storing the same flag; `best == 0.0f` is `fld [0.0] ; fld best ; fucompp ; test ah,44h ; jnp`,
//        `score < best` is `fld score ; fcomp best ; test ah,5`; the parent thing comes through an inline accessor so it
//        is loaded straight into ecx before the literal push; the CDefString temporaries share one frame word.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCHeroStats : public CTCHeroStatsDonor {
 public:                           // retail layout
};
class CDefString { public:                             // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator==(const char* text) const;    // 0x005B3440
};
class CThing { public:
    CDefString GetDefName() const;              // 0x004C7CC0
};

class CTCTavernGame { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x19];
    bool          GameWon;                      // +0x21
    unsigned char _pad_0x22[0xa];
    float         Score;                        // +0x2c
    unsigned char _pad_0x30[0x70];
    bool          NewBestScore;                 // +0xa0
    CThing* GetParentThing() const { return ParentThing; }
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0
    void SetState(long state);                  // 0x005E9B20
};
struct CTCTavernGameCoinBase : CTCTavernGame {
    void EndRound();                            // 0x008EF190
};
class CTCTavernGameCoinGolf : public CTCTavernGameCoinBase { public:
    void EndRound();
};

void CTCTavernGameCoinGolf::EndRound()
{
    CTCTavernGameCoinBase::EndRound();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (GameWon) {
        if (GetParentThing()->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF"
            && (stats->BestScoreCoinGolfOakVale == 0.0f || Score < stats->BestScoreCoinGolfOakVale))
            NewBestScore = true;
        else if (GetParentThing()->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE"
                 && (stats->BestScoreCoinGolfSnowSpire == 0.0f || Score < stats->BestScoreCoinGolfSnowSpire))
            NewBestScore = true;
    }
    SetState(8);
}