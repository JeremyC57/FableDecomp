// Behaviour test for NScript::CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main (0x00D58490): models the
// Guildmaster's decisions -- the hit-whisper condition, the two-comment switch, the battle score and the A+..F grade
// picked from the seven GUI_MeleeGrades thresholds (first threshold the score reaches; F when none is reached).
#include <stdio.h>
#include <string.h>

static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }

static bool HitWhisper(bool earlyHit, bool arrived, bool hitByHero) { return (earlyHit && arrived) || hitByHero; }

static float Score(float whisperStart, float whisperEnd, float heroStart, float heroEnd) {
    float whisperEndHealth = whisperEnd;
    float heroEndHealth = heroEnd;
    return (whisperStart - whisperEndHealth) - (heroStart - heroEndHealth);
}

static long Grade(const float* grades, float score) {
    long grade = 0;
    for (long i = 0; i < 7; ++i) {
        grade = i;
        if (grades[i] <= score) break;
    }
    return grade;
}

static const char* GradeText(long grade) {
    switch (grade) {
    case 0: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS";
    case 1: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A";
    case 2: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B";
    case 3: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C";
    case 4: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D";
    case 5: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E";
    case 6: return "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F";
    }
    return "";
}

// commentCount 0 -> first comment and advance to 1; 1 -> second comment, stays 1 (repeats on every 10 s timer).
static const char* Comment(long& commentCount) {
    switch (commentCount) {
    case 0: commentCount = 1; return "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST";
    case 1: return "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND";
    }
    return 0;
}

int main() {
    check(HitWhisper(true, true, false), "early hit after arrival");
    check(!HitWhisper(true, false, false), "early hit before arrival");
    check(HitWhisper(false, false, true), "hit by hero");
    check(!HitWhisper(false, true, false), "nothing");

    check(Score(100.0f, 40.0f, 100.0f, 90.0f) == 50.0f, "score 60 dealt - 10 taken");
    check(Score(100.0f, 100.0f, 100.0f, 70.0f) == -30.0f, "score none dealt");

    const float grades[7] = { 80.0f, 60.0f, 40.0f, 20.0f, 0.0f, -20.0f, -40.0f };
    check(Grade(grades, 90.0f) == 0, "A+");
    check(Grade(grades, 80.0f) == 0, "A+ at threshold");
    check(Grade(grades, 79.0f) == 1, "A");
    check(Grade(grades, 25.0f) == 3, "C");
    check(Grade(grades, -30.0f) == 6, "F on last threshold");
    check(Grade(grades, -1000.0f) == 6, "F when none reached");
    check(strcmp(GradeText(Grade(grades, 50.0f)), "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B") == 0, "B text");

    long commentCount = 0;
    check(strcmp(Comment(commentCount), "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST") == 0 && commentCount == 1, "first comment");
    check(strcmp(Comment(commentCount), "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND") == 0 && commentCount == 1, "second comment");
    check(strcmp(Comment(commentCount), "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND") == 0, "second repeats");

    if (failures == 0) printf("00d58490_TEST PASS\n");
    return failures;
}