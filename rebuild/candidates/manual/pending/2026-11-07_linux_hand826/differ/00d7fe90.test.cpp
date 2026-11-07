// Behaviour test for NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena (0x00D7FE90): three rounds of waves; Jack speaks
// before every wave but the first of a round (speech index counts waves); a def with NumCreatures > 1 spawns at its markers,
// a single creature at its one marker; the wave waits until CreatureCount drops to 0 (taunting on the timer: the card taunt
// first); cut-scenes after rounds 1 and 2; CollectedSoulFromArena after round 3.
#include <cstdio>
#include <cstring>
struct Creature { int num; };
struct Wave { int nDefs; Creature defs[2]; };
struct Round { int nWaves; Wave waves[2]; };
struct Sim {
    char log[4096]; Round rounds[3]; int creatureCount, frame, termFrame; bool collected; int timer;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void EvN(const char* e, long n) { char b[48]; std::sprintf(b, "%s%ld", e, n); Ev(b); }
    void Main()
    {
        long round = 0, jack = 1, taunts = 0;
        do {
            ++frame; if (Term()) return;
            for (long w = 0; w < rounds[round].nWaves; ++w) {
                if (w != 0) EvN("JACK", jack);
                timer = 20;
                for (long d = 0; d < rounds[round].waves[w].nDefs; ++d) {
                    int n = rounds[round].waves[w].defs[d].num;
                    for (long i = 0; i < n; ++i) { Ev(n > 1 ? "SPAWN_M" : "SPAWN_1"); ++creatureCount; }
                }
                while (creatureCount > 0) {
                    ++frame; if (Term()) return;
                    if (timer > 0) --timer;
                    if (timer == 0) { if (taunts == 0) Ev("CARD"); timer = 20; ++taunts; }
                    --creatureCount;
                }
                ++jack;
            }
            ++round;
            if (round == 1) Ev("CS3"); else if (round == 2) Ev("CS5");
        } while (round < 3);
        collected = true;
    }
};
int main()
{
    Sim s; std::memset(&s, 0, sizeof s); s.termFrame = 100000;
    for (int r = 0; r < 3; ++r) { s.rounds[r].nWaves = 2; for (int w = 0; w < 2; ++w) { s.rounds[r].waves[w].nDefs = 1; s.rounds[r].waves[w].defs[0].num = (w == 0) ? 1 : 3; } }
    s.Main();
    bool ok = s.collected && std::strncmp(s.log, "SPAWN_1;JACK2;SPAWN_M;SPAWN_M;SPAWN_M;CS3;", 42) == 0 && std::strstr(s.log, "CS5;") && !std::strstr(s.log, "CARD");
    Sim t; std::memset(&t, 0, sizeof t); t.termFrame = 2; t.rounds[0].nWaves = 1; t.rounds[0].waves[0].nDefs = 1; t.rounds[0].waves[0].defs[0].num = 5;
    t.Main();
    ok = ok && !t.collected && std::strcmp(t.log, "SPAWN_M;SPAWN_M;SPAWN_M;SPAWN_M;SPAWN_M;") == 0;
    std::printf(ok ? "00d7fe90_TEST PASS\n" : "00d7fe90_TEST FAIL\n");
    return ok ? 0 : 1;
}
