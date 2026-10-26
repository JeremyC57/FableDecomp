// Behaviour test for 007dab60: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBlackjack::UpdateValues() -- after a won sitting reports the winnings and the winning streak to the hero's online scoreboard, then rolls the bet over (last bet = bet, bet = 0)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CTCBase;

struct CInterfaceMap {                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);    // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
class CPlayer { public:
    CThing* GetHeroThing();                  // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;          // 0x00449960 (const)
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CTCTavernGameCardBlackjack {         // vptr +0x00 (CTCBase)
    unsigned char _pad_0x04[0x1c];
    bool          IsBeingPlayed;             // +0x20
    bool          GameWon;                   // +0x21
    unsigned char _pad_0x22[0x12];
    long          BaseBet;                   // +0x34
    long          LastBet;                   // +0x38
    long          TotalWinnings;             // +0x3c
    unsigned char _pad_0x40[0x64];
    long          NumConsecutiveRoundsWon;   // +0xa4
    virtual bool UpdateValues();
};

bool CTCTavernGameCardBlackjack::UpdateValues()
{
    if (GameWon) {
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        long winnings = TotalWinnings;
        if (winnings > 0) {
            scoreboard->UpdateScoreForActivity(0x18, (float)winnings, false);
            scoreboard->UpdateScoreForActivity(0x20, (float)NumConsecutiveRoundsWon, false);
        }
    }
    LastBet = BaseBet;
    BaseBet = 0;
    return false;
}

// ---- stubs and driver ----
static CTCHeroOnlineScoreboard g_board;
static long g_calls, g_act[4]; static float g_score[4]; static bool g_only[4];
void CTCHeroOnlineScoreboard::UpdateScoreForActivity(long activity, float score, bool onlyIfBetter)
{ if (this == &g_board && g_calls < 4) { g_act[g_calls] = activity; g_score[g_calls] = score; g_only[g_calls] = onlyIfBetter; } ++g_calls; }
static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CThing g_hero;
static CPlayer g_player;
static CPlayerManager g_manager;
static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
CThing* CPlayer::GetHeroThing() { return this == &g_player ? &g_hero : 0; }
CPlayer* CPlayerManager::GetMainPlayer() const { return this == &g_manager ? &g_player : 0; }
static CTCTavernGameCardBlackjack g_game;
static int Fail(int n) { std::printf("007dab60_TEST FAIL (%d)\n", n); return 1; }

int main()
{
    g_component.PlayerManager = &g_manager;
    g_entries[0].type = 4;     g_entries[0].component = 0;
    g_entries[1].type = 0x10d; g_entries[1].component = (CTCBase*)&g_board;
    g_entries[2].type = 0x200; g_entries[2].component = 0;
    g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 3;
    g_hero.m_presentBits[0x10d >> 5] = 1UL << (0x10d & 31);

    // won sitting with winnings: both activities reported, bet rolled over
    g_game.GameWon = true; g_game.TotalWinnings = 150; g_game.NumConsecutiveRoundsWon = 3;
    g_game.BaseBet = 25; g_game.LastBet = 7;
    if (g_game.UpdateValues() != false) return Fail(1);
    if (g_calls != 2 || g_act[0] != 0x18 || g_score[0] != 150.0f || g_only[0] || g_act[1] != 0x20 || g_score[1] != 3.0f || g_only[1]) return Fail(2);
    if (g_game.LastBet != 25 || g_game.BaseBet != 0) return Fail(3);
    // won but nothing won: no report
    g_calls = 0; g_game.TotalWinnings = 0; g_game.BaseBet = 40;
    g_game.UpdateValues();
    if (g_calls != 0 || g_game.LastBet != 40 || g_game.BaseBet != 0) return Fail(4);
    // not won: no lookup, no report
    g_calls = 0; g_game.GameWon = false; g_game.TotalWinnings = 99; g_game.BaseBet = 5;
    g_game.UpdateValues();
    if (g_calls != 0 || g_game.LastBet != 5 || g_game.BaseBet != 0) return Fail(5);
    std::printf("007dab60_TEST PASS\n");
    return 0;
}