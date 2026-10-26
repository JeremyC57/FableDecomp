// Behaviour test for 007d2920: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCoinGolf::EndRound() -- coin-base EndRound; after a won round on the Oakvale or Snowspire coin-golf table a first or lower score than the hero's best for that table flags a new best score; enters state 8
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CTCHeroStats { public:                           // retail layout
    unsigned char _pad[0x184];
    float BestScoreCoinGolfOakVale;             // +0x184
    float BestScoreCoinGolfSnowSpire;           // +0x188
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

// ---- stubs and driver ----
void CTCTavernGame::v00() {}
static int g_base; static long g_state = -1;
void CTCTavernGameCoinBase::EndRound() { ++g_base; }
static CTCHeroStats g_stats;
void CTCTavernGame::GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
void CTCTavernGame::SetState(long state) { g_state = state; }
static const char* g_name;
CDefString::CDefString() : m_tablePos(0) {}
CDefString CThing::GetDefName() const { CDefString s; s.m_tablePos = 1; return s; }
bool CDefString::operator==(const char* text) const { return m_tablePos == 1 && std::strcmp(g_name, text) == 0; }

static bool Run(const char* name, bool won, float score, float oak, float snow)
{
    static CThing thing;
    static CTCTavernGameCoinGolf game;
    game.ParentThing = &thing; game.GameWon = won; game.Score = score; game.NewBestScore = false;
    g_name = name; g_stats.BestScoreCoinGolfOakVale = oak; g_stats.BestScoreCoinGolfSnowSpire = snow;
    g_state = -1;
    game.EndRound();
    if (g_state != 8) { std::printf("007d2920_TEST FAIL (state)\n"); return false; }
    return game.NewBestScore;
}

int main()
{
    const char* oak = "OBJECT_TAVERN_TABLE_COIN_GOLF";
    const char* snow = "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE";
    bool ok = true;
    ok = ok && Run(oak, true, 5.0f, 0.0f, 3.0f);        // no best yet
    ok = ok && Run(oak, true, 5.0f, 6.0f, 1.0f);        // better
    ok = ok && !Run(oak, true, 7.0f, 6.0f, 9.0f);       // worse (snowspire best is not consulted for the oak table)
    ok = ok && !Run(oak, false, 1.0f, 0.0f, 0.0f);      // lost
    ok = ok && Run(snow, true, 4.0f, 1.0f, 0.0f);
    ok = ok && Run(snow, true, 4.0f, 1.0f, 5.0f);
    ok = ok && !Run(snow, true, 6.0f, 9.0f, 5.0f);
    ok = ok && !Run("OBJECT_SOMETHING_ELSE", true, 1.0f, 0.0f, 0.0f);
    if (!ok || g_base != 8) { std::printf("007d2920_TEST FAIL\n"); return 1; }
    std::printf("007d2920_TEST PASS\n");
    return 0;
}