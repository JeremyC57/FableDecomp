// Behaviour test for NScript::CV_KnotholeGladeGatesScript::ControlMainBalverines (0x00EA2080): one round of the main attack --
// pick a random villager outside (the hero if none is alive), wait until the KGG_Balv1 marker is off screen, spawn one balverine
// there (horn + persistent + best enemy target; the opinion source only for a villager target), then wait until it is dead.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct C3DVector { float x, y, z; };
struct CScriptThing { int id; bool alive; bool IsAlive() const { return alive; } C3DVector GetPos() const { C3DVector p = { (float)id, 0, 0 }; return p; } };
struct CGame {
    bool villagerAlive; int frames; int offScreenAfter; int deadAfter;
    CScriptThing GetThingWithScriptName(const char*) { CScriptThing t = { 1, true }; return t; }
    CScriptThing GetRandomThingWithScriptName(const char*) { CScriptThing t = { 2, villagerAlive }; return t; }
    CScriptThing GetHero() { CScriptThing t = { 3, true }; return t; }
    void NewScriptFrame() { ++frames; }
    bool IsCameraPosOnScreen(C3DVector p) { return p.x == 1.0f && frames < offScreenAfter; }
    CScriptThing CreateCreatureNearby(const char* def, C3DVector p, float r, const char* name, bool) {
        Log("create"); Log(def); Log(name); if (p.x != 1.0f || r != 6.0f) Log("badpos"); CScriptThing t = { 9, true }; return t; }
    void PlayCriteriaSoundOnThing(CScriptThing g, const char* s) { Log(g.id == 7 ? "horn" : "badgate"); Log(s); }
    void SetThingPersistent(CScriptThing, bool b) { Log(b ? "persist" : "nopersist"); }
    void GiveThingBestEnemyTarget(CScriptThing, CScriptThing t) { Log("target"); Log(t.id == 2 ? "villager" : t.id == 3 ? "hero" : "?"); }
    void EntitySetAsOpinionSource(CScriptThing, const char* s) { Log("opinion"); Log(s); }
};
struct CScript {
    CGame* Game; CScriptThing TheGate; int termAfter, termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
    bool AreAllThingsInVectorDead(const std::vector<CScriptThing>& v) { return v.empty() || Game->frames >= Game->deadAfter; }
    void ControlMainBalverines()
    {
        std::vector<CScriptThing> balverines;
        CScriptThing balv1 = Game->GetThingWithScriptName("KGG_Balv1");
        CScriptThing target;
        for (long i = 0; i < 1; ++i) {
            if (IsActiveThreadTerminating()) return;
            target = Game->GetRandomThingWithScriptName("KGG_VillagerOutside");
            if (target.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                while (Game->IsCameraPosOnScreen(balv1.GetPos())) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                CScriptThing balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", balv1.GetPos(), 6.0f, "KGG_Balverine", false);
                Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
                Game->SetThingPersistent(balv, true);
                Game->GiveThingBestEnemyTarget(balv, target);
                Game->EntitySetAsOpinionSource(balv, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
                balverines.push_back(balv);
            } else {
                if (IsActiveThreadTerminating()) return;
                target = Game->GetHero();
                while (Game->IsCameraPosOnScreen(balv1.GetPos())) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                CScriptThing balv = Game->CreateCreatureNearby("CREATURE_BALVERINE_01", balv1.GetPos(), 6.0f, "KGG_Balverine", false);
                Game->PlayCriteriaSoundOnThing(TheGate, "KNG_HORN");
                Game->SetThingPersistent(balv, true);
                Game->GiveThingBestEnemyTarget(balv, target);
                balverines.push_back(balv);
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (!AreAllThingsInVectorDead(balverines)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("done");
    }
};
static bool Run(bool villagerAlive, int termAfter, const char* expected, int minFrames)
{
    CGame g; g.villagerAlive = villagerAlive; g.frames = 0; g.offScreenAfter = 3; g.deadAfter = 8;
    CScript s; s.Game = &g; s.TheGate.id = 7; s.TheGate.alive = true; s.termAfter = termAfter; s.termCalls = 0;
    g_log[0] = 0;
    s.ControlMainBalverines();
    if (std::strcmp(g_log, expected) != 0) { std::printf("log: %s\n", g_log); return false; }
    return g.frames >= minFrames;
}
int main()
{
    bool ok = true;
    ok = ok && Run(true, 1000, "create;CREATURE_BALVERINE_01;KGG_Balverine;horn;KNG_HORN;persist;target;villager;opinion;OPINION_SOURCE_INANIMATE_EVIL_HIGH;done;", 8);
    ok = ok && Run(false, 1000, "create;CREATURE_BALVERINE_01;KGG_Balverine;horn;KNG_HORN;persist;target;hero;done;", 8);
    ok = ok && Run(true, 2, "", 0);   // terminated while the camera still shows the spawn marker: nothing spawned
    if (!ok) { std::printf("00ea2080_TEST FAIL\n"); return 1; }
    std::printf("00ea2080_TEST PASS\n");
    return 0;
}
