// Behaviour test for 0x00502170 (CWorldMap::CreateWorldLevelWad): with both WAD switches on, every level after index 0
// and its thing file are added to the bank between Begin/EndUpdateEntries; otherwise nothing is touched.
#include <cstdio>
#include <string>
#include <vector>
static bool g_enabled = true, g_levels = true;
static std::vector<std::string> g_bank;
static int g_begin = 0, g_end = 0, g_close = 0;
static std::string ThingFile(const std::string& level) { return level.substr(0, level.size() - 4) + ".tng"; }
static void CreateWorldLevelWad(const std::vector<std::string>& worldLevels)
{
    if (!g_enabled || !g_levels)
        return;
    std::vector<std::string> levelFiles(worldLevels);
    ++g_begin;
    unsigned long count = (unsigned long)levelFiles.size();
    for (unsigned long i = 1; i < count; ++i) {
        std::string levelFile(levelFiles[i]);
        g_bank.push_back(levelFile);
        g_bank.push_back(ThingFile(levelFiles[i]));
    }
    ++g_end;
    ++g_close;
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    std::vector<std::string> world;
    world.push_back("none.lev"); world.push_back("a.lev"); world.push_back("b.lev");
    g_levels = false; CreateWorldLevelWad(world);
    CHECK(g_bank.empty() && g_begin == 0);
    g_levels = true; CreateWorldLevelWad(world);
    CHECK(g_bank.size() == 4 && g_bank[0] == "a.lev" && g_bank[1] == "a.tng" && g_bank[3] == "b.tng");
    CHECK(g_begin == 1 && g_end == 1 && g_close == 1);
    std::vector<std::string> one(1, "only.lev");
    CreateWorldLevelWad(one);
    CHECK(g_bank.size() == 4 && g_begin == 2);
    if (g_fail) { std::printf("00502170_TEST FAIL\n"); return 1; }
    std::printf("00502170_TEST PASS\n");
    return 0;
}