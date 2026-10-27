#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct Thing { bool killed; bool MsgIsKilledBy(const char* w) const { return w[0] == 0 && killed; } };
struct Parent { float UndeadBar; int UndeadAlive; };
struct Undead { Thing Me; Parent* ParentClass; float RaiseUndeadBarAmount;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy("")) {
            float amount;
            if (ParentClass->UndeadBar < 50.0f)
                amount = RaiseUndeadBarAmount * 4.0f;
            else if (ParentClass->UndeadBar < 100.0f)
                amount = RaiseUndeadBarAmount * 3.0f;
            else if (ParentClass->UndeadBar < 200.0f)
                amount = RaiseUndeadBarAmount * 2.0f;
            else
                amount = RaiseUndeadBarAmount;
            ParentClass->UndeadBar += amount;
            --ParentClass->UndeadAlive;
        }
    }
};
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    bool ok = true;
    Parent p; Undead u; u.ParentClass = &p; u.RaiseUndeadBarAmount = 2.0f; u.Me.killed = true;
    float bars[4] = { 10.0f, 60.0f, 150.0f, 250.0f }; float want[4] = { 18.0f, 66.0f, 154.0f, 252.0f };
    for (int i = 0; i < 4; ++i) { p.UndeadBar = bars[i]; p.UndeadAlive = 3; u.OnPredicateFail(); ok = ok && Near(p.UndeadBar, want[i]) && p.UndeadAlive == 2; }
    u.Me.killed = false; p.UndeadBar = 10.0f; p.UndeadAlive = 3; u.OnPredicateFail();
    ok = ok && Near(p.UndeadBar, 10.0f) && p.UndeadAlive == 3;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e95dc0_TEST FAIL\n"); return 1; }
    std::printf("00e95dc0_TEST PASS\n");
    return 0;
}