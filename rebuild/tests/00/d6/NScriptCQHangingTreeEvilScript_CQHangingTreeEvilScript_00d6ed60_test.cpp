// Behaviour test for NScript::CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript() @ 0x00D6ED60: the implicit destructor
// frees the three vectors (last first), deregisters the mission timer, releases the strings/things, then the base.
#include <cstdio>
#include <string>
#include <vector>
namespace model {
std::string g_order;
struct CGameScriptInterface { void DeregisterTimer(long index) { g_order += "Timer"; g_order += (char)('0' + index); g_order += ';'; } } g_interface;
struct CTimer { long TimerIndex; CTimer(long i) : TimerIndex(i) {} ~CTimer() { g_interface.DeregisterTimer(TimerIndex); } };
struct CCharString { ~CCharString() { g_order += "String;"; } };
struct CScriptThing { ~CScriptThing() { g_order += "Thing;"; } };
template <class T> struct Vec { std::vector<T> v; const char* n; Vec(const char* name) : n(name) {} ~Vec() { g_order += n; g_order += ';'; } };
struct CLevelScriptBase { virtual ~CLevelScriptBase() { g_order += "Base;"; } };
struct CQ_HangingTreeEvilScript : CLevelScriptBase {
    bool flags[5]; long ReasonForFailing; long counters[5];
    CScriptThing Executioner, Prisoner, PartySpawnPoint, BanditExitPoint;
    CTimer MissionTimer; CCharString CurrentLevel;
    Vec<int> CurrentEnemyVector, CurrentBanditVector, AreasSpawnedIn;
    CQ_HangingTreeEvilScript() : MissionTimer(3), CurrentEnemyVector("Enemies"), CurrentBanditVector("Bandits"), AreasSpawnedIn("Areas") {}
};
}
int main()
{
    { model::CQ_HangingTreeEvilScript script; }
    bool ok = model::g_order == "Areas;Bandits;Enemies;String;Timer3;Thing;Thing;Thing;Thing;Base;";
    if (!ok) { std::printf("00d6ed60_TEST FAIL %s\n", model::g_order.c_str()); return 1; }
    std::printf("00d6ed60_TEST PASS\n");
    return 0;
}