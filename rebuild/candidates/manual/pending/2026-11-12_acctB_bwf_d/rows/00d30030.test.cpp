// Behaviour test for NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle (retail 0x00D30030): a copy of the control flow
// over a recording fake game interface -- the limbo passes (first pass hides every thing in the six lists in retail's loop order,
// the second pass restores them), the terminate checks returning early, the boss-fight wait, the result / morality branches
// choosing the epilogue movie and the HeroGoodAtEnd flag.
#include <cstdio>
#include <vector>
#include <string>

struct FakeGame {
    std::vector<std::string> log;
    int frames, terminateAfterFrames, bossPhase, health, result;
    float morality;
    bool goodAtEnd;
    long hidden;
    FakeGame() : frames(0), terminateAfterFrames(-1), bossPhase(0), health(100), result(1), morality(0.0f), goodAtEnd(false), hidden(0) {}
    bool Terminating() const { return terminateAfterFrames >= 0 && frames >= terminateAfterFrames; }
    void NewScriptFrame() { ++frames; if (frames > 3) { bossPhase = 4; } }
    long GetAll(const char* def, std::vector<int>& v) { v.clear(); long n = (long)std::string(def).size() % 4 + 1; for (long k = 0; k < n; ++k) v.push_back(k); return n; }
    void SetInLimbo(int, bool limbo) { hidden += limbo ? 1 : -1; }
    void Play(const char* movie) { log.push_back(movie); }
};

static bool LimboLoop(FakeGame& g, std::vector<int>& v, long n, bool limbo)
{
    long i = 0;
    if (n > 0) for (; i < n; ++i) {
        if (g.Terminating())
            return false;
        g.SetInLimbo(v[i], limbo);
    }
    return !g.Terminating();
}

static void BossBattle(FakeGame& g)
{
    std::vector<int> sconces, chandeliers, chandeliersLong, bookcases, candleRounds, candleTrios;
    long nS = g.GetAll("GENERIC_SCONCE", sconces);
    long nC = g.GetAll("GENERIC_CHANDELIER", chandeliers);
    long nL = g.GetAll("GENERIC_CHANDELIER_LONG", chandeliersLong);
    long nB = g.GetAll("OBJECT_GUILD_BOOKCASE_SELECT", bookcases);
    long nR = g.GetAll("Candle_Flame_Round_01", candleRounds);
    long nT = g.GetAll("Candle_Flame_Trio_01", candleTrios);
    if (!LimboLoop(g, sconces, nS, true) || !LimboLoop(g, chandeliers, nC, true) || !LimboLoop(g, chandeliersLong, nL, true) ||
        !LimboLoop(g, candleRounds, nR, true) || !LimboLoop(g, candleTrios, nT, true) || !LimboLoop(g, bookcases, nB, true))
        return;
    g.log.push_back("hidden");
    while (!(g.bossPhase == 4 || g.health <= 10)) {
        g.NewScriptFrame();
        if (g.Terminating())
            return;
    }
    if (g.Terminating())
        return;
    if (g.result == 1 || g.result == 2) {
        if (g.morality < 0.5f) { g.goodAtEnd = false; g.Play(g.result == 1 ? "9_evil_Sisterkilled" : "10_good_Sisterkilled"); }
        else { g.goodAtEnd = true; g.Play(g.result == 1 ? "9_evil_Sistersaved" : "10_good_Sistersaved"); }
    }
    nS = g.GetAll("GENERIC_SCONCE", sconces); nC = g.GetAll("GENERIC_CHANDELIER", chandeliers);
    nL = g.GetAll("GENERIC_CHANDELIER_LONG", chandeliersLong); nB = g.GetAll("OBJECT_GUILD_BOOKCASE_SELECT", bookcases);
    nR = g.GetAll("Candle_Flame_Round_01", candleRounds); nT = g.GetAll("Candle_Flame_Trio_01", candleTrios);
    if (!LimboLoop(g, sconces, nS, false) || !LimboLoop(g, chandeliers, nC, false) || !LimboLoop(g, bookcases, nB, false) ||
        !LimboLoop(g, chandeliersLong, nL, false) || !LimboLoop(g, candleRounds, nR, false) || !LimboLoop(g, candleTrios, nT, false))
        return;
    g.log.push_back("restored");
}

int main()
{
    FakeGame a; a.result = 1; a.morality = 0.2f; BossBattle(a);
    FakeGame b; b.result = 2; b.morality = 0.9f; BossBattle(b);
    FakeGame c; c.terminateAfterFrames = 0; BossBattle(c);
    FakeGame d; d.result = 0; BossBattle(d);
    bool ok = a.hidden == 0 && a.log.size() == 3 && a.log[1] == "9_evil_Sisterkilled" && !a.goodAtEnd
           && b.log.size() == 3 && b.log[1] == "10_good_Sistersaved" && b.goodAtEnd
           && c.log.empty() && c.hidden == 0
           && d.log.size() == 2 && d.log[1] == "restored" && d.frames == 4;
    if (!ok) { std::printf("00d30030_TEST FAIL\n"); return 1; }
    std::printf("00d30030_TEST PASS\n");
    return 0;
}
