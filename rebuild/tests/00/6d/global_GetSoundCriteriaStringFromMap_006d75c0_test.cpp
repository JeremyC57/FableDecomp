// Behaviour test for 0x006D75C0 (anonymous namespace) GetSoundCriteriaStringFromMap: the sound of the first entry whose
// key is at or above the multiplier, the last entry's sound past the end, and false for an empty map.
#include <cstdio>

struct Entry { long key; long sound; };
struct SoundMap { Entry entries[4]; int count; };

static bool GetSoundCriteriaStringFromMap(const SoundMap& sounds, long multiplier, long& sound)
{
    if (sounds.count == 0)
        return false;
    int it = 0;
    while (it < sounds.count && sounds.entries[it].key < multiplier)   // lower_bound
        ++it;
    if (it == sounds.count)
        --it;
    sound = sounds.entries[it].sound;
    return true;
}

int main()
{
    SoundMap m; m.count = 3;
    m.entries[0].key = 2; m.entries[0].sound = 20;
    m.entries[1].key = 5; m.entries[1].sound = 50;
    m.entries[2].key = 9; m.entries[2].sound = 90;
    long sound = -1;
    if (!GetSoundCriteriaStringFromMap(m, 1, sound) || sound != 20) { std::printf("006d75c0_TEST FAIL\n"); return 1; }
    if (!GetSoundCriteriaStringFromMap(m, 5, sound) || sound != 50) { std::printf("006d75c0_TEST FAIL\n"); return 1; }
    if (!GetSoundCriteriaStringFromMap(m, 6, sound) || sound != 90) { std::printf("006d75c0_TEST FAIL\n"); return 1; }
    if (!GetSoundCriteriaStringFromMap(m, 40, sound) || sound != 90) { std::printf("006d75c0_TEST FAIL\n"); return 1; }
    SoundMap empty; empty.count = 0; sound = 7;
    if (GetSoundCriteriaStringFromMap(empty, 3, sound) || sound != 7) { std::printf("006d75c0_TEST FAIL\n"); return 1; }
    std::printf("006d75c0_TEST PASS\n");
    return 0;
}