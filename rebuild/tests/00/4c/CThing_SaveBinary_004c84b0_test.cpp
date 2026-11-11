// Behaviour test for CThing::SaveBinary (0x004C84B0): the order and content of what a thing writes to a binary save.
#include <stdio.h>
#include <string>
#include <vector>
static std::string g_log;                       // one token per write
static void W(const std::string& s) { g_log += s; g_log += "|"; }
static std::string Num(long v) { char b[32]; sprintf(b, "%ld", v); return b; }
struct TC {
    std::string name; bool creatable, heroState, toBeRemoved; std::string block;
    TC(const char* n, bool c, bool h, bool r, const char* b) : name(n), creatable(c), heroState(h), toBeRemoved(r), block(b) { }
};
struct Thing {
    long uid; short map; signed char player; unsigned short defIndex; bool levelLess, hasDef, isCreature, gameStatePersisted, coopSpirit;
    bool addingTCsPermitted; int levelPersistentCalls;
    std::string scriptName, scriptData, state; std::vector<TC> tcs;
    bool IsLevelLess() const { bool r = false; if (hasDef && levelLess) r = true; return r; }
    bool IsGameStatePersisted() const { if (coopSpirit) return false; return gameStatePersisted; }
    bool IsLevelPersistent() { ++levelPersistentCalls; return true; }
    void SaveBinary(bool serialisingHeroState);
};
static long Remap(bool saving, bool levelLess, long level, long uid) { return uid + (levelLess ? 0 : level * 1000) + (saving ? 1 : 0); }
void Thing::SaveBinary(bool serialisingHeroState)
{
    long id = Remap(true, IsLevelLess(), map, uid);
    std::string def = "";
    if (defIndex > 0) def = "DEF" + Num(defIndex);
    W("player=" + Num(player)); W("uid=" + Num(id)); W("def=" + def);
    W("len=" + Num((long)state.size())); W(state);
    bool hero_state_only = false;
    if (isCreature && serialisingHeroState) hero_state_only = true;
    else W("map=" + Num(map));
    addingTCsPermitted = false;
    for (size_t i = 0; i < tcs.size(); ++i) {
        TC& tc = tcs[i];
        if (hero_state_only && !tc.heroState) continue;
        if (tc.toBeRemoved) continue;
        W(tc.name); W(tc.creatable ? "1" : "0"); W("len=" + Num((long)tc.block.size())); W(tc.block);
    }
    W("END");
    addingTCsPermitted = true;
    W(scriptName); W(scriptData);
    bool persistent = IsGameStatePersisted();
    IsLevelPersistent();
    W(persistent ? "1" : "0"); W(persistent ? "1" : "0");
}
int main(void)
{
    int fails = 0;
    Thing t; t.uid = 40; t.map = 3; t.player = -1; t.defIndex = 7; t.levelLess = false; t.hasDef = true; t.isCreature = true;
    t.gameStatePersisted = true; t.coopSpirit = false; t.addingTCsPermitted = true; t.levelPersistentCalls = 0;
    t.scriptName = "SN"; t.scriptData = "SD"; t.state = "abc";
    t.tcs.push_back(TC("CTCA", true, true, false, "aa")); t.tcs.push_back(TC("CTCB", false, false, false, "b"));
    t.tcs.push_back(TC("CTCC", true, true, true, "cc"));
    t.SaveBinary(false);
    if (g_log != "player=-1|uid=3041|def=DEF7|len=3|abc|map=3|CTCA|1|len=2|aa|CTCB|0|len=1|b|END|SN|SD|1|1|") { printf("FAIL 1: %s\n", g_log.c_str()); ++fails; }
    if (!t.addingTCsPermitted || t.levelPersistentCalls != 1) { printf("FAIL 2\n"); ++fails; }
    g_log = ""; t.levelLess = true; t.defIndex = 0; t.coopSpirit = true;
    t.SaveBinary(true);                         // hero state: no map number, only the hero-state components
    if (g_log != "player=-1|uid=41|def=|len=3|abc|CTCA|1|len=2|aa|END|SN|SD|0|0|") { printf("FAIL 3: %s\n", g_log.c_str()); ++fails; }
    g_log = ""; t.isCreature = false; t.hasDef = false; t.coopSpirit = false; t.gameStatePersisted = false;
    t.SaveBinary(true);                         // not a creature: the full form even while the hero state is being written
    if (g_log != "player=-1|uid=3041|def=|len=3|abc|map=3|CTCA|1|len=2|aa|CTCB|0|len=1|b|END|SN|SD|0|0|") { printf("FAIL 4: %s\n", g_log.c_str()); ++fails; }
    if (fails) { printf("004c84b0_TEST FAIL\n"); return 1; }
    printf("004c84b0_TEST PASS\n");
    return 0;
}