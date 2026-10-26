#include <cstdio>
// Behaviour test for CTCTavernGame::EnterGame (0x005EC0F0): the body of src/ over stand-ins -- the game starts in state 1,
// the hero is hidden in limbo and bound to the game, the hero menu closes, the game UI opens, the bet is clamped to the
// hero's money and the round counters reset.
typedef long ETavernGameStates;
static bool g_bTavernGameQuitRequested = false;
static int g_visible = -1, g_limbo = -1, g_menuClose = 0, g_guiClose = 0, g_openUI = 0, g_releases = 0, g_leave = 0;
static void* g_boundGame = 0;
class CTavernGameDef { public: long m_count; long MinBet; void Release() { --m_count; ++g_releases; } };
static CTavernGameDef g_def = { 1, 10 };
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef_005ed4a0(CDefPointer& out) { out.Object = &g_def; ++g_def.m_count; return true; }
};
class CTCTavernGame;
class CTCGamePlayer { public: void SetGame(CTCTavernGame* game) { g_boundGame = game; } };
struct CHeroMenuComponent { void RequestClose() { ++g_menuClose; } };
static CTCGamePlayer g_gamePlayer;
static CHeroMenuComponent g_menu;
class CThing { public:
    CDefClassBase* PDef;
    void SetVisible(bool v) { g_visible = v; }
    template<class T> bool GetInterface(T** ppOut, long type) { if (type != 0xd0) return false; *ppOut = (T*)(void*)&g_gamePlayer; return true; }
    template<class T> bool GetInterfaceDirect(T** ppOut, long type) { if (type != 0xf6) return false; *ppOut = (T*)(void*)&g_menu; return true; }
    void SetInLimbo(bool v) { g_limbo = v; }
};
static CDefClassBase g_defClass;
static CThing g_hero = { &g_defClass };
class CTCHeroStats { public: long Money; };
static CTCHeroStats g_stats = { 75 };
class CPlayer { public: CThing* GetHeroThing() { return &g_hero; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_player; } };
static CPlayerManager g_pm;
class CMainGameComponent { public: CPlayerManager* PlayerManager; };
static CMainGameComponent g_mgc = { &g_pm };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
class CPlayerGui { public: bool m_bShowing; void Close(bool) { ++g_guiClose; } void OpenGameUI() { ++g_openUI; } };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CTCTavernGame { public:
    CThing*           ParentThing;
    bool              IsBeingPlayed;
    bool              GameWon;
    bool              GameFinished;
    long              BaseBet;
    long              LastBet;
    long              TotalWinnings;
    long              CurrentMoney;
    long              NumConsecutiveRoundsWon;
    ETavernGameStates TavernGameState;
    ETavernGameStates PreviousTavernGameState;
    bool              GamePlayed;
    void LeaveGame() { ++g_leave; }
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
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }
    void GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
    void EnterGame();
};

void CTCTavernGame::EnterGame()
{
    IsBeingPlayed = true;
    SetState(1);
    GamePlayed = false;
    GameFinished = false;
    GameWon = false;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    hero->SetVisible(false);
    hero->SetInLimbo(true);
    CDefPointer def;
    GetDef(&def);
    {
        CTCGamePlayer* gamePlayer;
        hero->GetInterface(&gamePlayer, 0xd0);
        gamePlayer->SetGame(this);
    }
    {
        CHeroMenuComponent* menu;
        hero->GetInterfaceDirect(&menu, 0xf6);
        menu->RequestClose();
    }
    g_pPlayerGui->Close(false);
    g_pPlayerGui->m_bShowing = true;
    g_pPlayerGui->OpenGameUI();
    TotalWinnings = 0;
    {
        CTCHeroStats* stats;
        GetHeroStats(&stats);
        if (BaseBet > stats->Money)
            BaseBet = stats->Money;
        LastBet = def->MinBet;
        CurrentMoney = stats->Money;
        NumConsecutiveRoundsWon = 0;
    }
}

static int fail(int line) { printf("005ec0f0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing table = { &g_defClass };
    CTCTavernGame game;
    game.ParentThing = &table;
    game.IsBeingPlayed = false; game.GameWon = true; game.GameFinished = true; game.GamePlayed = true;
    game.BaseBet = 100; game.LastBet = 0; game.TotalWinnings = 9; game.CurrentMoney = 0; game.NumConsecutiveRoundsWon = 4;
    game.TavernGameState = 0; game.PreviousTavernGameState = 0;
    game.EnterGame();
    CHECK(game.IsBeingPlayed && game.TavernGameState == 1 && !game.GamePlayed && !game.GameFinished && !game.GameWon);
    CHECK(g_visible == 0 && g_limbo == 1 && g_boundGame == &game && g_menuClose == 1);
    CHECK(g_guiClose == 1 && g_gui.m_bShowing && g_openUI == 1);
    CHECK(game.TotalWinnings == 0 && game.BaseBet == 75 && game.LastBet == 10 && game.CurrentMoney == 75);
    CHECK(game.NumConsecutiveRoundsWon == 0 && g_releases == 1 && g_def.m_count == 1);
    game.BaseBet = 20;                                   // a bet within the money is kept
    game.EnterGame();
    CHECK(game.BaseBet == 20 && g_leave == 0);
    printf("005ec0f0_TEST PASS\n");
    return 0;
}