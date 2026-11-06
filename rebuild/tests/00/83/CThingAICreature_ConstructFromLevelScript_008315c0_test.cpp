// Behaviour test for CThingAICreature::ConstructFromLevelScript (0x008315C0): after a successful base construct the health comes from
// the def, Drawable is set, the relation UIDs start at zero, then each key of the level-script text sets its field (UIDs, the brain
// name with NULL meaning "", the ten bool flags with RespondingToFollowAndWait defaulting to TRUE when the value is unreadable, the
// initial position components) until EndThing; an InitialPos never given falls back to the current position. A failed base construct
// returns false and touches nothing.
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Creature {
    bool baseOk; float defHealth; float maxHealth, health; bool drawable;
    unsigned long uid[5]; char brain[32]; bool flags[10]; float initialPos[3]; float pos[3];
    // flags: 0 EnableCreatureAutoPlacing 1 HasInformation 2 WanderWithInformation 3 WaveWithInformation 4 ContinueAIWithInformation
    //        5 AllowedToFollowHero 6 RegionFollowingOverriddenFromScript 7 RespondingToFollowAndWait 8 CanBeCourted 9 CanBeMarried
    static bool ReadBool(const char* tok, bool def)
    {
        if (!std::strcmp(tok, "TRUE") || !std::strcmp(tok, "BTRUE")) return true;
        if (!std::strcmp(tok, "FALSE") || !std::strcmp(tok, "BFALSE")) return false;
        return def;
    }
    bool Construct(const char* script)
    {
        if (!baseOk) return false;
        maxHealth = health = defHealth;
        drawable = true;
        for (int i = 0; i < 5; ++i) uid[i] = 0;
        static const char* uidKeys[5] = { "HomeBuildingUID", "WorkBuildingUID", "FatherCreatureUID", "MotherCreatureUID", "SpouseCreatureUID" };
        static const char* flagKeys[10] = { "EnableCreatureAutoPlacing", "HasInformation", "WanderWithInformation", "WaveWithInformation",
            "ContinueAIWithInformation", "AllowedToFollowHero", "RegionFollowingOverriddenFromScript", "RespondingToFollowAndWait",
            "CanBeCourted", "CanBeMarried" };
        static const char* posKeys[3] = { "InitialPosX", "InitialPosY", "InitialPosZ" };
        bool initialPosSet = false;
        char buf[512]; std::strcpy(buf, script);
        char* tok = std::strtok(buf, " ;\n");
        while (tok) {
            char key[64]; std::strcpy(key, tok);
            if (!std::strcmp(key, "EndThing")) break;
            char* val = std::strtok(0, " ;\n");
            if (!val) val = (char*)"";
            int i;
            for (i = 0; i < 5; ++i) if (!std::strcmp(key, uidKeys[i])) uid[i] = std::strtoul(val, 0, 10);
            if (!std::strcmp(key, "OverridingBrainName")) std::strcpy(brain, std::strcmp(val, "NULL") ? val : "");
            for (i = 0; i < 10; ++i) if (!std::strcmp(key, flagKeys[i])) flags[i] = ReadBool(val, i == 7);
            for (i = 0; i < 3; ++i) if (!std::strcmp(key, posKeys[i])) { initialPos[i] = (float)std::atof(val); initialPosSet = true; }
            tok = std::strtok(0, " ;\n");
        }
        if (!initialPosSet)
            for (int i = 0; i < 3; ++i) initialPos[i] = pos[i];
        return true;
    }
};

static void Init(Creature& c)
{
    std::memset(&c, 0, sizeof(c));
    c.baseOk = true; c.defHealth = 75.0f; c.pos[0] = 1.0f; c.pos[1] = 2.0f; c.pos[2] = 3.0f;
    for (int i = 0; i < 5; ++i) c.uid[i] = 99;
    std::strcpy(c.brain, "OLD");
}

int main()
{
    bool ok = true;
    Creature a; Init(a);
    ok = ok && a.Construct("HomeBuildingUID 12; OverridingBrainName NULL; HasInformation TRUE; RespondingToFollowAndWait XYZ; "
                           "CanBeMarried BTRUE; InitialPosY 5.5; EndThing; CanBeCourted TRUE;");
    ok = ok && a.maxHealth == 75.0f && a.health == 75.0f && a.drawable;
    ok = ok && a.uid[0] == 12 && a.uid[1] == 0 && a.uid[4] == 0 && a.brain[0] == 0;
    ok = ok && a.flags[1] && a.flags[7] && a.flags[9] && !a.flags[8];
    ok = ok && a.initialPos[1] > 5.49f && a.initialPos[1] < 5.51f && a.initialPos[0] == 0.0f;
    Creature b; Init(b);
    ok = ok && b.Construct("OverridingBrainName BRAIN_GUARD; EndThing;");
    ok = ok && !std::strcmp(b.brain, "BRAIN_GUARD") && b.initialPos[0] == 1.0f && b.initialPos[2] == 3.0f;
    Creature c; Init(c); c.baseOk = false;
    ok = ok && !c.Construct("HomeBuildingUID 1;") && c.uid[0] == 99 && !c.drawable;
    if (!ok) { std::printf("008315c0_TEST FAIL\n"); return 1; }
    std::printf("008315c0_TEST PASS\n");
    return 0;
}