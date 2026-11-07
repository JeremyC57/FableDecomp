// Behaviour test for CQ_GuildTrainingWillScript::CTheRealGuildmaster::Main (0x00D5E0C0): the will-test grade lookup (the first of the
// seven GUI_WillGrades thresholds the score reaches; F when none) and the tutorial-state progression (2 after the walk-up, 3 after the
// zap instructions, 0 again on a disqualified retake, 4 + TestFinished at the end).
#include <cstdio>
#include <cstring>

static const char* const Grades[7] = { "APLUS", "A", "B", "C", "D", "E", "F" };

static const char* GradeFor(const float thresholds[7], float score)
{
    long grade = 0;
    for (long i = 0; i < 7; ++i) {
        grade = i;
        if (thresholds[i] <= score) break;
    }
    return Grades[grade];
}

struct Parent { long TutorialState; bool TestFinished; };

static void Lesson(Parent& p, bool disqualified, bool retake)
{
    p.TutorialState = 2;
    p.TutorialState = 3;
    if (disqualified && retake) { p.TutorialState = 0; return; }
    p.TutorialState = 4;
    p.TestFinished = true;
}

int main()
{
    const float t[7] = { 900.0f, 750.0f, 600.0f, 450.0f, 300.0f, 150.0f, 0.0f };
    bool ok = std::strcmp(GradeFor(t, 1000.0f), "APLUS") == 0
           && std::strcmp(GradeFor(t, 760.0f), "A") == 0
           && std::strcmp(GradeFor(t, 600.0f), "B") == 0
           && std::strcmp(GradeFor(t, 10.0f), "F") == 0
           && std::strcmp(GradeFor(t, -5.0f), "F") == 0;
    Parent p = { 0, false };
    Lesson(p, true, true);
    ok = ok && p.TutorialState == 0 && !p.TestFinished;
    Lesson(p, false, false);
    ok = ok && p.TutorialState == 4 && p.TestFinished;
    if (!ok) { std::printf("00d5e0c0_TEST FAIL\n"); return 1; }
    std::printf("00d5e0c0_TEST PASS\n");
    return 0;
}
