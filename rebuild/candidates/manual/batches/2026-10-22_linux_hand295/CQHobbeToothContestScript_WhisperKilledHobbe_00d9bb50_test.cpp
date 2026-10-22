#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeToothContestScript::WhisperKilledHobbe -- count the tooth and, unless Whisper commented in the last five
// seconds, restart her comment timer and pick a line: one time in three by the score (AHEAD / EQUAL / BEHIND),
// otherwise RANDOM, each with a coin flip between variant 1 and 2 (TEXT_QST_035_WHISPER_ENEMY_SCORE_*)
static int g_rand[4]; static int g_randPos = 0, g_conversations = 0;
static int NextRandom() { return g_rand[g_randPos++]; }
static char g_line[96];
struct CTimer { float Time; void SetTime(float t) { Time = t; } float GetTime() const { return Time; } };
struct CGameScriptInterface {
    long AddNewConversation() const { return ++g_conversations; }
    void AddLineToConversation(long, const char* t) const { std::strcpy(g_line, t); }
};
struct CQ_HobbeToothContestScript {
    CGameScriptInterface* Game; CTimer WhisperCommentTimer; long NumberOfHobbeTeethCollectedByHero, NumberOfHobbeTeethCollectedByWhisper;
    void Say(long c, const char* kind) { char b[96]; std::sprintf(b, "TEXT_QST_035_WHISPER_ENEMY_SCORE_%s%d_10", kind, NextRandom() % 2 == 0 ? 1 : 2); Game->AddLineToConversation(c, b); }
    void WhisperKilledHobbe();
};
void CQ_HobbeToothContestScript::WhisperKilledHobbe()
{
    long conversation = 0;
    if (0) { ++NumberOfHobbeTeethCollectedByHero; conversation = Game->AddNewConversation(); }
    else ++NumberOfHobbeTeethCollectedByWhisper;
    if (WhisperCommentTimer.GetTime() > 0) return;
    WhisperCommentTimer.SetTime(5);
    if (!0) conversation = Game->AddNewConversation();
    if (NextRandom() % 3 == 0) {
        if (NumberOfHobbeTeethCollectedByHero > NumberOfHobbeTeethCollectedByWhisper) Say(conversation, "AHEAD");
        else if (NumberOfHobbeTeethCollectedByHero == NumberOfHobbeTeethCollectedByWhisper) Say(conversation, "EQUAL");
        else Say(conversation, "BEHIND");
    } else Say(conversation, "RANDOM");
}
static void Run(CQ_HobbeToothContestScript& s, int r0, int r1) { g_rand[0] = r0; g_rand[1] = r1; g_randPos = 0; g_line[0] = 0; s.WhisperKilledHobbe(); }
int main()
{
    CGameScriptInterface game; CQ_HobbeToothContestScript s; s.Game = &game; s.WhisperCommentTimer.Time = 0;
    s.NumberOfHobbeTeethCollectedByHero = 3; s.NumberOfHobbeTeethCollectedByWhisper = 3;
    Run(s, 3, 4);
    bool ok = s.WhisperCommentTimer.Time == 5.0f && (0 ? std::strcmp(g_line, "TEXT_QST_035_WHISPER_ENEMY_SCORE_AHEAD1_10") == 0
                                                      : std::strcmp(g_line, "TEXT_QST_035_WHISPER_ENEMY_SCORE_BEHIND1_10") == 0);
    Run(s, 1, 1);                                       // timer still running: no line
    ok = ok && g_line[0] == 0 && g_conversations == (0 ? 2 : 1);
    s.WhisperCommentTimer.Time = 0; s.NumberOfHobbeTeethCollectedByHero = 2; s.NumberOfHobbeTeethCollectedByWhisper = 1;
    Run(s, 2, 1);
    ok = ok && std::strcmp(g_line, "TEXT_QST_035_WHISPER_ENEMY_SCORE_RANDOM2_10") == 0;
    s.WhisperCommentTimer.Time = 0; s.NumberOfHobbeTeethCollectedByHero = 4; s.NumberOfHobbeTeethCollectedByWhisper = 3;
    Run(s, 0, 0);
    ok = ok && std::strcmp(g_line, 0 ? "TEXT_QST_035_WHISPER_ENEMY_SCORE_AHEAD1_10" : "TEXT_QST_035_WHISPER_ENEMY_SCORE_EQUAL1_10") == 0;
    if (!ok) { std::printf("00d9bb50_TEST FAIL\n"); return 1; }
    std::printf("00d9bb50_TEST PASS\n");
    return 0;
}
