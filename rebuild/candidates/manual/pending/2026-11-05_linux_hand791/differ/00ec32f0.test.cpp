// Behaviour test for NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs (0x00EC32F0): the def list per rough
// experience level (0..3 level 1, 4..7 level 2, 8 and above level 3; 2-5 entries: grunts, archers, a leader at 3 / 7 / default),
// entry BanditCreatureGeneratorIndex % size (unsigned), the index advanced on every call.
#include <cstdio>
#include <string>
#include <vector>
typedef std::string Str;
static Str Grunt(long level) { char b[48]; std::sprintf(b, "CREATURE_BANDIT_GRUNT_LEVEL%ld_A", level); return b; }
struct Sim {
    unsigned int BanditCreatureGeneratorIndex; long Level;   // retail unsigned long is 32-bit
    Sim() : BanditCreatureGeneratorIndex(0), Level(0) {}
    Str Choose()
    {
        Str bandit;
        std::vector<Str> defs;
        switch (Level) {
        default:
            defs.push_back(Grunt(3)); defs.push_back(Grunt(3));
            defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3"); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3");
            defs.push_back("CREATURE_BANDIT_LEADER_LEVEL3");
            break;
        case 0: defs.push_back(Grunt(1)); defs.push_back(Grunt(1)); break;
        case 1: defs.push_back(Grunt(1)); defs.push_back(Grunt(1)); defs.push_back(Grunt(1)); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1"); break;
        case 2: defs.push_back(Grunt(1)); defs.push_back(Grunt(1)); defs.push_back(Grunt(1));
                defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1"); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1"); break;
        case 3: defs.push_back(Grunt(1)); defs.push_back(Grunt(1)); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1");
                defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL1"); defs.push_back("CREATURE_BANDIT_LEADER_LEVEL1"); break;
        case 4: defs.push_back(Grunt(2)); defs.push_back(Grunt(2)); break;
        case 5: defs.push_back(Grunt(2)); defs.push_back(Grunt(2)); defs.push_back(Grunt(2)); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2"); break;
        case 6: defs.push_back(Grunt(2)); defs.push_back(Grunt(2)); defs.push_back(Grunt(2));
                defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2"); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2"); break;
        case 7: defs.push_back(Grunt(2)); defs.push_back(Grunt(2)); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2");
                defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL2"); defs.push_back("CREATURE_BANDIT_LEADER_LEVEL2"); break;
        case 8: defs.push_back(Grunt(3)); defs.push_back(Grunt(3)); defs.push_back(Grunt(3)); defs.push_back("CREATURE_BANDIT_ARCHER_LEVEL3"); break;
        }
        bandit = defs[BanditCreatureGeneratorIndex % defs.size()];
        ++BanditCreatureGeneratorIndex;
        return bandit;
    }
};
int main()
{
    int fails = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d\n", __LINE__); ++fails; } } while (0)
    Sim s;
    s.Level = 3;
    CHECK(s.Choose() == "CREATURE_BANDIT_GRUNT_LEVEL1_A");
    CHECK(s.Choose() == "CREATURE_BANDIT_GRUNT_LEVEL1_A");
    CHECK(s.Choose() == "CREATURE_BANDIT_ARCHER_LEVEL1");
    CHECK(s.Choose() == "CREATURE_BANDIT_ARCHER_LEVEL1");
    CHECK(s.Choose() == "CREATURE_BANDIT_LEADER_LEVEL1");
    CHECK(s.Choose() == "CREATURE_BANDIT_GRUNT_LEVEL1_A");          // 5 % 5 wraps
    CHECK(s.BanditCreatureGeneratorIndex == 6);
    s.Level = 7; s.BanditCreatureGeneratorIndex = 4; CHECK(s.Choose() == "CREATURE_BANDIT_LEADER_LEVEL2");
    s.Level = 8; s.BanditCreatureGeneratorIndex = 3; CHECK(s.Choose() == "CREATURE_BANDIT_ARCHER_LEVEL3");
    s.Level = 42; s.BanditCreatureGeneratorIndex = 9; CHECK(s.Choose() == "CREATURE_BANDIT_LEADER_LEVEL3");   // default arm, 9 % 5
    s.Level = 0; s.BanditCreatureGeneratorIndex = 0xFFFFFFFFu; CHECK(s.Choose() == "CREATURE_BANDIT_GRUNT_LEVEL1_A");
    CHECK(s.BanditCreatureGeneratorIndex == 0);                    // unsigned wrap
    if (fails == 0) std::printf("00ec32f0_TEST PASS\n");
    return fails;
}
