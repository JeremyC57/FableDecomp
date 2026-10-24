// Behaviour test for 0x006ED3F0 (CCombatManager::CCombatManager(CWorld&): empty maps, defaults, then RegisterCombatSequences).
#include <cstdio>
#include <map>
#include <string>

struct CWorld { int dummy; };
struct Info { int x; };
static int g_registerCalls = 0;
static int g_dialogueCalls = 0;
struct CombatDialogueManager { CombatDialogueManager() { ++g_dialogueCalls; } };
struct Base { virtual ~Base() { } };
struct Manager : Base {
    std::map<std::string, int> RegisteredCombatSequences;
    std::map<unsigned long long, Info> Combatants;
    bool CombatStrafeTurningEnabled; CWorld* World; bool DrawDebug; bool TraceDebug;
    float ProximityForNavigation; bool EnableAINonAttackMode;
    CombatDialogueManager DialogueManager;
    int registeredSeenEmpty;
    explicit Manager(CWorld& world)
        : CombatStrafeTurningEnabled(true), World(&world), DrawDebug(false), TraceDebug(false),
          ProximityForNavigation(2.6f), EnableAINonAttackMode(false), registeredSeenEmpty(0)
    {
        RegisterCombatSequences();
    }
    void RegisterCombatSequences()
    {
        ++g_registerCalls;
        registeredSeenEmpty = RegisteredCombatSequences.empty() && Combatants.empty() && g_dialogueCalls == 1;
        RegisteredCombatSequences["Attack"] = 1;
    }
};
#define FAIL { std::printf("006ed3f0_TEST FAIL\n"); return 1; }
int main()
{
    CWorld w = { 0 };
    Manager m(w);
    if (g_registerCalls != 1 || !m.registeredSeenEmpty) FAIL;           // dialogue manager built, maps empty first
    if (m.World != &w || !m.CombatStrafeTurningEnabled || m.DrawDebug || m.TraceDebug || m.EnableAINonAttackMode) FAIL;
    if (m.ProximityForNavigation != 2.6f) FAIL;
    if (m.RegisteredCombatSequences.size() != 1 || !m.Combatants.empty()) FAIL;
    std::printf("006ed3f0_TEST PASS\n");
    return 0;
}