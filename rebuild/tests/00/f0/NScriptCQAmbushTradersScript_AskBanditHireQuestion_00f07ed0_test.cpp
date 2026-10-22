// Behaviour test for CQ_AmbushTradersScript::AskBanditHireQuestion @ 0x00F07ED0: a live bandit asks yes/no; "yes" with
// gold >= -cost pays (hero gets cost, bandit gets -cost) and returns true; "yes" while poor says CASH_POOR; "no" or a dead
// bandit falls to NO_HIRE; the bandit only speaks while its health is above zero.
#include <cstdio>
#include <string>
struct World { bool BanditAlive; long Answer; long HeroGold, BanditGold; float Health; std::string Said; };
static bool Ask(World& w, long cost)
{
    if (w.BanditAlive) {
        long answer = w.Answer;
        if (answer == 1) {
            if (w.HeroGold >= -cost) { w.HeroGold += cost; w.BanditGold += -cost; return true; }
            if (w.Health > 0.0f) w.Said = "TEXT_QST_003_BANDIT_CASH_POOR";
            return false;
        }
    }
    if (w.Health > 0.0f) w.Said = "TEXT_QST_003_BANDIT_NO_HIRE";
    return false;
}
int main()
{
    World rich = { true, 1, 500, 0, 1.0f, "" };
    World poor = { true, 1, 10, 0, 1.0f, "" };
    World no = { true, 0, 500, 0, 1.0f, "" };
    World dead = { false, 1, 500, 0, 0.0f, "" };
    bool r1 = Ask(rich, -100), r2 = Ask(poor, -100), r3 = Ask(no, -100), r4 = Ask(dead, -100);
    bool ok = r1 && rich.HeroGold == 400 && rich.BanditGold == 100 && rich.Said.empty() &&
              !r2 && poor.Said == "TEXT_QST_003_BANDIT_CASH_POOR" && poor.HeroGold == 10 &&
              !r3 && no.Said == "TEXT_QST_003_BANDIT_NO_HIRE" && !r4 && dead.Said.empty();
    if (!ok) { std::printf("00f07ed0_TEST FAIL\n"); return 1; }
    std::printf("00f07ed0_TEST PASS\n");
    return 0;
}