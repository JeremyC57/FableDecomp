// Behaviour test for 0x00DFCA90 NScript::CQ_TraderConflictGoodScript::CBanditExtra::Main: always pauses 0.5 s; spawns a
// CREATURE_BANDIT_GRUNT_LEVEL2 at Me (no script name, no flag) only while fewer than ten creatures are alive AND the hero is more
// than 14 m away (and the thread is not terminating); the chance divisor is ceil(distance(bandit, hero) / 15) capped at 3 (the cap
// re-checks termination); a zero remainder of rand() gives the bandit the hero as best enemy target (after a termination check);
// a spawn always ends with the live-enemy rebuild.
#include <cstdio>
#include <string>
#include <vector>
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static long Ceil(float v) { long r = (long)v; if ((float)r < v) ++r; return r; }
struct Game {
    std::vector<float> pauses; std::vector<std::string> spawned; int targets; int rebuilds;
    float heroDistance, banditDistance; int alive; int terminateAt; int checks; int randValue; long lastDivisor;
    Game() : targets(0), rebuilds(0), heroDistance(20.0f), banditDistance(10.0f), alive(0), terminateAt(-1), checks(0), randValue(0), lastDivisor(0) { }
    bool Terminating() { return checks++ == terminateAt; }
};
static void Main(Game& g) {
    g.pauses.push_back(0.5f);
    if (g.alive < 10 && g.heroDistance > 14.0f) {
        if (g.Terminating()) return;
        g.spawned.push_back("CREATURE_BANDIT_GRUNT_LEVEL2");
        long chance = Ceil(g.banditDistance * (1.0f / 15.0f));
        if (chance > 3) {
            if (g.Terminating()) return;
            chance = 3;
        }
        g.lastDivisor = chance;
        if (g.randValue % chance == 0) {
            if (g.Terminating()) return;
            ++g.targets;
        }
        ++g.rebuilds;
    }
}
int main() {
    { Game g; g.alive = 10; Main(g); check(g.pauses.size() == 1 && g.spawned.empty() && g.rebuilds == 0, "ten alive: pause only"); }
    { Game g; g.heroDistance = 14.0f; Main(g); check(g.spawned.empty() && g.rebuilds == 0, "hero at 14 m: no spawn (strictly over)"); }
    { Game g; g.alive = 9; g.banditDistance = 10.0f; g.randValue = 0; Main(g);
      check(g.spawned.size() == 1 && g.lastDivisor == 1 && g.targets == 1 && g.rebuilds == 1, "near bandit: divisor 1, targets hero"); }
    { Game g; g.banditDistance = 40.0f; g.randValue = 2; Main(g); check(g.lastDivisor == 3 && g.targets == 0 && g.rebuilds == 1, "40 m: ceil 2.67 = 3, 2 % 3 != 0"); }
    { Game g; g.banditDistance = 100.0f; g.randValue = 6; Main(g); check(g.lastDivisor == 3 && g.targets == 1, "100 m: capped at 3, 6 % 3 == 0"); }
    { Game g; g.banditDistance = 20.0f; g.randValue = 3; Main(g); check(g.lastDivisor == 2 && g.targets == 0, "20 m: divisor 2, odd roll"); }
    { Game g; g.terminateAt = 0; Main(g); check(g.spawned.empty() && g.rebuilds == 0, "terminating before the spawn"); }
    { Game g; g.banditDistance = 100.0f; g.terminateAt = 1; Main(g); check(g.spawned.size() == 1 && g.rebuilds == 0 && g.targets == 0, "terminating at the cap"); }
    { Game g; g.randValue = 0; g.terminateAt = 1; Main(g); check(g.targets == 0 && g.rebuilds == 0, "terminating before the target"); }
    if (!fails) std::printf("00dfca90_TEST PASS\n");
    return fails;
}
