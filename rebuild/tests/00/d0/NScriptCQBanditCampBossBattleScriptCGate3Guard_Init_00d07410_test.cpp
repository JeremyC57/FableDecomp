#include <cstdio>
#include <cstring>
// Behaviour model: the three interface calls happen in order with the entity's own thing.
static int step = 0; static char faction[64];
struct Game { void Info(bool a, bool b, bool c) { if (step == 0 && !a && b && !c) step = 1; }
              void Faction(const char* f) { if (step == 1) { std::strcpy(faction, f); step = 2; } }
              void Pushable(bool p) { if (step == 2 && !p) step = 3; } };
int main()
{
    Game g; g.Info(false, true, false); g.Faction("FACTION_TWINBLADE_CAMP_BANDITS"); g.Pushable(false);
    if (step != 3 || std::strcmp(faction, "FACTION_TWINBLADE_CAMP_BANDITS")) { std::printf("00d07410_TEST FAIL\n"); return 1; }
    std::printf("00d07410_TEST PASS\n");
    return 0;
}