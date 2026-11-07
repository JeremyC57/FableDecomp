// Behaviour test for NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Main (0x00D56790): Whisper walks to her stand once the hero
// talked to her; per round she waits for tutorial state 3 (sword hits count up to 7, a plain hit shows the wield help on the first of
// every 5), state 4 (blocked hits count, an unblocked hit shows the block help on the first of every 5), state 6 (insults or Thunder's
// lines) and state 7, then repeats while the parent's MeleeRepeating says so. Insults fire only when the 15-frame timer is <= 8 and
// a wait insult when it runs out.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Opponent {
    int Frames, TerminateAt, Timer, Counter, Rounds, Repeats;
    bool Repeat;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (Timer > 0) --Timer; }
    void Main()
    {
        Log("walk_to_stand");
        while (Repeat) {
            NSF(); if (T()) return;
            int hits = 0; Timer = 15;
            for (int i = 0; i < 4 && Counter < 7; ++i) {          // state 3: four sword hits, one plain hit
                NSF(); if (T()) return;
                if (i < 3) { ++Counter; if (Timer <= 8) { Log("MELEE_HIT_INSULT"); Timer = 15; } }
                else { if (hits == 0) Log("SWORD_WIELD_HELP"); hits = (hits + 1) % 5; }
                if (Timer <= 0) { Log("MELEE_WAIT_INSULT"); Timer = 15; }
            }
            Timer = 15; hits = 0;
            for (int i = 0; i < 2; ++i) {                         // state 4: two unblocked hits
                NSF(); if (T()) return;
                if (hits == 0) { Log("MAZE_BLOCK"); Log("BLOCK_HELP"); Timer = 15; }
                hits = (hits + 1) % 5;
            }
            ++Rounds;
            if (Rounds >= Repeats) Repeat = false;
            Log("reset");
        }
    }
};
int main()
{
    Opponent a = { 0, 100, 0, 0, 0, 2, true };
    a.Main();
    bool ok = !std::strcmp(g_log, "walk_to_stand;SWORD_WIELD_HELP;MAZE_BLOCK;BLOCK_HELP;reset;SWORD_WIELD_HELP;MAZE_BLOCK;BLOCK_HELP;reset;") && a.Rounds == 2 && a.Counter == 6;
    g_log[0] = 0;
    Opponent b = { 0, 3, 0, 0, 0, 5, true };
    b.Main();
    ok = ok && !std::strcmp(g_log, "walk_to_stand;") && b.Rounds == 0;
    std::printf(ok ? "00d56790_TEST PASS\n" : "00d56790_TEST FAIL\n");
    return ok ? 0 : 1;
}
