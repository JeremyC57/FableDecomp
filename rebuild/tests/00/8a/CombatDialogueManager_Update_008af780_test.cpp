#include <cstdio>
// Behaviour test: Update retries a pending group, keeps a talking talker, and otherwise drops the talker and stamps the frame.
struct CTCTalk { bool talking; bool IsTalking() const { return talking; } };
struct CThingCreatureBase {
    unsigned char Dead : 1;
    CTCTalk* talk;
    bool GetInterface(CTCTalk** out, int type) { if (type != 0xf || talk == 0) return false; *out = talk; return true; }
};
struct CPtr {
    CThingCreatureBase* p;
    CPtr() : p(0) { }
    CPtr(CThingCreatureBase* q) : p(q) { }
    CThingCreatureBase* Get() const { return p; }
    CThingCreatureBase* operator->() const { return p; }
};
struct CWorld { int frame; int GetFrameNumber() { return frame; } };
static CWorld g_world;
static CWorld* g_pWorld = &g_world;
static int g_attempts = 0;
struct CombatDialogueManager {
    CPtr PCurrentTalker;
    unsigned int CurrentTalkerTextGroupIndex;
    int LastSpeechFrame;
    void AttemptToSpeak() { ++g_attempts; }
    void Update()
    {
        if (PCurrentTalker.Get() == 0 || PCurrentTalker->Dead)
            return;
        if (CurrentTalkerTextGroupIndex != 0)
        {
            AttemptToSpeak();
            return;
        }
        CTCTalk* talk;
        if (PCurrentTalker->GetInterface(&talk, 0xf) && talk->IsTalking())
            return;
        PCurrentTalker = CPtr(0);
        LastSpeechFrame = g_pWorld->GetFrameNumber();
    }
};
static int Fail() { std::printf("008af780_TEST FAIL\n"); return 1; }
int main()
{
    CTCTalk talk; talk.talking = true;
    CThingCreatureBase c; c.Dead = 0; c.talk = &talk;
    CombatDialogueManager m; m.CurrentTalkerTextGroupIndex = 3; m.LastSpeechFrame = -1; g_world.frame = 50;
    m.Update(); if (g_attempts != 0 || m.LastSpeechFrame != -1) return Fail();      // no talker
    m.PCurrentTalker = CPtr(&c);
    m.Update(); if (g_attempts != 1) return Fail();                                 // pending group retried
    m.CurrentTalkerTextGroupIndex = 0;
    m.Update(); if (m.PCurrentTalker.p != &c || m.LastSpeechFrame != -1) return Fail();   // still talking
    talk.talking = false;
    m.Update(); if (m.PCurrentTalker.p != 0 || m.LastSpeechFrame != 50) return Fail();
    m.PCurrentTalker = CPtr(&c); c.Dead = 1; m.LastSpeechFrame = 0;
    m.Update(); if (m.PCurrentTalker.p != &c || m.LastSpeechFrame != 0) return Fail(); // dead talker: nothing
    c.Dead = 0; c.talk = 0;
    m.Update(); if (m.PCurrentTalker.p != 0 || m.LastSpeechFrame != 50) return Fail(); // no CTCTalk: dropped
    std::printf("008af780_TEST PASS\n");
    return 0;
}