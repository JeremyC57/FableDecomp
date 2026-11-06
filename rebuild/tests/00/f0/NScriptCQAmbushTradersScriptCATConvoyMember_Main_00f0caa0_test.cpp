// Behaviour test for NScript::CQ_AmbushTradersScript::CAT_ConvoyMember::Main (0x00F0CAA0): wait for a convoy region, take control
// (priority 4 in Bowerstone Jail, else 2), in the Deserted Farm hold a "WaitForTrigger" member until ConvoyTrigger2 and mark it
// "Triggered", walk to the exit point, then per frame: on quest fail/finish run to REGION_EXIT_POINT and fade; at the exit point fade
// (jail), record a teleport entry (farm) or flag AnyTraderEscaped for the CREATURE_TRADER_01 convoy member (woods). A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Member {
    int Region;                         // 0 none yet, 1 jail, 2 farm, 3 woods (from frame RegionAt on)
    int RegionAt, Frames, TriggerAt, ArriveAt, Teleports, Priority;
    bool QuestFailed, QuestFinished, AnyTraderEscaped, Terminate;
    const char* Data; const char* DefName; const char* Name;
    bool IsRegionLoaded(int r) { return Frames >= RegionAt && Region == r; }
    void NewScriptFrame() { ++Frames; if (Frames >= 50) Terminate = true; }
    bool IsActiveThreadTerminating() { return Terminate; }
    bool Trigger2() { return Frames >= TriggerAt; }
    bool AtExit() { return Frames >= ArriveAt; }
    void Fade() { Log("fade"); Terminate = true; }    // the entity dies: the thread ends next check
    void Main()
    {
        while (!(IsRegionLoaded(1) || IsRegionLoaded(2) || IsRegionLoaded(3))) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Priority = IsRegionLoaded(1) ? 4 : 2;
        if (IsRegionLoaded(2)) {
            if (!std::strcmp(Data, "WaitForTrigger")) {
                while (!Trigger2()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                Data = "Triggered";
            }
        }
        Log("move_exit");
        for (;;) {
            if (QuestFailed || QuestFinished) { Log("region_exit"); Fade(); }
            if (!Terminate && AtExit()) {
                if (IsRegionLoaded(1)) Fade();
                else if (IsRegionLoaded(2)) { ++Teleports; Fade(); }
                else if (IsRegionLoaded(3)) {
                    if (!std::strcmp(DefName, "CREATURE_TRADER_01") && !std::strcmp(Name, "AT_ConvoyMember") && !AnyTraderEscaped)
                        AnyTraderEscaped = true;
                    Fade();
                }
            }
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
};
int main()
{
    Member a = { 2, 2, 0, 5, 7, 0, 0, false, false, false, false, "WaitForTrigger", "CREATURE_TRADER_01", "AT_ConvoyMember" };
    a.Main();
    bool ok = !std::strcmp(g_log, "move_exit;fade;") && a.Teleports == 1 && a.Priority == 2 && !std::strcmp(a.Data, "Triggered") && a.Frames == 8;
    g_log[0] = 0;
    Member b = { 3, 1, 0, 0, 3, 0, 0, false, false, false, false, "", "CREATURE_TRADER_01", "AT_ConvoyMember" };
    b.Main();
    ok = ok && !std::strcmp(g_log, "move_exit;fade;") && b.AnyTraderEscaped && b.Teleports == 0;
    g_log[0] = 0;
    Member c = { 3, 1, 0, 0, 3, 0, 0, false, false, false, false, "", "CREATURE_GUARD", "AT_ConvoyMember" };
    c.Main();
    ok = ok && !c.AnyTraderEscaped;
    g_log[0] = 0;
    Member d = { 1, 0, 0, 0, 40, 0, 0, true, false, false, false, "", "CREATURE_TRADER_01", "AT_ConvoyMember" };
    d.Main();
    ok = ok && !std::strcmp(g_log, "move_exit;region_exit;fade;") && d.Priority == 4 && d.Frames == 1;
    std::printf(ok ? "00f0caa0_TEST PASS\n" : "00f0caa0_TEST FAIL\n");
    return ok ? 0 : 1;
}