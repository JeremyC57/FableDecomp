#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct Script {
    int GenericTutorialCounter, TutorialState;
    bool TalkedToWhisper, WhisperArrived, EarlyHitWhisper, WhisperStopWalking, MeleeRepeating, MeleeRepeatKnown, MeleeOpponentReset;
    void Init()
    {
        GenericTutorialCounter = 0;
        TutorialState = 1;
        TalkedToWhisper = false;
        EarlyHitWhisper = false;
        WhisperArrived = false;
        WhisperStopWalking = false;
        MeleeRepeating = true;
        MeleeRepeatKnown = false;
        MeleeOpponentReset = false;
    }
};
int main()
{
    int ok = 1;
    Script s; s.GenericTutorialCounter = 9; s.TutorialState = 4;
    s.TalkedToWhisper = s.WhisperArrived = s.EarlyHitWhisper = s.WhisperStopWalking = s.MeleeRepeatKnown = s.MeleeOpponentReset = true; s.MeleeRepeating = false;
    s.Init();
    ok = s.GenericTutorialCounter == 0 && s.TutorialState == 1 && !s.TalkedToWhisper && !s.WhisperArrived && !s.EarlyHitWhisper
      && !s.WhisperStopWalking && s.MeleeRepeating && !s.MeleeRepeatKnown && !s.MeleeOpponentReset;
    if (!ok) { std::printf("00d55da0_TEST FAIL\n"); return 1; }
    std::printf("00d55da0_TEST PASS\n");
    return 0;
}