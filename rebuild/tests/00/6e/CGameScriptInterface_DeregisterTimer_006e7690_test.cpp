// Behaviour test for 0x006E7690 CGameScriptInterface::DeregisterTimer: erase the timer if registered (standalone copy of the logic)
#include <cstdio>
#include <map>
struct Gsi { std::map<long, long> ScriptTimers; void DeregisterTimer(long t) { std::map<long, long>::iterator it = ScriptTimers.find(t); if (it != ScriptTimers.end()) ScriptTimers.erase(it); } };

int main()
{
    Gsi g; g.ScriptTimers[3] = 30; g.ScriptTimers[5] = 50;
    g.DeregisterTimer(4); if (g.ScriptTimers.size() != 2) { std::printf("006e7690_TEST FAIL\n"); return 1; }
    g.DeregisterTimer(3); if (g.ScriptTimers.size() != 1 || g.ScriptTimers.count(3) || g.ScriptTimers[5] != 50) { std::printf("006e7690_TEST FAIL\n"); return 1; }
    std::printf("006e7690_TEST PASS\n");
    return 0;
}