#include <cstdio>
#include <cstring>
#include <cstdlib>
// Behaviour test for CTCOracleMinigame::Quit (0x007E62D0): the body of src/ over stand-ins -- quit stage, minigame mode
// removed, pending inputs freed, quit wait frame, GUI resets, win/lose text and the scoreboard report.
struct CTCHeroOnlineScoreboard {
    long activity; float score; bool onlyIfBetter;
    void UpdateScoreForActivity(long a, float s, bool o) { activity = a; score = s; onlyIfBetter = o; }
};
static CTCHeroOnlineScoreboard g_board;
struct CThing {
    long requested;
    template<class T> bool GetInterface(T** ppOut, long interfaceType) { requested = interfaceType; *ppOut = (T*)&g_board; return true; }
};
static CThing g_hero;
struct CPlayer {
    long mode; long removed;
    long GetCurrentMode() { return mode; }
    void RemoveMode(long m) { removed = m; mode = 0; }
    CThing* GetHeroThing() { return &g_hero; }
};
static CPlayer g_player;
struct CPlayerManager {
    CPlayer* GetMainPlayer() const { return &g_player; }
};
static CPlayerManager g_manager;
struct CMainGameComponent { const CPlayerManager* PlayerManager; };
static CMainGameComponent g_mgc = { &g_manager };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CWorld { static long GetWorldFrame() { return 1000; } };
static long g_framesPerSecond = 30;
struct CCharString {
    char text[8];
    CCharString(const char* t, long length = -1) { strcpy(text, t); }
};
struct CSubtitleRenderer {
    int calls; char last[8];
    void SetButton2OverrideText(const CCharString& t) { ++calls; strcpy(last, t.text); }
};
struct CDrawQuestInfo {
    long a, b, c;
    void DisplayMiniGameInfo(long x, long y, long z) { a = x; b = y; c = z; }
};
struct CPlayerGui { CSubtitleRenderer Subtitles; bool m_bShowMiniGameInfo; CDrawQuestInfo* QuestInfo; };
static CDrawQuestInfo g_info;
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct COracleMinigameDef { float QuitWaitTime; unsigned long LoseText; unsigned long WinText; };
static int g_freed = 0;
struct CInputList {
    struct node { node* next; node* prev; long value; };
    node* head;
    void clear()
    {
        node* cur = head->next;
        while (cur != head) {
            node* tmp = cur;
            cur = cur->next;
            free(tmp); ++g_freed;
        }
        head->next = head;
        head->prev = head;
    }
};
struct CTCOracleMinigame {
    unsigned long CurrentText;
    long Level;
    long Stage;
    long WaitTime;
    bool Success;
    CInputList InputsPending;
    COracleMinigameDef* PDef;
    CPlayerManager* GetPlayerManager() { return &g_manager; }
    void SetText(unsigned long text) { CurrentText = text; }
    void Quit();
};

void CTCOracleMinigame::Quit()
{
    Stage = 5;
    CPlayer* player = GetPlayerManager()->GetMainPlayer();
    if (player->GetCurrentMode() == 0x29)
        player->RemoveMode(0x29);
    InputsPending.clear();
    long framesPerSecond = g_framesPerSecond;
    COracleMinigameDef* def = PDef;
    WaitTime = (long)(CWorld::GetWorldFrame() + framesPerSecond * def->QuitWaitTime);
    g_pPlayerGui->QuestInfo->DisplayMiniGameInfo(0, 7, 0);
    {
        CCharString empty("");
        g_pPlayerGui->Subtitles.SetButton2OverrideText(empty);
    }
    g_pPlayerGui->m_bShowMiniGameInfo = false;
    if (Success)
        SetText(PDef->WinText);
    else
        SetText(PDef->LoseText);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    CTCHeroOnlineScoreboard* scoreboard;
    hero->GetInterface(&scoreboard, 0x10d);
    scoreboard->UpdateScoreForActivity(0x15, (float)Level, false);
}

static int fail(int line) { printf("007e62d0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CInputList::node sentinel;
    sentinel.next = sentinel.prev = &sentinel;
    for (int i = 0; i < 3; ++i) {
        CInputList::node* n = (CInputList::node*)malloc(sizeof(CInputList::node));
        n->next = sentinel.next; n->prev = &sentinel; sentinel.next->prev = n; sentinel.next = n;
    }
    COracleMinigameDef def = { 2.5f, 11, 22 };
    g_gui.QuestInfo = &g_info; g_gui.m_bShowMiniGameInfo = true; g_gui.Subtitles.calls = 0;
    CTCOracleMinigame game;
    game.Level = 7; game.Stage = 2; game.Success = true; game.PDef = &def; game.InputsPending.head = &sentinel;
    g_player.mode = 0x29; g_player.removed = 0;
    game.Quit();
    CHECK(game.Stage == 5 && g_player.removed == 0x29 && g_freed == 3);
    CHECK(sentinel.next == &sentinel && sentinel.prev == &sentinel);
    CHECK(game.WaitTime == 1075);
    CHECK(g_info.a == 0 && g_info.b == 7 && g_info.c == 0);
    CHECK(g_gui.Subtitles.calls == 1 && g_gui.Subtitles.last[0] == 0 && !g_gui.m_bShowMiniGameInfo);
    CHECK(game.CurrentText == 22);
    CHECK(g_hero.requested == 0x10d && g_board.activity == 0x15 && g_board.score == 7.0f && !g_board.onlyIfBetter);
    // lose path, player not in the minigame mode
    game.Success = false; g_player.mode = 3; g_player.removed = 0;
    game.Quit();
    CHECK(game.CurrentText == 11 && g_player.removed == 0 && g_freed == 3);
    printf("007e62d0_TEST PASS\n");
    return 0;
}
