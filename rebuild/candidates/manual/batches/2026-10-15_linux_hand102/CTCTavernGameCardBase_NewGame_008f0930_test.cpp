// Behaviour test for CTCTavernGameCardBase::NewGame @ 0x008F0930: base NewGame, selection state and counter reset, state 7.
#include <cstdio>
struct CCard { int id; };
struct CTCTavernGame {
    long BaseBet, CurrentMoney; bool GameFinished, NewBestScore, GameStarted; long state;
    void NewGameBase() { CurrentMoney -= BaseBet; NewBestScore = false; GameStarted = false; }
    void SetState(long s) { state = s; }
};
struct CTCTavernGameCardBase : CTCTavernGame {
    float Counter; long CardOrder; CCard* SelectedCard; CCard* HighlightedCard; long CurrentMovingCard;
    void NewGame()
    {
        NewGameBase(); CardOrder = 0; GameFinished = false; SelectedCard = 0; HighlightedCard = 0; Counter = 0.0f; SetState(7);
    }
};
int main()
{
    CCard c; CTCTavernGameCardBase g; g.BaseBet = 10; g.CurrentMoney = 100; g.GameFinished = true; g.NewBestScore = true; g.GameStarted = true;
    g.state = 0; g.Counter = 5.0f; g.CardOrder = 3; g.SelectedCard = &c; g.HighlightedCard = &c; g.CurrentMovingCard = 4;
    g.NewGame();
    if (g.CurrentMoney != 90 || g.GameFinished || g.NewBestScore || g.GameStarted || g.state != 7 || g.Counter != 0.0f || g.CardOrder
        || g.SelectedCard || g.HighlightedCard || g.CurrentMovingCard != 4) { std::printf("008f0930_TEST FAIL\n"); return 1; }
    std::printf("008f0930_TEST PASS\n");
    return 0;
}
