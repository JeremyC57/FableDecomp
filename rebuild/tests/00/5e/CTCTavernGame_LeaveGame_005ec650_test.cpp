#include <cstdio>
// Behaviour test for CTCTavernGame::LeaveGame (0x005EC650): the body of src/ over stand-ins -- the game stops, the state
// goes to 0 (or the pending quit re-enters LeaveGame), a live game master gets a 5-second message, the hero leaves the
// tavern mode and limbo, the GUI closes, the money is written back and the table is cleared.
class C3DVector { public: float X, Y, Z; };
typedef long ETavernGameStates;
static bool g_bTavernGameQuitRequested = false;
static long g_framesPerSecond = 30;
static int g_messages = 0, g_removeMode = -1, g_visible = -1, g_limbo = -1, g_setGame = 0, g_close = 0, g_clearBox = 0,
           g_clearBet = 0, g_subClear = 0;
static long g_msgType = 0, g_money = -1; static unsigned long g_msgDist = 0;
class CTCGamePlayer;
static void* g_gamePlayerPtr = 0;
class CThing { public:
    bool dead;
    C3DVector pos;
    void SetVisible(bool v) { g_visible = v; }
    bool IsDead() const { return dead; }
    template<class T> bool GetInterface(T** ppOut, long type) { if (type != 0xd0) return false; *ppOut = (T*)g_gamePlayerPtr; return true; }
    const C3DVector& GetPosition() const { return pos; }
    void SetInLimbo(bool v) { g_limbo = v; }
};
template<class T> class CIntelligentPointer { public: T* item; T* Get() const { return item; } };
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long distance, C3DVector*, CThing*, CThing*, float, float, long)
    { ++g_messages; g_msgType = type; g_msgDist = distance; return 0; }
};
static CMessageEventManager g_msgMgr;
class CWorld { public: CMessageEventManager* m_pMessages; };
static CWorld g_world = { &g_msgMgr };
class CTCTavernGame;
class CTCGamePlayer { public: void SetGame(CTCTavernGame* game) { if (!game) ++g_setGame; } };
class CTCHeroStats { public: void SetMoney(long money, bool) { g_money = money; } };
static CTCHeroStats g_stats;
static CThing g_hero;
class CPlayer { public:
    void RemoveMode(long mode) { g_removeMode = mode; }
    CThing* GetHeroThing() { return &g_hero; }
};
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
class CMainGameComponent { public: CPlayerManager* PlayerManager; };
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CSubtitleRenderer { public: void Clear(bool) { ++g_subClear; } };
class CPlayerGui { public: CSubtitleRenderer Subtitles; bool m_bShowing; void Close() { ++g_close; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
static int g_leaveCalls = 0;
class CTCTavernGame { public:
    bool              IsBeingPlayed;
    long              CurrentMoney;
    ETavernGameStates TavernGameState;
    ETavernGameStates PreviousTavernGameState;
    CIntelligentPointer<CThing> PGameMaster;
    void LeaveGame();
    void SetState(ETavernGameStates state)
    {
        if (g_bTavernGameQuitRequested) {
            g_bTavernGameQuitRequested = false;
            LeaveGame();
            return;
        }
        PreviousTavernGameState = TavernGameState;
        TavernGameState = state;
    }
    CWorld* GetWorld() { return &g_world; }
    void GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
    void ClearBox() { ++g_clearBox; }
    void ClearBetAndMoney() { ++g_clearBet; }
};
static CTCGamePlayer g_gamePlayerObj;

void CTCTavernGame::LeaveGame()
{
    ++g_leaveCalls;
    IsBeingPlayed = false;
    SetState(0);
    if (PGameMaster.Get() && !PGameMaster.Get()->IsDead()) {
        CThing* gameMaster = PGameMaster.Get();
        long framesPerSecond = g_framesPerSecond;
        CMessageEventManager* messages = GetWorld()->m_pMessages;
        messages->AddMessage(0x40, framesPerSecond * 5, (C3DVector*)&gameMaster->GetPosition(),
                                       PGameMaster.Get(), 0, 0.0f, 0.0f, 0);
    }
    CPlayer* player = g_pMainGameComponent->PlayerManager->GetMainPlayer();
    player->RemoveMode(0x1a);
    CThing* hero = player->GetHeroThing();
    hero->SetVisible(true);
    hero->SetInLimbo(false);
    {
        CTCGamePlayer* gamePlayer;
        hero->GetInterface(&gamePlayer, 0xd0);
        gamePlayer->SetGame(0);
    }
    g_pPlayerGui->Close();
    g_pPlayerGui->m_bShowing = false;
    {
        CTCHeroStats* stats;
        GetHeroStats(&stats);
        stats->SetMoney(CurrentMoney, true);
    }
    ClearBox();
    ClearBetAndMoney();
    g_pPlayerGui->Subtitles.Clear(false);
}

static int fail(int line) { printf("005ec650_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    g_gamePlayerPtr = &g_gamePlayerObj;
    CThing master; master.dead = false;
    CTCTavernGame game;
    game.IsBeingPlayed = true; game.CurrentMoney = 250; game.TavernGameState = 4; game.PreviousTavernGameState = 1;
    game.PGameMaster.item = &master;
    g_gui.m_bShowing = true;
    game.LeaveGame();
    CHECK(!game.IsBeingPlayed && game.TavernGameState == 0 && game.PreviousTavernGameState == 4);
    CHECK(g_messages == 1 && g_msgType == 0x40 && g_msgDist == 150);
    CHECK(g_removeMode == 0x1a && g_visible == 1 && g_limbo == 0 && g_setGame == 1);
    CHECK(g_close == 1 && !g_gui.m_bShowing && g_money == 250 && g_clearBox == 1 && g_clearBet == 1 && g_subClear == 1);

    master.dead = true;                                  // dead game master: no message
    game.LeaveGame();
    CHECK(g_messages == 1 && g_close == 2);

    g_bTavernGameQuitRequested = true;                   // pending quit: SetState re-enters LeaveGame once
    g_leaveCalls = 0;
    game.LeaveGame();
    CHECK(g_leaveCalls == 2 && !g_bTavernGameQuitRequested && g_close == 4);
    printf("005ec650_TEST PASS\n");
    return 0;
}