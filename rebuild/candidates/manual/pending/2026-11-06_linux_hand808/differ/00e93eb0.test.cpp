// Behaviour test for NScript::CV_HauntedHouseScript::Main (0x00E93EB0): until Q_TraderEscort is done raise the forcefield barrier
// whenever the haunted house region loads (lock the cellar door and the house door, candles off), re-create the two forcefields when
// both died while the barrier marker lives, drop the barrier flag when the region unloads; then remove the forcefields, bind the four
// entity scripts, spawn CheckRegionLeaving, wait for the hero inside (candles off while outside), light the candles until the cellar
// door opens, and put them out again.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct HauntedHouse {
    int Frames, TerminateAt, QuestDoneAt, LoadedFrom, LoadedTo, FieldsDieAt, InHouseAt, CellarAt;
    bool T() const { return Frames >= TerminateAt; }
    void NewScriptFrame() { ++Frames; }
    bool Loaded() const { return Frames >= LoadedFrom && Frames < LoadedTo; }
    void Main()
    {
        bool barrierUp = false, fieldsAlive = false;
        while (Frames < QuestDoneAt) {
            NewScriptFrame(); if (T()) return;
            if (!barrierUp && Loaded()) { Log("barrier"); fieldsAlive = true; barrierUp = true; }
            if (barrierUp && Frames == FieldsDieAt) fieldsAlive = false;
            if (barrierUp && !fieldsAlive) { Log("recreate"); fieldsAlive = true; }
            if (barrierUp && !Loaded()) barrierUp = false;
        }
        if (T()) return;
        if (fieldsAlive) Log("remove");
        Log("bind4"); Log("spawn");
        while (Frames < InHouseAt) { NewScriptFrame(); if (T()) return; }
        Log("lights_on");
        while (Frames < CellarAt) { NewScriptFrame(); if (T()) return; }
        Log("lights_off");
    }
};
int main()
{
    HauntedHouse a = { 0, 30, 8, 2, 20, 4, 10, 12 };
    a.Main();
    bool ok = !std::strcmp(g_log, "barrier;recreate;remove;bind4;spawn;lights_on;lights_off;") && a.Frames == 12;
    g_log[0] = 0;
    HauntedHouse b = { 0, 5, 9, 1, 3, -1, 0, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "barrier;") && b.Frames == 5;
    std::printf(ok ? "00e93eb0_TEST PASS\n" : "00e93eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
