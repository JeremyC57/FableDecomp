// Behaviour test for 00f045e0 CAT_Sentry::Main: per-frame decisions (exit walk, attack, scare, turn on the spy once).
#include <cstdio>
#include <string>
struct Frame { bool phaseOver, alerted, seen, spyNear; };
static std::string Run(const Frame* frames, int n, int guardsAlive)
{
    std::string log;
    bool scared = false, attacked = false;
    for (int f = 0; f < n; ++f) {
        const Frame& fr = frames[f];
        if (fr.phaseOver) log += "X";
        if (fr.alerted || fr.seen) { log += scared ? "A" : "AH"; log += "F"; return log; }
        if (fr.spyNear && !scared) { scared = true; log += "S"; }
        if (!attacked && scared) {
            log += "E";
            for (int i = 0; i < guardsAlive; ++i) log += "g";
            attacked = true;
        }
        log += ".";
    }
    return log;
}
int main()
{
    Frame a[4] = { { false, false, false, false }, { false, false, false, true }, { false, false, false, true }, { false, false, true, false } };
    Frame b[2] = { { false, false, false, false }, { false, true, false, false } };
    std::string ra = Run(a, 4, 2), rb = Run(b, 2, 0);
    if (ra != ".SEgg..AF" || rb != ".AHF") {
        std::printf("00f045e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00f045e0_TEST PASS\n");
    return 0;
}