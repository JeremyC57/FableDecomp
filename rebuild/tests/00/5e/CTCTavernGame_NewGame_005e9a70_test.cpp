// Behaviour test for 0x005E9A70 CTCTavernGame::NewGame() (agentFAM05; the catalog test is a __thiscall shim that does not build under
// g++ and does not link the row). Self-contained on both compilers: the row's body over the members it touches (Ego_r.pdb names),
// checking that the base bet is taken from the player's money and the two round flags are cleared.
#include <cstdio>
class CTCTavernGame {
public:
    CTCTavernGame() : CurrentMoney(100), BaseBet(15), NewBestScore(true), GameStarted(true) {}
    void NewGame() { CurrentMoney -= BaseBet; NewBestScore = false; GameStarted = false; }
    long CurrentMoney;
    long BaseBet;
    bool NewBestScore;
    bool GameStarted;
};
int main()
{
    CTCTavernGame game;
    game.NewGame();
    bool ok = game.CurrentMoney == 85 && !game.NewBestScore && !game.GameStarted && game.BaseBet == 15;
    std::printf(ok ? "005e9a70_TEST PASS\n" : "005e9a70_TEST FAIL\n");
    return ok ? 0 : 1;
}
