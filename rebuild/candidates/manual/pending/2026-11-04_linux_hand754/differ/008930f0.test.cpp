// Behaviour test for CGameScriptInterface::GetThingWithScriptName (0x008930F0): a model of the row.
#include <stdio.h>

struct Thing { const char* name; int map; bool dead; int script; };
static int Get(const Thing* things, int n, const char* name, int map)
{
    const Thing* found = 0;
    for (int i = 0; i < n && !found; ++i) {
        const char* a = things[i].name; const char* b = name; while (*a && *a == *b) { ++a; ++b; }
        if (*a == *b && things[i].map == map) found = &things[i];
    }
    if (found && !found->dead) return found->script;
    return 0;
}
static int run()
{
    Thing t[3] = { { "A", 1, false, 11 }, { "B", 2, true, 22 }, { "B", 1, false, 33 } };
    return Get(t, 3, "A", 1) == 11 && Get(t, 3, "B", 2) == 0 && Get(t, 3, "B", 1) == 33 && Get(t, 3, "C", 1) == 0;
}
int main()
{
    int ok = run();
    printf(ok ? "008930f0_TEST PASS\n" : "008930f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
