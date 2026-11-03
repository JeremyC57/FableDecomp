// behaviour test for 0x007CF7E0 CTCTavernGameShoveHaPenny::NewRound(): runs the coin-game base NewRound, reads the
// shove-ha'penny sub-def of the parent thing's base def, clears the board, finds the obstacles, adds the first coin at
// (BoardSize.X / 2, def PositionCoinMinimum, 0) and enters state 15.
#include <cstdio>
#include <string>
struct Def { float positionCoinMinimum; };
class Coin { public: float x, y, z; };
struct Game {
    const Def* def; float boardSizeX; std::string log; Coin coin; long state;
    void BaseNewRound() { log += "N"; }
    void ClearBoard() { log += "C"; }
    void FindObstacles() { log += "F"; }
    void AddNewCoin(const Coin& c) { log += "A"; coin = c; }
    void SetState(long s) { log += "S"; state = s; }
    void NewRound()
    {
        BaseNewRound();
        const Def* d = def;
        ClearBoard();
        FindObstacles();
        Coin c = { 0.5f * boardSizeX, d->positionCoinMinimum, 0.0f };
        AddNewCoin(c);
        SetState(15);
    }
};
int main()
{
    Def def = { 0.25f };
    Game g = { &def, 3.0f, std::string(), { 9, 9, 9 }, 0 };
    g.NewRound();
    bool ok = g.log == "NCFAS" && g.coin.x == 1.5f && g.coin.y == 0.25f && g.coin.z == 0.0f && g.state == 15;
    g.boardSizeX = 8.0f; def.positionCoinMinimum = -1.0f; g.NewRound();
    ok = ok && g.log == "NCFASNCFAS" && g.coin.x == 4.0f && g.coin.y == -1.0f && g.state == 15;
    if (!ok) { std::printf("007cf7e0_TEST FAIL\n"); return 1; }
    std::printf("007cf7e0_TEST PASS\n");
    return 0;
}