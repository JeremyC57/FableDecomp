// Behaviour test for CV_BodyGuardScript::CV_BodyGuardScript (00e5d140): member construction order and values.
#include <cstdio>
struct FakeGSI { long timers; long RegisterTimer() { return ++timers; } long GetWorldFrame() const { return 500; } };
static FakeGSI g_gsi = { 0 };
static FakeGSI* g_pGameScriptInterface = &g_gsi;
class CTimer { public: long Index; CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { } };
class CCounter { public: long StartFrame; CCounter() : StartFrame(g_pGameScriptInterface->GetWorldFrame()) { } };
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) { } };
struct Base { FakeGSI* Game; void* Data; Base(void* d, FakeGSI* g) : Game(g), Data(d) { } };
struct BodyGuard : Base {
    long TotalWages; bool A, B; CCounter Counter; CTimer T0, T1, T2, T3; Vec List; long Conv; CTimer Respawn[4]; long Status[4];
    BodyGuard(void* d, FakeGSI* g) : Base(d, g) { }
};
int main()
{
    int data = 0;
    BodyGuard b(&data, &g_gsi);
    bool ok = b.Game == &g_gsi && b.Data == &data && b.Counter.StartFrame == 500 && b.T0.Index == 1 && b.T3.Index == 4
        && b.List.a == 0 && b.List.c == 0 && b.Respawn[0].Index == 5 && b.Respawn[3].Index == 8 && g_gsi.timers == 8;
    if (!ok) { std::printf("00e5d140_TEST FAIL\n"); return 1; }
    std::printf("00e5d140_TEST PASS\n");
    return 0;
}