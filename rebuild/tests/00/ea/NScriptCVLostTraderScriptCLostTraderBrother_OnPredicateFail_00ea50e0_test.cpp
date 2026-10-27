#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct ThingK { bool Killed; bool MsgIsKilledBy(const char* killer) const { return Killed && killer[0] == 0; } };
struct Parent { bool g_bLostTraderKilled, g_bTraderBrotherKilled; int ReasonForFailure; };
struct E { ThingK Me; Parent* ParentClass;
    void OnPredicateFail()
    {
        if (Me.MsgIsKilledBy("")) {
            ParentClass->ReasonForFailure = 1;
            ParentClass->g_bTraderBrotherKilled = true;
        }
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    Parent p; p.g_bLostTraderKilled = false; p.g_bTraderBrotherKilled = false; p.ReasonForFailure = 0;
    E e; e.ParentClass = &p; e.Me.Killed = false; e.OnPredicateFail();
    ok = !p.g_bTraderBrotherKilled && p.ReasonForFailure == 0;
    e.Me.Killed = true; e.OnPredicateFail();
    ok = ok && p.g_bTraderBrotherKilled && p.ReasonForFailure == 1 && !p.g_bLostTraderKilled;
    if (!ok) { std::printf("00ea50e0_TEST FAIL\n"); return 1; }
    std::printf("00ea50e0_TEST PASS\n");
    return 0;
}