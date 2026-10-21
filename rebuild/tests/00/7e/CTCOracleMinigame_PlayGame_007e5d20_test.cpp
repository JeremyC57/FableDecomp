// Behaviour test for CTCOracleMinigame::PlayGame (0x007E5D20): level and stage become 1, the output interval comes from the
// def, the five named things are looked up, the firing flags clear, and the player enters mode 0x29 only if not already in it.
#include <cstdio>
#include <cstring>

struct Thing { const char* name; };
static Thing g_things[5] = { { "OBJECT_SS_ORACLE_SYMBOL_YELLOW_01" }, { "OBJECT_SS_ORACLE_SYMBOL_RED_01" },
                             { "OBJECT_SS_ORACLE_SYMBOL_GREEN_01" }, { "OBJECT_SS_ORACLE_SYMBOL_BLUE_01" }, { "OracleGamesmaster" } };
static bool Find(const char* name, Thing** out)
{
    for (int i = 0; i < 5; ++i)
        if (std::strcmp(g_things[i].name, name) == 0) { *out = &g_things[i]; return true; }
    return false;
}
struct Player { long mode; int adds; };
struct Oracle { long level, stage; float interval, defInterval; Thing* symbols[4]; long firing[4]; Thing* speech; unsigned long text; };
static void PlayGame(Oracle& o, Player& p)
{
    o.level = 1; o.stage = 1; o.interval = o.defInterval;
    Find("OBJECT_SS_ORACLE_SYMBOL_YELLOW_01", &o.symbols[0]);
    Find("OBJECT_SS_ORACLE_SYMBOL_RED_01", &o.symbols[1]);
    Find("OBJECT_SS_ORACLE_SYMBOL_GREEN_01", &o.symbols[2]);
    Find("OBJECT_SS_ORACLE_SYMBOL_BLUE_01", &o.symbols[3]);
    Find("OracleGamesmaster", &o.speech);
    for (int i = 0; i < 4; ++i) o.firing[i] = 0;
    if (p.mode != 0x29) { p.mode = 0x29; ++p.adds; }
    o.text = 0;
}
int main()
{
    Oracle o; std::memset(&o, 0, sizeof(o)); o.defInterval = 0.5f; o.firing[2] = 7; o.text = 3;
    Player p; p.mode = 0; p.adds = 0;
    PlayGame(o, p);
    if (o.level != 1 || o.stage != 1 || o.interval != 0.5f || o.symbols[3] != &g_things[3] || o.speech != &g_things[4] ||
        o.firing[2] != 0 || o.text != 0 || p.mode != 0x29 || p.adds != 1) { std::printf("007e5d20_TEST FAIL\n"); return 1; }
    PlayGame(o, p);
    if (p.adds != 1) { std::printf("007e5d20_TEST FAIL\n"); return 1; }
    std::printf("007e5d20_TEST PASS\n");
    return 0;
}