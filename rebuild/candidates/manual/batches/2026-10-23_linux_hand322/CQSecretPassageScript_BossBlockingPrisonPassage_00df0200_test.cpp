#include <cstdio>
#include <cstring>
#include <vector>
// behaviour test: CQ_SecretPassageScript::BossBlockingPrisonPassage -- wait for the exterior, spawn the switcher once,
// wait for the hero at the entrance, spawn a boss per spawn point, block the doors until all bosses die, then unblock
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frame = 0;
struct Thing { int id; bool alive; };
struct Game {
    std::vector<Thing>* bosses;
    void NewScriptFrame() { ++g_frame; if (g_frame == 6) for (unsigned i = 0; i < bosses->size(); ++i) (*bosses)[i].alive = false; }
    bool IsLevelLoaded() const { return g_frame >= 1; }
    bool HeroNearEntrance() const { return g_frame >= 3; }
    void SetThingAsUsable(bool on) { L(on ? "usable" : "unusable"); }
    Thing CreateCreature(int pos) { L("create"); Thing t; t.id = 100 + pos; t.alive = true; return t; }
    void GiveThingBestEnemyTarget(const Thing&) { L("target"); }
};
static bool AllDead(const std::vector<Thing>& v) { for (unsigned i = 0; i < v.size(); ++i) if (v[i].alive) return false; return true; }
struct Script {
    Game* G; bool DoorsBlocked, DoorsBeingDone; int blockedSeen;
    bool IsActiveThreadTerminating() { return g_frame > 50; }
    void Run()
    {
        for (; !IsActiveThreadTerminating(); G->NewScriptFrame()) {
            while (!G->IsLevelLoaded()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (!DoorsBeingDone) { L("spawn"); DoorsBeingDone = true; }
            std::vector<Thing> points; for (int i = 0; i < 2; ++i) { Thing t; t.id = i; t.alive = false; points.push_back(t); }
            G->bosses = &points;
            G->SetThingAsUsable(false);
            while (!G->HeroNearEntrance() && G->IsLevelLoaded()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (G->IsLevelLoaded()) {
                DoorsBlocked = true;
                for (unsigned i = 0; i < points.size(); ++i) { Thing b = G->CreateCreature(points[i].id); G->GiveThingBestEnemyTarget(b); points[i] = b; }
                while (!AllDead(points)) { if (DoorsBlocked) ++blockedSeen; G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                DoorsBlocked = false;
                G->SetThingAsUsable(true);
                return;
            }
        }
    }
};
int main()
{
    Game g; Script s; s.G = &g; s.DoorsBlocked = false; s.DoorsBeingDone = false; s.blockedSeen = 0;
    s.Run();
    if (std::strcmp(g_log, "spawn;unusable;create;target;create;target;usable;") != 0 || s.DoorsBlocked || !s.DoorsBeingDone || s.blockedSeen == 0) {
        std::printf("00df0200_TEST FAIL\n"); return 1;
    }
    std::printf("00df0200_TEST PASS\n");
    return 0;
}
