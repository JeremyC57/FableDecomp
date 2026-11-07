// Behaviour test for NScript::CQ_HobbeToothContestScript::CSpawnNewHobbe::Main (0x00D9DAF0): the hobbe spawner waits while the
// drinking hobbes are unhit, then each frame until the outro starts counts the live hobbes and, when neither the hero nor the whisper
// is within 10 of it and fewer than three hobbes live, spawns a grunt (rand % 4 > 0), else a spellcaster (second roll), else a
// lieutenant, aims it at the nearer of whisper and hero and pauses. A frame-stepped model with a scripted rand.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Spawner {
    int Frame, TerminateAt, DrinkEndAt, OutroAt, Hobbes, HeroNearFrom, Roll;
    bool DrinkingHobbesNotHit, CS_OutroBegin;
    const int* Rolls;
    float HeroD, WhisperD;
    void NSF()
    {
        ++Frame;
        if (Frame == DrinkEndAt) DrinkingHobbesNotHit = false;
        if (Frame == OutroAt) CS_OutroBegin = true;
    }
    bool T() const { return Frame >= TerminateAt; }
    int Rand() { return Rolls[Roll++]; }
    void Main()
    {
        while (DrinkingHobbesNotHit) { NSF(); if (T()) return; }
        if (T()) return;
        while (!CS_OutroBegin) {
            NSF(); if (T()) return;
            int count = Hobbes;
            bool heroNear = Frame >= HeroNearFrom;
            if (!heroNear && count < 3) {
                if (T()) return;
                if (Rand() % 4 > 0) { if (T()) return; Log("GRUNT"); }
                else if (Rand() % 4 > 0) { if (T()) return; Log("SPELLCASTER"); }
                else { if (T()) return; Log("LIEUTENANT"); }
                ++Hobbes;
                if (WhisperD < HeroD) { if (T()) return; Log("whisper"); }
                else { if (T()) return; Log("hero"); }
                Log("pause");
            }
        }
        if (T()) return;
    }
};
int main()
{
    static const int rolls[] = { 1, 0, 2, 0, 0, 3 };
    Spawner a = { 0, 100, 3, 20, 0, 100, 0, true, false, rolls, 5.0f, 2.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "GRUNT;whisper;pause;SPELLCASTER;whisper;pause;LIEUTENANT;whisper;pause;") && a.Hobbes == 3 && a.Frame == 20;
    g_log[0] = 0;
    Spawner b = { 0, 100, 2, 10, 0, 4, 0, true, false, rolls, 1.0f, 2.0f };
    b.Main();
    ok = ok && !std::strcmp(g_log, "GRUNT;hero;pause;") && b.Frame == 10;
    g_log[0] = 0;
    Spawner c = { 0, 4, 50, 60, 0, 100, 0, true, false, rolls, 1.0f, 2.0f };
    c.Main();
    ok = ok && g_log[0] == 0 && c.Frame == 4;
    std::printf(ok ? "00d9daf0_TEST PASS\n" : "00d9daf0_TEST FAIL\n");
    return ok ? 0 : 1;
}