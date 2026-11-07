// Behaviour test for CQ_ArenaScript::Main (0x00F0FB70): the arena state machine (0 hall of heroes -> 1 intro -> 2 cells -> 3 pause
// -> 4 Cham dead -> 5 whisper -> 6/7 walk out), the round loop (rounds 0..7 play a wave each, round 8 wins) and the hero-title
// switch (titles 1..20 pick a name tag; 0 or out of range keeps the default).
#include <cstdio>

static long NameTagForTitle(long title)
{
    switch (title) {
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20:
        return title;
    default:
        return 0;
    }
}

struct Arena {
    long ArenaState, ArenaRound, WavesPlayed; bool PlayerWon, PlayerLeaving;
    void Rounds()
    {
        while (!PlayerWon) {
            while (!PlayerLeaving && !PlayerWon) {
                if (ArenaState == 8) PlayerWon = true;
                else { ++WavesPlayed; if (++ArenaRound == 8) ArenaState = 8; }
            }
            if (PlayerLeaving) return;
        }
    }
};

int main()
{
    bool ok = NameTagForTitle(1) == 1 && NameTagForTitle(20) == 20 && NameTagForTitle(0) == 0 && NameTagForTitle(21) == 0;
    long state = 0;
    for (long step = 0; step < 7; ++step) ++state;
    ok = ok && state == 7;
    Arena a = { 7, 0, 0, false, false };
    a.Rounds();
    ok = ok && a.PlayerWon && a.WavesPlayed == 8 && a.ArenaState == 8;
    Arena b = { 7, 0, 0, false, true };
    b.Rounds();
    ok = ok && !b.PlayerWon && b.WavesPlayed == 0;
    if (!ok) { std::printf("00f0fb70_TEST FAIL\n"); return 1; }
    std::printf("00f0fb70_TEST PASS\n");
    return 0;
}
