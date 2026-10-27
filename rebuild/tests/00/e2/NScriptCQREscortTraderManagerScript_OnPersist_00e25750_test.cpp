#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& def) { char b[96]; std::sprintf(b, "%s=%d(def %d)", name, (int)value, (int)def); Log(b); }
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, T()); }
};
enum EscortTraderManagerPhase { PHASE0, PHASE1, PHASE2 };
struct S { int OrchardFarmTraderEscortCounter; int DebugCounter; bool Activated; EscortTraderManagerPhase OrchardFarmTraderEscortPhase;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("OrchardFarmTraderEscortCounter", OrchardFarmTraderEscortCounter, 0);
        context.Transfer("OrchardFarmTraderEscortPhase", OrchardFarmTraderEscortPhase, (EscortTraderManagerPhase)0);
        context.Transfer("Activated", Activated, false);
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    S s; s.OrchardFarmTraderEscortCounter = 3; s.Activated = true; s.OrchardFarmTraderEscortPhase = PHASE2; CPersistContext c; s.OnPersist(c);
    ok = ok && std::strcmp(g_log, "OrchardFarmTraderEscortCounter=3(def 0);OrchardFarmTraderEscortPhase=2(def 0);Activated=1(def 0);") == 0 && (true);
    if (!ok) std::printf("log: %s\n", g_log);
    if (!ok) { std::printf("00e25750_TEST FAIL\n"); return 1; }
    std::printf("00e25750_TEST PASS\n");
    return 0;
}