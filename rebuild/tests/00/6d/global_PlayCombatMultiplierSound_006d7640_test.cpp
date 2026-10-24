// Behaviour test for 0x006D7640 (anonymous namespace) PlayCombatMultiplierSound: a thing with a sound player plays the
// map's sound for the multiplier (attached, not looping); a thing without one, or an empty map, plays nothing.
#include <cstdio>

struct SoundPlayer { int plays; long lastSound; bool lastAttach, lastLoop, lastB3; long lastD; };
struct Thing { bool hasSoundPlayer; SoundPlayer player; };
struct Entry { long key; long sound; };
struct SoundMap { Entry entries[4]; int count; };

static bool GetSoundCriteriaStringFromMap(const SoundMap& sounds, long multiplier, long& sound)
{
    if (sounds.count == 0) return false;
    int it = 0;
    while (it < sounds.count && sounds.entries[it].key < multiplier) ++it;
    if (it == sounds.count) --it;
    sound = sounds.entries[it].sound;
    return true;
}

static void PlayCombatMultiplierSound(Thing& thing, const SoundMap& sounds, long multiplier)
{
    if (thing.hasSoundPlayer) {
        long sound = 0;
        if (GetSoundCriteriaStringFromMap(sounds, multiplier, sound)) {
            SoundPlayer& p = thing.player;
            ++p.plays; p.lastSound = sound; p.lastAttach = true; p.lastLoop = false; p.lastB3 = false; p.lastD = -1;
        }
    }
}

int main()
{
    SoundMap m; m.count = 2; m.entries[0].key = 2; m.entries[0].sound = 20; m.entries[1].key = 6; m.entries[1].sound = 60;
    Thing t; t.hasSoundPlayer = true; t.player.plays = 0;
    PlayCombatMultiplierSound(t, m, 4);
    if (t.player.plays != 1 || t.player.lastSound != 60 || !t.player.lastAttach || t.player.lastLoop || t.player.lastD != -1) { std::printf("006d7640_TEST FAIL\n"); return 1; }
    SoundMap empty; empty.count = 0;
    PlayCombatMultiplierSound(t, empty, 4);
    if (t.player.plays != 1) { std::printf("006d7640_TEST FAIL\n"); return 1; }
    Thing silent; silent.hasSoundPlayer = false; silent.player.plays = 0;
    PlayCombatMultiplierSound(silent, m, 1);
    if (silent.player.plays != 0) { std::printf("006d7640_TEST FAIL\n"); return 1; }
    std::printf("006d7640_TEST PASS\n");
    return 0;
}