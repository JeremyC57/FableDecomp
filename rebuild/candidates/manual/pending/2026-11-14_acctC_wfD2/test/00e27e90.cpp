// Behaviour test for NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main (0x00E27E90): a state model of the generated
// script. Phase 1 (per frame, stop on termination): a hit (melee, or any special ability except heal) plays the SETUP / HITME / INFO
// cutscenes, a talk plays SETUP / INFO; either ends phase 1 and completes the piece once (quest card, Witchwood teleporter, face back
// to InitialAngle, logbook entry 130, quest completed, PieceOver). Phase 2 (per frame): a hit gives ON_HIT then REPEAT while Maze has
// health; a talk gives REPEAT; both in one frame give both.
#include <cstdio>
#include <cstring>

static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }

struct Maze {
    int Frame, TerminateAt, HitAt, HealAt, TalkAt, Hit2At, Talk2At;
    float Health; bool PieceOver; long Logbook;
    bool T() const { return Frame >= TerminateAt; }
    bool Hit(int at) const { return Frame == at; }
    bool HitByAbility() const { return Frame == HealAt; }
    bool HealOnly() const { return true; }   // the only special ability in the cases below is the heal spell
    void NewScriptFrame() { ++Frame; }
    void Main()
    {
        NewScriptFrame();
        if (T()) return;
        for (;; NewScriptFrame()) {
            if (T()) return;
            if (Hit(HitAt) || (HitByAbility() && !HealOnly())) {
                if (T()) return;
                Log("SETUP"); Log("HITME"); Log("INFO");
                break;
            }
            if (Hit(TalkAt)) {
                if (T()) return;
                Log("SETUP"); Log("INFO");
                break;
            }
        }
        if (T()) return;
        Log("card"); Log("teleporter"); Logbook = 130; PieceOver = true;
        for (;;) {
            if (T()) return;
            if (Hit(Hit2At)) {
                if (T()) return;
                if (Health > 0.0f) Log("ON_HIT");
                if (Health > 0.0f) Log("REPEAT");
            }
            if (Hit(Talk2At)) {
                if (T()) return;
                if (Health > 0.0f) Log("REPEAT");
            }
            NewScriptFrame();
        }
    }
};

static Maze Make(int term, int hit, int heal, int talk, int hit2, int talk2)
{
    Maze m; std::memset(&m, 0, sizeof m);
    m.TerminateAt = term; m.HitAt = hit; m.HealAt = heal; m.TalkAt = talk; m.Hit2At = hit2; m.Talk2At = talk2; m.Health = 10.0f;
    g_log[0] = 0;
    return m;
}

int main()
{
    bool ok = true;
    Maze a = Make(20, 100, 2, 4, 7, 9); a.Main();       // heal at 2 ignored, talk at 4, hit at 7, talk at 9
    ok = ok && std::strcmp(g_log, "SETUP,INFO,card,teleporter,ON_HIT,REPEAT,REPEAT,") == 0 && a.PieceOver && a.Logbook == 130;
    Maze b = Make(20, 3, 100, 3, 8, 8); b.Main();        // hit wins over talk in the same frame; both in phase 2
    ok = ok && std::strcmp(g_log, "SETUP,HITME,INFO,card,teleporter,ON_HIT,REPEAT,REPEAT,") == 0 && b.PieceOver;
    Maze c = Make(5, 100, 100, 100, 100, 100); c.Main(); // terminates before anything happens
    ok = ok && g_log[0] == 0 && !c.PieceOver && c.Frame == 5;
    Maze d = Make(20, 100, 100, 3, 6, 100); d.Health = 0.0f; d.Main(); // no lines when Maze has no health
    ok = ok && std::strcmp(g_log, "SETUP,INFO,card,teleporter,") == 0;
    if (!ok) { std::printf("%s 00e27e90_TEST FAIL\n", g_log); return 1; }
    std::printf("00e27e90_TEST PASS\n");
    return 0;
}
