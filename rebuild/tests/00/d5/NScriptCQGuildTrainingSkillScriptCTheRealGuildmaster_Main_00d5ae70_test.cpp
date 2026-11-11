// Behaviour test for NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main (0x00D5AE70): a model of its rules. The walk-up
// comment alternates FIRST / SECOND (commentCount = 1 - commentCount). Each practice round shows three control ticks (unsheathe,
// a middle one, fire): the lock-target tick in the first round, the first-person-view tick (its own variable) in the second; they
// are updated every frame and removed exactly once, either when all three controls are held or at the end of the round. The result
// grade is the first index i (0..6) whose threshold GUI_SkillGrades[i] is <= the score; with no such index it stays at 6.
#include <cstdio>
#include <cstring>
static const char* Comment(long& count)
{
    const char* text = count == 0 ? "COMMENT_FIRST" : "COMMENT_SECOND";
    count = 1 - count;
    return text;
}
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
// One practice round: frames[] holds bit 0 = bow carried, bit 1 = the middle control, bit 2 = fire held.
static void Round(const char* middle, const int* frames, int count)
{
    bool ticksRemoved = false;
    Log("add:unsheathe"); Log(middle); Log("add:fire");
    for (int f = 0; f < count; ++f) {
        if (!ticksRemoved && frames[f] == 7) { ticksRemoved = true; Log("remove3"); }
        else if (!ticksRemoved) Log("update3");
    }
    if (!ticksRemoved) Log("remove3_end");
}
static long Grade(const float* thresholds, float score)
{
    long grade = 0;
    long i = 0;
    for (; i < 7; ++i) {
        grade = i;
        if (thresholds[i] <= score) break;
    }
    return grade;
}
int main()
{
    long c = 0;
    bool ok = !std::strcmp(Comment(c), "COMMENT_FIRST") && !std::strcmp(Comment(c), "COMMENT_SECOND") && !std::strcmp(Comment(c), "COMMENT_FIRST");
    static const int first[] = { 1, 3, 7, 7 };
    static const int second[] = { 1, 5 };
    Round("add:lock_target", first, 4);
    Round("add:first_person_view", second, 2);
    ok = ok && !std::strcmp(g_log, "add:unsheathe;add:lock_target;add:fire;update3;update3;remove3;"
                                   "add:unsheathe;add:first_person_view;add:fire;update3;update3;remove3_end;");
    static const float thresholds[7] = { 600.0f, 500.0f, 400.0f, 300.0f, 200.0f, 100.0f, 50.0f };
    ok = ok && Grade(thresholds, 700.0f) == 0 && Grade(thresholds, 500.0f) == 1 && Grade(thresholds, 250.0f) == 4
            && Grade(thresholds, 75.0f) == 6 && Grade(thresholds, 10.0f) == 6;
    std::printf(ok ? "00d5ae70_TEST PASS\n" : "00d5ae70_TEST FAIL\n");
    return ok ? 0 : 1;
}