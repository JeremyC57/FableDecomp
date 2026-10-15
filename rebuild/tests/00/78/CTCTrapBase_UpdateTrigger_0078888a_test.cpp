#include <cstdio>
struct CTrapDef { long TriggerType; };
static long g_frame = 0;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
static CWorld g_world;
static int g_updates = 0;
struct CTCTrapBase {
    CTrapDef* PTrapDef_Object;
    bool Active;
    CWorld* GetWorld() { return &g_world; }
    void UpdateTriggerOnProximity() { ++g_updates; }
    void UpdateTrigger()
    {
        if (Active) {
            if ((GetWorld()->GetWorldFrame() & 8) == 0) {
                if (PTrapDef_Object->TriggerType == 1)
                    UpdateTriggerOnProximity();
            }
        }
    }
};
int main()
{
    CTrapDef def; def.TriggerType = 1;
    CTCTrapBase t; t.PTrapDef_Object = &def; t.Active = true;
    g_frame = 3; t.UpdateTrigger();            // window open
    if (g_updates != 1) { std::printf("0078888a_TEST FAIL\n"); return 1; }
    g_frame = 9; t.UpdateTrigger();            // bit 3 set: skipped
    if (g_updates != 1) { std::printf("0078888a_TEST FAIL\n"); return 1; }
    g_frame = 16; t.Active = false; t.UpdateTrigger();
    if (g_updates != 1) { std::printf("0078888a_TEST FAIL\n"); return 1; }
    t.Active = true; def.TriggerType = 0; t.UpdateTrigger();
    if (g_updates != 1) { std::printf("0078888a_TEST FAIL\n"); return 1; }
    def.TriggerType = 1; t.UpdateTrigger();
    if (g_updates != 2) { std::printf("0078888a_TEST FAIL\n"); return 1; }
    std::printf("0078888a_TEST PASS\n");
    return 0;
}