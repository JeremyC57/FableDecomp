// Behaviour test for 0x00dcf4c0 (CVtable_012DB4B8::Main (OGSP entity script)): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct GSI { int updates; int removed; GSI() : updates(0), removed(0) {} void Update() { ++updates; } void RemoveThing(bool a, bool b) { if (a && b) ++removed; } };
struct Parent { bool UndeadCutsceneStarted; };
struct Script { GSI* Game; Parent* ParentClass; int doneAfter; int filters; int isDoneCalls;
    void RegisterFilter() { ++filters; } bool IsDone() { ++isDoneCalls; return isDoneCalls > doneAfter; }
    void Main() { RegisterFilter(); Game->Update(); if (!IsDone() && ParentClass->UndeadCutsceneStarted && !IsDone()) Game->RemoveThing(true, true); } };

int main()
{
    GSI g; Parent p; p.UndeadCutsceneStarted = true; Script s; s.Game = &g; s.ParentClass = &p; s.doneAfter = 5; s.filters = 0; s.isDoneCalls = 0;
    s.Main(); CHECK(g.updates == 1 && g.removed == 1 && s.filters == 1 && s.isDoneCalls == 2);
    p.UndeadCutsceneStarted = false; s.isDoneCalls = 0; s.Main(); CHECK(g.removed == 1 && s.isDoneCalls == 1);
    p.UndeadCutsceneStarted = true; s.isDoneCalls = 0; s.doneAfter = 0; s.Main(); CHECK(g.removed == 1 && s.isDoneCalls == 1);
    if (g_fail) { std::printf("00dcf4c0_TEST FAIL\n"); return 1; }
    std::printf("00dcf4c0_TEST PASS\n");
    return 0;
}