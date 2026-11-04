// Behaviour test for CTCEnemy::CTCEnemy(CThing*) (0x0076C8A0), g++ model.
#include <cstdio>
#include <string>
#include <map>
struct Def { long refs; bool hasEnemy; long faction; };
static std::map<long, std::string> g_names;
struct Enemy { bool friendsWithEverything, followersProxy; std::string faction; bool hasFaction; };
static Enemy Construct(Def& thingDef, Def& enemyDef)
{
    Enemy e; e.friendsWithEverything = false; e.followersProxy = true; e.hasFaction = false;
    ++thingDef.refs;
    if (thingDef.hasEnemy) {
        ++enemyDef.refs;
        e.faction = g_names[enemyDef.faction]; e.hasFaction = true;
        --enemyDef.refs;
    }
    --thingDef.refs;
    return e;
}
int main()
{
    g_names[7] = "FACTION_BANDITS";
    Def t = { 1, true, 0 }, en = { 1, false, 7 };
    Enemy e = Construct(t, en);
    int ok = !e.friendsWithEverything && e.followersProxy && e.hasFaction && e.faction == "FACTION_BANDITS" && t.refs == 1 && en.refs == 1;
    Def t2 = { 1, false, 0 };
    Enemy f = Construct(t2, en);
    ok &= !f.hasFaction && t2.refs == 1;
    std::printf(ok ? "0076c8a0_TEST PASS\n" : "0076c8a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
