#include <cstdio>
// Behaviour test for 00dfa020: CQ_TraderConflictEvilScript::Alloc builds the script with its interface/data pair, an empty AllCreatures vector and a fresh ScreamOutTimer.
struct Interface { long nextTimer; long RegisterTimer() { return nextTimer++; } };
static Interface g_interface;
struct CTimer { long Index; CTimer() : Index(g_interface.RegisterTimer()) {} };
struct Vec { void* start; void* finish; void* end; Vec() : start(0), finish(0), end(0) {} };
struct Base { Interface* Game; void* MasterScriptData; Base(void* data, Interface& game) : Game(&game), MasterScriptData(data) {} };
struct Script : Base {
    Vec AllCreatures; long CounterID, NumberSpawned, InitialNumberInRegion; bool QuestStartScreened;
    long NextTimeToSpawnGuards; bool MissionSucceeded, HeroAttackedBandit, PlayerEngaged;
    CTimer ScreamOutTimer;
    Script(void* data, Interface& game) : Base(data, game) {}
};
static Script* Alloc(void* data, Interface& game) { return new Script(data, game); }
int main()
{
    g_interface.nextTimer = 3;
    int token = 0;
    Script* s = Alloc(&token, g_interface);
    Script* t = Alloc(0, g_interface);
    int ok = s && t && s->Game == &g_interface && s->MasterScriptData == &token && t->MasterScriptData == 0
          && s->AllCreatures.start == 0 && s->AllCreatures.finish == 0 && s->AllCreatures.end == 0
          && s->ScreamOutTimer.Index == 3 && t->ScreamOutTimer.Index == 4;
    delete s; delete t;
    if (!ok) { std::printf("00dfa020_TEST FAIL\n"); return 1; }
    std::printf("00dfa020_TEST PASS\n");
    return 0;
}