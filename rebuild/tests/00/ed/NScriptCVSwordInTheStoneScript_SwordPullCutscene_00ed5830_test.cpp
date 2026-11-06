// Behaviour test for CV_SwordInTheStoneScript::SwordPullCutscene (0x00ED5830): the first attempt records the hero's
// physique / toughness / health levels; an attempt succeeds when every stat gained its required amount since then or is at
// its maximum; a failure names the first stat short of it; every attempt clears SwordCutscene and counts itself.
#include <cstdio>
#include <cstring>

struct Sword {
    bool StatsInitialised, SwordCutscene, HeroHasTakenSword; long Base[3], Level[3], Max[3], Need[3]; long SwordRemovalAttemptCount;
    const char* Line;
    void Pull()
    {
        if (!StatsInitialised) { for (int i = 0; i < 3; ++i) Base[i] = Level[i]; StatsInitialised = true; }
        bool okv[3];
        for (int i = 0; i < 3; ++i) okv[i] = Level[i] - Base[i] >= Need[i] || Level[i] == Max[i];
        Line = "";
        if (okv[0] && okv[1] && okv[2]) HeroHasTakenSword = true;
        else if (!okv[0]) Line = "PHYSIQUE";
        else if (!okv[1]) Line = "TOUGHNESS";
        else if (!okv[2]) Line = "HEALTH";
        SwordCutscene = false; SwordRemovalAttemptCount++;
    }
};

int main()
{
    Sword s; std::memset(&s, 0, sizeof s);
    for (int i = 0; i < 3; ++i) { s.Level[i] = 2; s.Max[i] = 10; s.Need[i] = 3; }
    s.SwordCutscene = true; s.Pull();
    bool ok = !s.HeroHasTakenSword && std::strcmp(s.Line, "PHYSIQUE") == 0 && s.SwordRemovalAttemptCount == 1 && !s.SwordCutscene;
    s.Level[0] = 5; s.Level[1] = 10; s.Pull();
    ok = ok && std::strcmp(s.Line, "HEALTH") == 0 && s.Base[0] == 2;
    s.Level[2] = 6; s.Pull();
    ok = ok && s.HeroHasTakenSword && s.SwordRemovalAttemptCount == 3;
    if (!ok) { std::printf("00ed5830_TEST FAIL\n"); return 1; }
    std::printf("00ed5830_TEST PASS\n");
    return 0;
}