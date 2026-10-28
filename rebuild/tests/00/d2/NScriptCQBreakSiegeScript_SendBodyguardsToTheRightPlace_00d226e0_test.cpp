// Behaviour test for 0x00D226E0 NScript::CQ_BreakSiegeScript::SendBodyguardsToTheRightPlace(CCharString, CCharString)
#include <stdio.h>
#include <string.h>

static int g_guards = 0;
static bool g_terminating = false;
static int g_teleports = 0;
static int g_teleported[4];
static const char* g_targets[4];
static void Teleport(int guard, const char* marker) { g_teleported[g_teleports] = guard; g_targets[g_teleports] = marker; ++g_teleports; }
static void SendBodyguardsToTheRightPlace(const char* first, const char* second)   // the row's body
{
    int guards[8]; int n = 0;
    for (int i = 0; i < g_guards; ++i) guards[n++] = 100 + i;      // GetAllThingsWithScriptName("BodyGuard")
    if (n == 1) {
        if (g_terminating) return;
        Teleport(guards[0], first);
    }
    if (n == 2) {
        if (g_terminating) return;
        Teleport(guards[0], first);
        Teleport(guards[1], second);
    }
}

int main()
{
    int ok = 1;
    g_guards = 1; SendBodyguardsToTheRightPlace("A", "B");
    ok &= g_teleports == 1 && g_teleported[0] == 100 && strcmp(g_targets[0], "A") == 0;
    g_teleports = 0; g_guards = 2; SendBodyguardsToTheRightPlace("A", "B");
    ok &= g_teleports == 2 && g_teleported[1] == 101 && strcmp(g_targets[1], "B") == 0;
    g_teleports = 0; g_guards = 3; SendBodyguardsToTheRightPlace("A", "B");
    ok &= g_teleports == 0;
    g_guards = 2; g_terminating = true; SendBodyguardsToTheRightPlace("A", "B");
    ok &= g_teleports == 0;
    printf(ok ? "00d226e0_TEST PASS\n" : "00d226e0_TEST FAIL\n");
    return ok ? 0 : 1;
}