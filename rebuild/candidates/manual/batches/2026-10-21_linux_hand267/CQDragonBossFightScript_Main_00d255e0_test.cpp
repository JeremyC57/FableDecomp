// Behaviour test for 00d255e0 (CQ_DragonBossFightScript::Main): three entity-script bindings in order, both
// PostAddScriptedEntities calls, then the DoMission spawn.
#include <cstdio>
#include <cstring>

static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Binding { const char* name; void* parent; int alloc; bool valid; unsigned long flags; };
struct Script {
    Binding bindings[4]; int nb;
    void AddEntityScriptBinding(const Binding& b) { bindings[nb++] = b; Log(b.name); }
    void PostAddScriptedEntities() { Log("post"); }
    void GamePostAddScriptedEntities() { Log("gpost"); }
    void SpawnFunction(const char* n) { Log(n); }
    void Main()
    {
        Binding a = { "Dragon", this, 1, true, 0 }; AddEntityScriptBinding(a);
        Binding b = { "DBMinion", this, 2, true, 0 }; AddEntityScriptBinding(b);
        Binding c = { "DBSummoner", this, 3, true, 0 }; AddEntityScriptBinding(c);
        PostAddScriptedEntities();
        GamePostAddScriptedEntities();
        SpawnFunction("DoMission");
    }
};
int main()
{
    Script s; s.nb = 0; g_log[0] = 0;
    s.Main();
    int ok = std::strcmp(g_log, "Dragon;DBMinion;DBSummoner;post;gpost;DoMission;") == 0 && s.nb == 3 &&
             s.bindings[2].alloc == 3 && s.bindings[0].parent == &s && s.bindings[1].valid && s.bindings[1].flags == 0;
    if (!ok) { std::printf("00d255e0_TEST FAIL\n"); return 1; }
    std::printf("00d255e0_TEST PASS\n");
    return 0;
}
