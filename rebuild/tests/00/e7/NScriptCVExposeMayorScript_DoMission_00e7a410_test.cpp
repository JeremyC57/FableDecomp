// Behaviour test for 0x00E7A410 NScript::CV_ExposeMayorScript::DoMission: from stage 0 the thread waits for BowerstonePosh and
// runs PhaseOne; from a later stage it skips straight to PhaseTwo; PhaseTwo .. PhaseFive run in order, then the thread waits
// for stage 5 and runs EndMission. A terminating thread stops.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
class CGameScriptInterface { public:
    int Frames, LoadedAt, Stage5At; long* Stage;
    void NewScriptFrame() { ++Frames; if (Stage5At && Frames >= Stage5At) *Stage = 5; }
    bool IsLevelLoaded(const CCharString& n) const { return !std::strcmp(n.Text, "BowerstonePosh") && Frames >= LoadedAt; }
};
struct CV_ExposeMayorScript {
    CGameScriptInterface* Game; long Path, StageNumber; int TermAt, TermCalls;
    bool IsActiveThreadTerminating() { ++TermCalls; return TermAt && TermCalls >= TermAt; }
    void PhaseOne() { Log("1"); }
    void PhaseTwo() { Log("2"); }
    void PhaseThree() { Log("3"); }
    void PhaseFour() { Log("4"); }
    void PhaseFive() { Log("5"); }
    void EndMission() { Log("end"); }
    void DoMission()
    {
        if (StageNumber == 0) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsLevelLoaded("BowerstonePosh")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            PhaseOne();
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        PhaseTwo();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        PhaseThree();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        PhaseFour();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        PhaseFive();
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (StageNumber != 5) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        EndMission();
    }
};

int main()
{
    CV_ExposeMayorScript s; CGameScriptInterface g;
    g.Frames = 0; g.LoadedAt = 3; g.Stage5At = 12; g.Stage = &s.StageNumber;
    s.Game = &g; s.Path = 0; s.StageNumber = 0; s.TermAt = 0; s.TermCalls = 0;
    s.DoMission();
    int ok = !std::strcmp(g_log, "1;2;3;4;5;end;") && g.Frames == 12;
    g_log[0] = 0; g.Frames = 0; g.Stage5At = 0; s.StageNumber = 2; s.TermCalls = 0;
    s.TermAt = 6;                                       // stops while waiting for stage 5
    s.DoMission();
    ok = ok && !std::strcmp(g_log, "2;3;4;5;");
    if (!ok) { std::printf("00e7a410_TEST FAIL\n"); return 1; }
    std::printf("00e7a410_TEST PASS\n");
    return 0;
}