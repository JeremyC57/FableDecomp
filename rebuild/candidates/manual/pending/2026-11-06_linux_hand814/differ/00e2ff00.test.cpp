// Behaviour test for NScript::CV_AmbushScamScript::CAS_Buddy::Main (0x00E2FF00): the buddy waits for the trickster, follows the hero
// into the ambush (ambushers become enemies), helps when close, then wanders back; when the hero attacks him he turns hostile; when
// talked to he answers by quest state; when the quest ends he walks to his exit point and fades out. The assist line number:
// 10 the first time (and the counter counts up), 20 once the counter passed 1. A log model of the branch order.
#include <cstdio>
#include <cstring>
static int AssistLine(int& counter) { int line = 10; if (counter > 1) line = 20; else counter++; return line; }
static const char* TalkLine(bool complete, bool helped, bool ambush, bool allDead, bool approach, bool introDone)
{
    if (complete) return helped ? "HELPED" : "END";
    if (ambush) return allDead ? "AMBUSH_FINISHED" : "AMBUSH_STARTED";
    if (approach) return "CAUTION";
    return introDone ? "OFFER" : "INTRO";
}
int main()
{
    int c = 0;
    bool ok = AssistLine(c) == 10 && c == 1 && AssistLine(c) == 10 && c == 2 && AssistLine(c) == 20 && c == 2;
    ok = ok && !std::strcmp(TalkLine(true, true, true, true, true, true), "HELPED") && !std::strcmp(TalkLine(true, false, false, false, false, false), "END")
        && !std::strcmp(TalkLine(false, false, true, false, false, false), "AMBUSH_STARTED") && !std::strcmp(TalkLine(false, false, true, true, false, false), "AMBUSH_FINISHED")
        && !std::strcmp(TalkLine(false, false, false, false, true, false), "CAUTION") && !std::strcmp(TalkLine(false, false, false, false, false, false), "INTRO")
        && !std::strcmp(TalkLine(false, false, false, false, false, true), "OFFER");
    std::printf(ok ? "00e2ff00_TEST PASS\n" : "00e2ff00_TEST FAIL\n");
    return ok ? 0 : 1;
}
