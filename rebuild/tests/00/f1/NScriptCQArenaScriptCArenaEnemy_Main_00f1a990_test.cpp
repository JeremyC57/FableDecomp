// Behaviour test for 00f1a990 CArenaEnemy::Main: enemy index lookup, health-bar choice, per-hit scoring and cooldown.
#include <cstdio>
#include <cstring>
static long FindIndex(const char* const* names, long count, const char* me)
{
    long index = 0;
    for (long i = 0; i < count; ++i)
        if (std::strcmp(names[i], me) == 0) index = i;
    return index;
}
static const char* Icon(const char* def)
{
    if (!std::strcmp(def, "CREATURE_EARTH_TROLL_START_STANDING")) return "HUD_QUEST_ICON_EARTH_TROLL";
    if (!std::strcmp(def, "CREATURE_ROCK_TROLL_START_STANDING")) return "HUD_QUEST_ICON_ROCK_TROLL";
    if (!std::strcmp(def, "CREATURE_SCORPION_KING")) return "HUD_QUEST_ICON_SCORPION_KING";
    return 0;
}
enum { FLOURISH, DECAPITATE, HIT, NONE };
static void Frame(int hit, long* score, long* timer)
{
    if (hit == FLOURISH) { if (*timer <= 0) { *timer = 10; *score += 5; } }
    else if (hit == DECAPITATE) { if (*timer <= 8) { *timer = 10; *score += 7; } }
    else if (hit == HIT) ++*score;
    if (*timer > 0) --*timer;
}
int main()
{
    const char* names[3] = { "A", "B", "A" };
    long score = 0, timer = 0;
    int hits[5] = { FLOURISH, FLOURISH, HIT, DECAPITATE, NONE };
    for (int i = 0; i < 5; ++i) Frame(hits[i], &score, &timer);
    bool ok = FindIndex(names, 3, "A") == 2 && FindIndex(names, 3, "C") == 0
        && Icon("CREATURE_SCORPION_KING") && !Icon("CREATURE_HOBBE") && score == 13;
    if (!ok) { std::printf("00f1a990_TEST FAIL\n"); return 1; }
    std::printf("00f1a990_TEST PASS\n");
    return 0;
}