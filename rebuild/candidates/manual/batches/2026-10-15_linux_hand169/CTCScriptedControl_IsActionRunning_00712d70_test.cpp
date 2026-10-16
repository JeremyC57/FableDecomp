#include <cstdio>
#include <cstring>
// Behaviour model of CTCScriptedControl::IsActionRunning (0x00712d70): name search over the action list.
struct Action { const char* name; bool terminated; };
static bool Find(Action* a, int n, const char* name, bool terminate)
{
    for (int i = 0; i < n; ++i)
        if (std::strcmp(a[i].name, name) == 0) { if (terminate) a[i].terminated = true; return true; }
    return false;
}
int main()
{
    Action a[3] = { {"walk", false}, {"talk", false}, {"talk", false} };
    bool term = false;
    if (!Find(a, 3, "talk", term) || Find(a, 3, "run", term) || a[1].terminated != term || a[2].terminated) { std::printf("00712d70_TEST FAIL\n"); return 1; }
    std::printf("00712d70_TEST PASS\n");
    return 0;
}
