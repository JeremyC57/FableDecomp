// Behaviour test for NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main (0x00D5E0C0): the Guildmaster's will lesson.
// While the lesson waits (TutorialState 1) he comments every 10 frames when the hero is within 5.5 units (FIRST / SECOND
// alternately) and a chat starts the lesson (or says NOT_START while another guild test runs). Each round (repeatable): the lightning
// cut scene and the zap instructions (Xbox / PC text, logbook entry), wait for the first dummy hit (WillScore), the test cut scene,
// the ZAPTEST instructions, the first spell, then the timed test (GUI_WillTimer frames; a guild warning disqualifies; NO_WILL every
// 8 frames with no will energy). Afterwards the disqualified cut scene or the grade (the first of the seven GUI_WillGrades the score
// reaches, else F) with the won cut scenes, then the repeat question: RETAKE replays the round; CONTINUE plays the Whisper cut scene
// and asks whether to play her -- PLAY ends training on the spot (AVI, world reset, state 4, TestFinished) and idles, otherwise
// the lesson closes normally. After the lesson: targeting type, state 4, TestFinished, the will apprentice, the woods quest (unless
// the bandits are beaten), objective 06; then forever: BanditsDefeated once the woods quest is inactive, and a chat asks the END
// question (YES: end training; NO: END_QUESTION_NO and back to his marker). A frame-stepped model with the engine calls logged.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Guildmaster {
    long TutorialState; bool TestFinished, BanditsDefeated;                         // CQ_GuildTrainingWillScript
    bool HeroTakingGuildTest, WillTrainingStarted, WillTestOccuring, GuildWarningOccuring; long WillScore;   // master data
    float Grades[7]; long WillTimer;                                                // CScriptDef GUI_WillGrades / GUI_WillTimer
    int Frame, TerminateAt, TalkAt[6], NTalk, FirstHitAt, SpellAt, WarnAt, ScorePerFrame, Answers[6], NAnswer, WoodsQuestDoneAt; float HeroDist; bool Xbox, OtherTest, WoodsQuestActive;
    bool Idle;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (Frame == WarnAt) GuildWarningOccuring = true; if (Frame == WoodsQuestDoneAt) WoodsQuestActive = false; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    void EndTraining() { Log("FadeOut"); Log("2_guild_split_2_comp.xmv"); Log("ConfiscateWeapons"); if (WoodsQuestActive) Log("DeactivateWoodsQuest"); }
    void EndWorld() { Log("TimeOfDay11"); Log("Adult"); Log("WhisperApprentice"); }
    // false when the thread terminated or training ended inside the question
    bool RepeatAnswer(bool& repeat)
    {
        if (Answers[NAnswer++] == 1) {
            Log("CS_GUILD_WILL_CONTINUE"); repeat = false;
            if (Answers[NAnswer++] != 1) Log("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES");
            else {
                Log("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO"); EndTraining(); EndWorld(); Log("OBJECTIVE_06");
                HeroTakingGuildTest = false; TutorialState = 4; TestFinished = true; Idle = true;
                while (!T()) NSF();
                return false;
            }
        } else { Log("CS_GUILD_MELEE_REPEAT"); repeat = true; }
        return true;
    }
    void Main()
    {
        NSF(); if (T()) return;
        Log("StartScripting");
        long CommentTimer = 0, commentCount = 0;
        if (!TestFinished) {
            Log("MoveToTeacherStand");
            while (TutorialState == 1) {
                NSF(); if (CommentTimer > 0) --CommentTimer; if (T()) return;
                if (Talked()) { if (HeroTakingGuildTest) Log("TEXT_QST_028_GUILDMASTER_WILL_NOT_START"); else TutorialState = 2; }
                if (HeroDist < 5.5f && CommentTimer <= 0) {
                    CommentTimer = 10;
                    Log(commentCount == 0 ? "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST" : "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND");
                    commentCount = 1 - commentCount;
                }
            }
            if (T()) return;
            HeroTakingGuildTest = true;
            bool repeat = true;
            while (repeat) {
                TutorialState = 2; WillTrainingStarted = true;
                Log("CS_GUILD_WILL_LIGHTNING");
                Log(Xbox ? "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP" : "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"); Log("TEXT_QST_LOG_COMBAT_USINGSPELLS");
                WillScore = 0;
                while (WillScore == 0) { NSF(); if (T()) return; if (Frame >= FirstHitAt) WillScore = 1; }
                TutorialState = 3; Log("CS_GUILD_WILL_TEST");
                Log(Xbox ? "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST" : "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC");
                while (Frame < SpellAt) { NSF(); if (T()) return; }
                long TestTimer = WillTimer, NoWillTimer = 0; bool talked = false;
                WillScore = 0;
                while (TestTimer > 0 && !talked) {
                    NSF(); --TestTimer; if (NoWillTimer > 0) --NoWillTimer; if (T()) return;
                    WillTestOccuring = true; WillScore += ScorePerFrame;
                    if (GuildWarningOccuring) talked = true;
                }
                WillTestOccuring = false; TutorialState = 0;
                if (talked) Log("CS_GUILD_WILL_DISQUALIFIED");
                else {
                    float score = (float)WillScore; long grade = 0;
                    for (long i = 0; i < 7; ++i) { grade = i; if (Grades[i] <= score) break; }
                    static const char* const name[7] = { "GRADE_APLUS", "GRADE_A", "GRADE_B", "GRADE_C", "GRADE_D", "GRADE_E", "GRADE_F" };
                    Log(name[grade]); Log("CS_GUILD_WILL_WON");
                }
                if (!RepeatAnswer(repeat)) return;
            }
        }
        HeroTakingGuildTest = false; TutorialState = 4; TestFinished = true;
        Log("MoveToGuildmasterMarker"); Log("WillApprentice");
        if (!BanditsDefeated) { Log("Q_GuildTrainingWoodsWill"); WoodsQuestActive = true; }
        Log("OBJECTIVE_06");
        for (;; NSF()) {
            if (T()) break;
            if (!BanditsDefeated && !WoodsQuestActive) BanditsDefeated = true;
            if (Talked()) {
                if (Answers[NAnswer++] == 1) { EndTraining(); EndWorld(); }
                else { Log("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO"); Log("MoveToGuildmasterMarker"); }
            }
        }
    }
};
static Guildmaster Make()
{
    Guildmaster g; std::memset(&g, 0, sizeof g);
    static const float grades[7] = { 60.0f, 50.0f, 40.0f, 30.0f, 20.0f, 10.0f, 0.0f };
    for (int i = 0; i < 7; ++i) g.Grades[i] = grades[i];
    g.TutorialState = 1; g.WillTimer = 10; g.TerminateAt = 200; g.WarnAt = -1; g.WoodsQuestDoneAt = -1; g.HeroDist = 3.0f; g.FirstHitAt = 12; g.SpellAt = 14;
    return g;
}
int main()
{
    bool ok = true;
    {   // comments, the lesson, grade B (4 points a frame over 10 frames), retake, grade B again, continue, do not play Whisper; end question NO
        Guildmaster g = Make(); g.TalkAt[0] = 9; g.TalkAt[1] = 70; g.NTalk = 2; g.ScorePerFrame = 4;
        g.Answers[0] = 0; g.Answers[1] = 1; g.Answers[2] = 0; g.Answers[3] = 0; g.WoodsQuestDoneAt = 60; g.TerminateAt = 80;
        g.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;MoveToTeacherStand;TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST;CS_GUILD_WILL_LIGHTNING;"
            "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC;TEXT_QST_LOG_COMBAT_USINGSPELLS;CS_GUILD_WILL_TEST;TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC;GRADE_B;"
            "CS_GUILD_WILL_WON;CS_GUILD_MELEE_REPEAT;CS_GUILD_WILL_LIGHTNING;TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC;TEXT_QST_LOG_COMBAT_USINGSPELLS;"
            "CS_GUILD_WILL_TEST;TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC;GRADE_B;CS_GUILD_WILL_WON;CS_GUILD_WILL_CONTINUE;"
            "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES;MoveToGuildmasterMarker;WillApprentice;Q_GuildTrainingWoodsWill;OBJECTIVE_06;"
            "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO;MoveToGuildmasterMarker;")
            && g.TestFinished && g.TutorialState == 4 && g.BanditsDefeated && !g.HeroTakingGuildTest && g.WillTrainingStarted && !g.Idle;
    }
    g_log[0] = 0;
    {   // another test running: NOT_START; then disqualified by a guild warning, continue and play Whisper: training ends inside the question
        Guildmaster g = Make(); g.HeroTakingGuildTest = true; g.TalkAt[0] = 3; g.NTalk = 1; g.HeroDist = 9.0f; g.TerminateAt = 5; g.Xbox = true;
        g.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;MoveToTeacherStand;TEXT_QST_028_GUILDMASTER_WILL_NOT_START;") && g.TutorialState == 1;
        g_log[0] = 0;
        Guildmaster h = Make(); h.TalkAt[0] = 2; h.NTalk = 1; h.HeroDist = 9.0f; h.WarnAt = 17; h.Xbox = true; h.Answers[0] = 1; h.Answers[1] = 1; h.BanditsDefeated = true;
        h.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;MoveToTeacherStand;CS_GUILD_WILL_LIGHTNING;TEXT_QST_028_WILL_INSTRUCTIONS_ZAP;"
            "TEXT_QST_LOG_COMBAT_USINGSPELLS;CS_GUILD_WILL_TEST;TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST;CS_GUILD_WILL_DISQUALIFIED;CS_GUILD_WILL_CONTINUE;"
            "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO;FadeOut;2_guild_split_2_comp.xmv;ConfiscateWeapons;TimeOfDay11;Adult;WhisperApprentice;OBJECTIVE_06;")
            && h.Idle && h.TestFinished && h.TutorialState == 4 && h.Frame == 200 && !h.WillTestOccuring;
    }
    g_log[0] = 0;
    {   // the grade table: the first grade whose threshold the score reaches; below every threshold is F; loaded with the test finished
        Guildmaster g = Make(); g.TestFinished = true; g.TutorialState = 4; g.TerminateAt = 6; g.TalkAt[0] = 4; g.NTalk = 1; g.Answers[0] = 1; g.BanditsDefeated = true;
        g.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;MoveToGuildmasterMarker;WillApprentice;OBJECTIVE_06;FadeOut;2_guild_split_2_comp.xmv;ConfiscateWeapons;TimeOfDay11;Adult;WhisperApprentice;");
    }
    std::printf(ok ? "00d5e0c0_TEST PASS\n" : "00d5e0c0_TEST FAIL\n");
    return ok ? 0 : 1;
}