// Behaviour test for CV_ArcheryCompetitionScript::WatchForQuestFinished (00e32f30).
#include <cstdio>

struct Script;
struct FakeGSI { Script* s; int frames; void NewScriptFrame(); };

struct Script {
    FakeGSI* GSI;
    bool IsOwnerDead, QuestFinished;
    long ContestState;
    int termCalls, terminateAt, deadAtFrame, stateAtFrame;
    bool IsActiveThreadTerminating() { return ++termCalls >= terminateAt; }
    void WatchForQuestFinished()
    {
        while (!IsActiveThreadTerminating()) {
            if (IsOwnerDead) {
                if (IsActiveThreadTerminating())
                    return;
                while (ContestState != 0 && ContestState != 2) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                QuestFinished = true;
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                IsActiveThreadTerminating();
                return;
            }
            GSI->NewScriptFrame();
        }
    }
};

void FakeGSI::NewScriptFrame()
{
    ++frames;
    if (frames == s->deadAtFrame) s->IsOwnerDead = true;
    if (frames == s->stateAtFrame) s->ContestState = 2;
}

int main()
{
    Script s; FakeGSI g; g.s = &s; g.frames = 0; s.GSI = &g;
    s.IsOwnerDead = false; s.QuestFinished = false; s.ContestState = 1;
    s.termCalls = 0; s.terminateAt = 1000; s.deadAtFrame = 3; s.stateAtFrame = 5;
    s.WatchForQuestFinished();
    bool ok = s.QuestFinished && g.frames == 7;
    Script t; FakeGSI h; h.s = &t; h.frames = 0; t.GSI = &h;
    t.IsOwnerDead = false; t.QuestFinished = false; t.ContestState = 1;
    t.termCalls = 0; t.terminateAt = 4; t.deadAtFrame = 100; t.stateAtFrame = 100;
    t.WatchForQuestFinished();
    ok = ok && !t.QuestFinished && h.frames == 3;
    if (!ok) { std::printf("00e32f30_TEST FAIL\n"); return 1; }
    std::printf("00e32f30_TEST PASS\n");
    return 0;
}
