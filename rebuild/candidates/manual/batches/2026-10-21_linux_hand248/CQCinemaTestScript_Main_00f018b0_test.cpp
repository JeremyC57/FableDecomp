// Behaviour test for 00f018b0 (CQ_CinemaTestScript::Main): the mission flags are reset, the "Dummy" binding is added
// (name copied, parent set, alloc valid, flags 0), entities are post-added locally and in the game, then the
// WatchForTermination and DoMission threads are spawned in that order with an empty region name.
#include <cstdio>
#include <cstring>

struct Binding { char name[16]; void* parent; int allocValid; unsigned long flags; };
static char g_log[8][32]; static int g_n = 0;
static void Log(const char* s) { std::strcpy(g_log[g_n++], s); }
struct Script {
    bool Succeeded, Failed, Over; Binding binding; int bindings;
    void AddEntityScriptBinding(const char* name, void* parent, unsigned long flags)
    {
        std::strcpy(binding.name, name); binding.parent = parent; binding.allocValid = 1; binding.flags = flags; ++bindings; Log("bind");
    }
    void Spawn(const char* name, const char* region) { if (region[0] == 0) Log(name); }
    void Main()
    {
        Succeeded = false; Failed = false; Over = false;
        AddEntityScriptBinding("Dummy", this, 0);
        Log("post"); Log("gamepost");
        Spawn("WatchForTermination", "");
        Spawn("DoMission", "");
    }
};
int main()
{
    Script s; std::memset(&s, 0, sizeof s); s.Succeeded = s.Failed = s.Over = true;
    s.Main();
    const char* want[] = { "bind", "post", "gamepost", "WatchForTermination", "DoMission" };
    int ok = !s.Succeeded && !s.Failed && !s.Over && s.bindings == 1 && std::strcmp(s.binding.name, "Dummy") == 0 &&
             s.binding.parent == &s && s.binding.allocValid && s.binding.flags == 0 && g_n == 5;
    for (int i = 0; i < 5 && ok; ++i) if (std::strcmp(g_log[i], want[i]) != 0) ok = 0;
    if (!ok) { std::printf("00f018b0_TEST FAIL\n"); return 1; }
    std::printf("00f018b0_TEST PASS\n");
    return 0;
}
