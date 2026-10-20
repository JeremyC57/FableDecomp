// Behaviour test for CV_ArcheryCompetitionScript::WatchForSignpostConditions (00e33120).
#include <cstdio>

struct Script;
struct FakeGSI { Script* s; int frames; void NewScriptFrame(); };
struct Owner { long m_phase; long m_signpostState; };

struct Script {
    FakeGSI* GSI; Owner* O;
    bool QuestActivated, AttackedByHero, QuestFinished;
    int termCalls, terminateAt;
    bool IsActiveThreadTerminating() { return ++termCalls >= terminateAt; }
    void Watch()
    {
        O->m_signpostState = 2;
        while (!IsActiveThreadTerminating()) {
            long phase = O->m_phase;
            if (phase == 0) {
                if (IsActiveThreadTerminating()) return;
                if (AttackedByHero) {
                    if (IsActiveThreadTerminating()) return;
                    O->m_signpostState = 2;
                } else if (QuestFinished) {
                    if (IsActiveThreadTerminating()) return;
                    O->m_signpostState = 1;
                }
            } else if (phase == 2) {
                if (IsActiveThreadTerminating()) return;
                if (QuestActivated && !AttackedByHero) {
                    if (IsActiveThreadTerminating()) return;
                    O->m_signpostState = 0;
                }
            }
            GSI->NewScriptFrame();
        }
    }
};
int log_[8];
void FakeGSI::NewScriptFrame() { log_[frames] = s->O->m_signpostState; ++frames; if (frames == 1) s->O->m_phase = 2; if (frames == 2) s->QuestFinished = true, s->O->m_phase = 0; if (frames == 3) s->AttackedByHero = true; }

int main()
{
    Owner o = { 1, -1 }; Script s; FakeGSI g = { &s, 0 };
    s.GSI = &g; s.O = &o; s.QuestActivated = true; s.AttackedByHero = false; s.QuestFinished = false; s.termCalls = 0; s.terminateAt = 1000;
    // run 4 frames then terminate: count term calls per frame to stop at the 4th loop test
    s.terminateAt = 1 + 1 + 3 + 3 + 3 + 1;
    s.Watch();
    bool ok = g.frames == 4 && log_[0] == 2 && log_[1] == 0 && log_[2] == 1 && log_[3] == 2;
    if (!ok) { std::printf("00e33120_TEST FAIL %d %ld %ld %ld %ld\n", g.frames, (long)log_[0], (long)log_[1], (long)log_[2], (long)log_[3]); return 1; }
    std::printf("00e33120_TEST PASS\n");
    return 0;
}
