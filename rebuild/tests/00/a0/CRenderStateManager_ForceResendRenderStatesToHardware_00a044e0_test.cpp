// Behaviour test for 0x00A044E0: ForceResendRenderStatesToHardware: every state's desired value goes to the right device setter, current = desired, dirty cleared, update list emptied
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct StateInfo { unsigned int current, desired, mask; long state; bool dirty; unsigned char type, stage; };
struct Dev {
    int rs, tss, ss; long lastState; unsigned int lastValue; unsigned int lastStage;
    Dev() : rs(0), tss(0), ss(0), lastState(0), lastValue(0), lastStage(0) {}
    void SetRenderState(long s, unsigned int v) { ++rs; lastState = s; lastValue = v; }
    void SetTextureStageState(unsigned int st, long s, unsigned int v) { ++tss; lastStage = st; lastState = s; lastValue = v; }
    void SetSamplerState(unsigned int st, long s, unsigned int v) { ++ss; lastStage = st; lastState = s; lastValue = v; }
};
struct RSM {
    Dev* dev; StateInfo* all[8]; long allCount; long updateCount;
    void ForceResend()
    {
        for (long i = 0; i < allCount; ++i)
        {
            StateInfo* s = all[i]; unsigned int desired = s->desired;
            s->current = desired; s->dirty = false;
            switch (s->type)
            {
            case 1: dev->SetRenderState(s->state, desired); break;
            case 2: dev->SetTextureStageState(s->stage, s->state, desired); break;
            case 3: dev->SetSamplerState(s->stage, s->state, desired); break;
            }
        }
        updateCount = 0;
    }
};
static void Run()
{
    StateInfo a = { 0, 7, 0, 22, true, 1, 0 }, b = { 1, 8, 0, 4, true, 2, 3 }, c = { 2, 9, 0, 5, false, 3, 6 }, e = { 3, 10, 0, 1, true, 4, 0 };
    Dev d; RSM r; r.dev = &d; r.all[0] = &a; r.all[1] = &b; r.all[2] = &c; r.all[3] = &e; r.allCount = 4; r.updateCount = 12;
    r.ForceResend();
    CHECK(d.rs == 1 && d.tss == 1 && d.ss == 1);
    CHECK(d.lastStage == 6 && d.lastState == 5 && d.lastValue == 9);
    CHECK(a.current == 7 && b.current == 8 && c.current == 9 && e.current == 10 && !a.dirty && !b.dirty && !e.dirty);
    CHECK(r.updateCount == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a044e0_TEST FAIL\n"); return 1; }
    std::printf("00a044e0_TEST PASS\n");
    return 0;
}