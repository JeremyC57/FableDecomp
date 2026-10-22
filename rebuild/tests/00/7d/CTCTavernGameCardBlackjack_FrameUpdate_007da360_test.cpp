#include <cstdio>

struct CTCTavernGameCardBlackjack {
    long TavernGameState; int baseCalls, turns, updates;
    CTCTavernGameCardBlackjack() : TavernGameState(0), baseCalls(0), turns(0), updates(0) {}
    void BaseFrameUpdate() { ++baseCalls; }
    void TurnPlayerCards() { ++turns; }
    virtual void UpdateGameState() { ++updates; }
    virtual ~CTCTavernGameCardBlackjack() {}
    void FrameUpdate()
    {
        BaseFrameUpdate();
        if (TavernGameState >= 28) {
            if (TavernGameState > 37) {
                if (TavernGameState == 38)
                    TurnPlayerCards();
            } else
                UpdateGameState();
        }
    }
};
static bool Run()
{
    CTCTavernGameCardBlackjack g;
    static const long states[] = { 0, 27, 28, 37, 38, 39, 50 };
    for (unsigned i = 0; i < 7; ++i) { g.TavernGameState = states[i]; g.FrameUpdate(); }
    return g.baseCalls == 7 && g.updates == 2 && g.turns == 1;
}

int main()
{
    if (!Run()) { std::printf("007da360_TEST FAIL\n"); return 1; }
    std::printf("007da360_TEST PASS\n");
    return 0;
}