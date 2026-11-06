// Behaviour test for CThingAICreature::Save (0x008304A0): after the base text, one "Key value;\r\n" line per set relation UID (home /
// work building, father, mother, spouse; remapped), the overriding brain name or NULL, the ten flags as TRUE/FALSE and the initial
// position with 6 digits, in retail order.
#include <cstdio>
#include <cstring>
struct Creature {
    unsigned long uid[5]; bool set[5]; const char* brain; bool flags[10]; float pos[3];
    static unsigned long Remap(unsigned long v) { return v + 1000; }
    void Save(char* out) const
    {
        char line[128];
        std::strcat(out, "BASE;\r\n");
        static const char* keys[5] = { "HomeBuildingUID ", "WorkBuildingUID ", "FatherCreatureUID ", "MotherCreatureUID ", "SpouseCreatureUID " };
        for (int i = 0; i < 5; ++i)
            if (set[i]) { std::sprintf(line, "%s%lu;\r\n", keys[i], Remap(uid[i])); std::strcat(out, line); }
        std::sprintf(line, "OverridingBrainName %s;\r\n", brain[0] == 0 ? "NULL" : brain); std::strcat(out, line);
        static const char* names[10] = { "HasInformation ", "WanderWithInformation ", "WaveWithInformation ", "ContinueAIWithInformation ",
            "EnableCreatureAutoPlacing ", "AllowedToFollowHero ", "RegionFollowingOverriddenFromScript ", "RespondingToFollowAndWait ",
            "CanBeCourted ", "CanBeMarried " };
        for (int i = 0; i < 10; ++i) { std::sprintf(line, "%s%s;\r\n", names[i], flags[i] ? "TRUE" : "FALSE"); std::strcat(out, line); }
        std::sprintf(line, "InitialPosX %.6f;\r\n", (double)pos[0]); std::strcat(out, line);
        std::sprintf(line, "InitialPosY %.6f;\r\n", (double)pos[1]); std::strcat(out, line);
        std::sprintf(line, "InitialPosZ %.6f;\r\n", (double)pos[2]); std::strcat(out, line);
    }
};
int main()
{
    Creature c; std::memset(c.set, 0, sizeof(c.set)); std::memset(c.flags, 0, sizeof(c.flags)); c.brain = "";
    c.set[1] = true; c.uid[1] = 5; c.set[4] = true; c.uid[4] = 7; c.flags[0] = true; c.flags[9] = true;
    c.pos[0] = 1.5f; c.pos[1] = -2.0f; c.pos[2] = 0.25f;
    static char out[2048]; out[0] = 0; c.Save(out);
    const char* want = "BASE;\r\nWorkBuildingUID 1005;\r\nSpouseCreatureUID 1007;\r\nOverridingBrainName NULL;\r\nHasInformation TRUE;\r\n"
        "WanderWithInformation FALSE;\r\nWaveWithInformation FALSE;\r\nContinueAIWithInformation FALSE;\r\nEnableCreatureAutoPlacing FALSE;\r\n"
        "AllowedToFollowHero FALSE;\r\nRegionFollowingOverriddenFromScript FALSE;\r\nRespondingToFollowAndWait FALSE;\r\nCanBeCourted FALSE;\r\n"
        "CanBeMarried TRUE;\r\nInitialPosX 1.500000;\r\nInitialPosY -2.000000;\r\nInitialPosZ 0.250000;\r\n";
    bool ok = std::strcmp(out, want) == 0;
    Creature d = c; d.brain = "BRAIN_VILLAGER"; static char o2[2048]; o2[0] = 0; d.Save(o2);
    ok = ok && std::strstr(o2, "OverridingBrainName BRAIN_VILLAGER;\r\n") != 0 && std::strstr(o2, "NULL") == 0;
    if (!ok) { std::printf("008304a0_TEST FAIL\n"); return 1; }
    std::printf("008304a0_TEST PASS\n");
    return 0;
}