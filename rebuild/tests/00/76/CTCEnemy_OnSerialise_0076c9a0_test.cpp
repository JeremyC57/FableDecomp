#include <cstdio>
#include <string>
// Behaviour model of CTCEnemy::OnSerialise's faction-name logic (0x0076C9A0).
struct Ctx { int mode; std::string stored; bool has; };
struct Faction { std::string name; };
static Faction gFactions[2] = { {"GOOD"}, {"EVIL"} };
static Faction* lookup(const std::string& n) { for (int i = 0; i < 2; ++i) if (gFactions[i].name == n) return &gFactions[i]; return 0; }
struct Enemy {
    Faction* pfaction; bool hasEnemyDef; std::string defFaction;
    void Serialise(Ctx& c) {
        if (c.mode == 1 || c.mode == 3) {
            std::string name;
            bool differs = pfaction && hasEnemyDef && defFaction != pfaction->name;
            if (differs) name = pfaction ? pfaction->name : "";
            c.stored = name; c.has = true;
        } else {
            std::string name = c.has ? c.stored : std::string();
            if (name != "") pfaction = lookup(name);
        }
    }
};
int main()
{
    Ctx c = {1, "", false};
    Enemy e = {&gFactions[1], true, "GOOD"};
    e.Serialise(c);
    if (c.stored != "EVIL") { std::printf("0076c9a0_TEST FAIL\n"); return 1; }
    Enemy same = {&gFactions[0], true, "GOOD"};
    Ctx c2 = {3, "x", false}; same.Serialise(c2);
    if (c2.stored != "") { std::printf("0076c9a0_TEST FAIL\n"); return 1; }
    Enemy none = {0, true, "GOOD"}; Ctx c3 = {1, "x", false}; none.Serialise(c3);
    if (c3.stored != "") { std::printf("0076c9a0_TEST FAIL\n"); return 1; }
    Enemy l = {0, false, ""}; c.mode = 0; l.Serialise(c);
    if (l.pfaction != &gFactions[1]) { std::printf("0076c9a0_TEST FAIL\n"); return 1; }
    Enemy l2 = {&gFactions[0], false, ""}; c2.mode = 2; l2.Serialise(c2);
    if (l2.pfaction != &gFactions[0]) { std::printf("0076c9a0_TEST FAIL\n"); return 1; }
    std::printf("0076c9a0_TEST PASS\n");
    return 0;
}