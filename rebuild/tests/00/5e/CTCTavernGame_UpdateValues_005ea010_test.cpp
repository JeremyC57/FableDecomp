// Behaviour test for 005ea010: body copied from src/005ea010.cpp (CTCTavernGame::UpdateValues), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CPrizeScoreDef { public:
    unsigned char _pad00[0x28];
    float Score;
    long Mult;
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad08[0x170 - 0x08];
    bool BestScoreHigh;
    CPrizeScoreDef* PrizeScores_first;
    CPrizeScoreDef* PrizeScores_last;
};
static CTavernGameDef* g_gameDef = 0; static int g_getSubDef = 0;
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out) { ++g_getSubDef; if (!g_gameDef) return false; ++g_gameDef->RefCount; out.Object = g_gameDef; return true; }
};
class CHeroStatsDef { public:
    unsigned char _pad00[0x5c];
    long MaximumMoney;
};
struct CDefinitionHolder {
    unsigned char _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;
};
static CDefinitionHolder g_holder; static CHeroStatsDef g_heroStats;
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;
};
class CTCTavernGame { public:
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad08[0x20 - 0x08];
    bool IsBeingPlayed;
    bool GameWon;
    unsigned char _pad22[0x2c - 0x22];
    float Score;
    float Target;
    long BaseBet;
    long LastBet;
    long TotalWinnings;
    long CurrentMoney;
    unsigned char _pad44[0xa4 - 0x44];
    long NumConsecutiveRoundsWon;
    CDefinitionHolder* GetDefinitions() const { return &g_holder; }
    bool UpdateValues();
};
bool CTCTavernGame::UpdateValues()
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefOut<CTavernGameDef> defOut;
    thingDef->GetSubDef_005ed4a0(defOut);
    CDefHolder<CTavernGameDef> def(defOut.Object);
    bool prizeWon = false;
    if (GameWon) {
        ++NumConsecutiveRoundsWon;
        for (CPrizeScoreDef* prize = def->PrizeScores_first; prize != def->PrizeScores_last; ++prize) {
            if (def->BestScoreHigh ? Score >= prize->Score : Score <= prize->Score) {
                CurrentMoney += BaseBet * prize->Mult;
                if (CurrentMoney > GetDefinitions()->PHeroStatsDef->MaximumMoney)
                    CurrentMoney = GetDefinitions()->PHeroStatsDef->MaximumMoney;
                TotalWinnings += (prize->Mult - 1) * BaseBet;
                prizeWon = true;
                break;
            }
        }
    } else {
        TotalWinnings -= BaseBet;
        NumConsecutiveRoundsWon = 0;
    }
    LastBet = BaseBet;
    BaseBet = 0;
    return prizeWon;
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CTavernGameDef gameDef; gameDef.RefCount = 1; g_gameDef = &gameDef; thing.PDef = &classDef;
    g_holder.PHeroStatsDef = &g_heroStats; g_heroStats.MaximumMoney = 1000;
    CPrizeScoreDef prizes[3]; memset(prizes, 0, sizeof(prizes)); prizes[0].Score = 100; prizes[0].Mult = 5; prizes[1].Score = 50; prizes[1].Mult = 3; prizes[2].Score = 10; prizes[2].Mult = 2;
    gameDef.PrizeScores_first = prizes; gameDef.PrizeScores_last = prizes + 3; gameDef.BestScoreHigh = true;
    CTCTavernGame g; memset(&g, 0, sizeof(g)); g.ParentThing = &thing;
    // lost round: the bet is lost, the streak reset
    g.GameWon = false; g.BaseBet = 20; g.TotalWinnings = 100; g.NumConsecutiveRoundsWon = 4; g.CurrentMoney = 500;
    CHECK(!g.UpdateValues()); CHECK(g.TotalWinnings == 80 && g.NumConsecutiveRoundsWon == 0 && g.LastBet == 20 && g.BaseBet == 0 && g.CurrentMoney == 500); CHECK(g_getSubDef == 1 && gameDef.RefCount == 1 && g_destroyed == 0);
    // won round, high scores better: score 60 reaches the second prize (x3)
    g.GameWon = true; g.BaseBet = 20; g.Score = 60;
    CHECK(g.UpdateValues()); CHECK(g.CurrentMoney == 560 && g.TotalWinnings == 120 && g.NumConsecutiveRoundsWon == 1 && g.LastBet == 20 && g.BaseBet == 0);
    // won round, first prize, money capped at the hero maximum
    g.BaseBet = 200; g.Score = 150; CHECK(g.UpdateValues()); CHECK(g.CurrentMoney == 1000 && g.TotalWinnings == 120 + 800 && g.NumConsecutiveRoundsWon == 2);
    // won round but no prize reached: no payout, streak still counts
    g.BaseBet = 10; g.Score = 5; CHECK(!g.UpdateValues()); CHECK(g.CurrentMoney == 1000 && g.TotalWinnings == 920 && g.NumConsecutiveRoundsWon == 3 && g.LastBet == 10);
    // low scores better: score 8 is at or below the first entry (100), so the first prize (x5) pays
    gameDef.BestScoreHigh = false; g.BaseBet = 10; g.Score = 8; g.CurrentMoney = 10;
    CHECK(g.UpdateValues()); CHECK(g.CurrentMoney == 60 && g.TotalWinnings == 960);
    // a def held only by the lookup is destroyed
    gameDef.RefCount = 0; g.GameWon = false; g.UpdateValues(); CHECK(g_destroyed == 1);
    if (fails == 0) printf("005ea010_TEST PASS\n"); else printf("005ea010_TEST FAIL\n");
    return fails;
}