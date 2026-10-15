// Behaviour test for CTCTavernGameCardBlackjack::EnterGame @ 0x007DA020: base EnterGame, sitting winnings and indices cleared.
#include <cstdio>
struct CTCTavernGameCardBase { int entered; void EnterGameBase() { ++entered; } };
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    void* CurrentPlayerCards; float MostMoneyWon, MoneyWonThisSitting; long CurrentPlayerCardsIndex, CurrentDealingCardsIndex, CurrentPackCardIndex;
    void EnterGame() { EnterGameBase(); MoneyWonThisSitting = 0.0f; CurrentPlayerCardsIndex = 0; CurrentPackCardIndex = 0; CurrentPlayerCards = 0; }
};
int main()
{
    CTCTavernGameCardBlackjack g; int x; g.entered = 0; g.CurrentPlayerCards = &x; g.MostMoneyWon = 7.0f; g.MoneyWonThisSitting = 3.0f;
    g.CurrentPlayerCardsIndex = 2; g.CurrentDealingCardsIndex = 5; g.CurrentPackCardIndex = 9;
    g.EnterGame();
    if (g.entered != 1 || g.CurrentPlayerCards || g.MoneyWonThisSitting != 0.0f || g.MostMoneyWon != 7.0f || g.CurrentPlayerCardsIndex
        || g.CurrentDealingCardsIndex != 5 || g.CurrentPackCardIndex) { std::printf("007da020_TEST FAIL\n"); return 1; }
    std::printf("007da020_TEST PASS\n");
    return 0;
}
