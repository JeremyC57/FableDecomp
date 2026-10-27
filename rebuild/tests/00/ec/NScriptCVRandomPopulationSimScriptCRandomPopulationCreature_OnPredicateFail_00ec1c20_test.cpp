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
struct Parent { float CurrentPopulationScale[4]; int CurrentPopulationRegion; int CreaturesAlive[3]; };
struct Creature { Thing Me; Parent* ParentClass; int MyType;
    void OnPredicateFail()
    {
        --ParentClass->CreaturesAlive[MyType];
        if (Me.MsgIsKilledBy("")) {
            if (MyType == 2)
                ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] -= 0.05f;
            else
                ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion] += 0.05f;
            float& scale = ParentClass->CurrentPopulationScale[ParentClass->CurrentPopulationRegion];
            if (scale < 0.0f)
                scale = 0.0f;
            else if (scale > 1.0f)
                scale = 1.0f;
        }
    }
};
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    bool ok = true;
    Parent p; Creature c; c.ParentClass = &p; c.Me.killed = true;
    for (int i = 0; i < 4; ++i) p.CurrentPopulationScale[i] = 0.5f;
    p.CurrentPopulationRegion = 1; p.CreaturesAlive[0] = 5; p.CreaturesAlive[2] = 5;
    c.MyType = 0; c.OnPredicateFail();
    ok = ok && Near(p.CurrentPopulationScale[1], 0.55f) && p.CreaturesAlive[0] == 4 && Near(p.CurrentPopulationScale[0], 0.5f);
    c.MyType = 2; c.OnPredicateFail(); c.OnPredicateFail();
    ok = ok && Near(p.CurrentPopulationScale[1], 0.45f) && p.CreaturesAlive[2] == 3;
    p.CurrentPopulationScale[1] = 0.02f; c.OnPredicateFail(); ok = ok && p.CurrentPopulationScale[1] == 0.0f;
    p.CurrentPopulationScale[1] = 0.98f; c.MyType = 1; p.CreaturesAlive[1] = 1; c.OnPredicateFail(); ok = ok && p.CurrentPopulationScale[1] == 1.0f && p.CreaturesAlive[1] == 0;
    c.Me.killed = false; p.CurrentPopulationScale[1] = 0.5f; c.OnPredicateFail(); ok = ok && p.CurrentPopulationScale[1] == 0.5f && p.CreaturesAlive[1] == -1;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ec1c20_TEST FAIL\n"); return 1; }
    std::printf("00ec1c20_TEST PASS\n");
    return 0;
}