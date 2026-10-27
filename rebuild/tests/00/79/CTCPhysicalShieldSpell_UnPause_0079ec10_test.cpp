// Behaviour test for 0x0079EC10: UnPause restarts the level's looping sound (player-flagged) and replays the shield effects
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SShield { bool Pausable, Paused; long SpellLevel; long OwnerType; long LoopingSound; int Effects; bool LastPlayer; long LastSound;
    long PlaySound(long id, bool player) { LastSound = id; LastPlayer = player; return id + 1000; }
    void UnPause()
    {
        static const long sounds[3] = { 11, 22, 33 };
        if (Pausable && Paused) { Paused = false; LoopingSound = PlaySound(sounds[SpellLevel], OwnerType == 2); ++Effects; }
    } };
int main()
{
    SShield s; s.Pausable = true; s.Paused = true; s.SpellLevel = 1; s.OwnerType = 2; s.LoopingSound = 0; s.Effects = 0;
    s.UnPause(); CHECK(!s.Paused && s.LastSound == 22 && s.LastPlayer && s.LoopingSound == 1022 && s.Effects == 1);
    s.UnPause(); CHECK(s.Effects == 1);
    s.Paused = true; s.OwnerType = 1; s.SpellLevel = 2; s.UnPause(); CHECK(!s.LastPlayer && s.LoopingSound == 1033);
    if (!g_fail) printf("0079ec10_TEST PASS\n");
    return g_fail;
}