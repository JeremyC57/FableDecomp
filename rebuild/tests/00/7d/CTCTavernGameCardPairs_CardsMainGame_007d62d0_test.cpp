#include <cstdio>
#include <cstring>
// behaviour test: CTCTavernGameCardPairs::CardsMainGame turns back a mismatched pair, runs the timer, and on all cards face-up plays the win/lose sound and shows the result
class CCard { public: bool Turning, FaceDown; void Turn() { FaceDown = !FaceDown; } };
static const char* g_lastSound = 0; static int g_sounds = 0; static long g_state = 0;
struct CPairs {
    CCard* Selected; CCard* Alternate; CCard* Cards[4]; int NumCards;
    bool TimerStopped, GameStarted, GameFinished, GameWon; long GameType, NumRounds; float Counter, TargetTime;
    void PlaySound(const char* s) { g_lastSound = s; ++g_sounds; }
    void CardsMainGame()
    {
        if (Selected && Alternate && !Selected->Turning && !Alternate->Turning) {
            if (Selected->FaceDown && Alternate->FaceDown) { Selected = 0; Alternate = 0; }
            else {
                if (!Selected->FaceDown) { PlaySound("TAVERN_GAME_CARD_FLIP"); Selected->Turn(); }
                if (!Alternate->FaceDown) { PlaySound("TAVERN_GAME_CARD_FLIP"); Alternate->Turn(); }
            }
        }
        bool stopped = TimerStopped;
        if (!stopped && GameType == 1 && GameStarted) Counter += 1.0f / 30.0f;
        if (Counter < 0.0f) Counter = 0.0f;
        if (GameType == 1 && !stopped && Counter > TargetTime) TimerStopped = true;
        if (!TimerStopped) for (int i = 0; i < NumCards; ++i) if (Cards[i]->FaceDown) return;
        TimerStopped = true;
        if (GameType == 1) { if (NumRounds <= 1) GameFinished = true; GameWon = Counter <= TargetTime; }
        for (int i = 0; i < NumCards; ++i) if (Cards[i]->Turning) return;
        PlaySound(GameWon ? "TAVERN_GAME_WIN_GAME" : "TAVERN_GAME_LOSE_GAME");
        g_state = 0x18;
    }
};
int main()
{
    CCard c0 = { false, false }, c1 = { false, true };
    CPairs p; p.Selected = &c0; p.Alternate = &c1; p.Cards[0] = &c0; p.Cards[1] = &c1; p.NumCards = 2;
    p.TimerStopped = false; p.GameStarted = true; p.GameFinished = false; p.GameWon = false; p.GameType = 1; p.NumRounds = 1;
    p.Counter = 0.0f; p.TargetTime = 10.0f;
    p.CardsMainGame();
    bool ok = c0.FaceDown && c1.FaceDown && g_sounds == 1 && !p.TimerStopped && p.Counter > 0.0f && g_state == 0;
    p.CardsMainGame();
    ok = ok && p.Selected == 0 && p.Alternate == 0;
    c0.FaceDown = false; c1.FaceDown = false; p.CardsMainGame();
    ok = ok && p.TimerStopped && p.GameWon && p.GameFinished && g_state == 0x18 && std::strcmp(g_lastSound, "TAVERN_GAME_WIN_GAME") == 0;
    if (!ok) { std::printf("007d62d0_TEST FAIL\n"); return 1; }
    std::printf("007d62d0_TEST PASS\n");
    return 0;
}