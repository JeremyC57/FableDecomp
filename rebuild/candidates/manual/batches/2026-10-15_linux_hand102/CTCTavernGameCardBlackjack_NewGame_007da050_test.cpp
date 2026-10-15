// Behaviour test for CTCTavernGameCardBlackjack::NewGame @ 0x007DA050: card-base NewGame, round winnings, hands won and draw cleared.
#include <cstdio>
struct CTCTavernGameCardBase { bool Draw; int newGames; void NewGameBase() { ++newGames; } };
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    long AmountWonThisRound, NumHandsWonThisRound;
    void NewGame() { NewGameBase(); AmountWonThisRound = 0; NumHandsWonThisRound = 0; Draw = false; }
};
int main()
{
    CTCTavernGameCardBlackjack g; g.Draw = true; g.newGames = 0; g.AmountWonThisRound = 40; g.NumHandsWonThisRound = 2;
    g.NewGame();
    if (g.newGames != 1 || g.Draw || g.AmountWonThisRound || g.NumHandsWonThisRound) { std::printf("007da050_TEST FAIL\n"); return 1; }
    std::printf("007da050_TEST PASS\n");
    return 0;
}
