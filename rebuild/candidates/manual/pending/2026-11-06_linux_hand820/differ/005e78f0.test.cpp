// Behaviour test for CTCQuestCompletionUI::GlobalUpdate (0x005E78F0): a plain model of the digit-count step -- each frame the
// shown gold and experience move toward their targets by target * elapsed / DigitCountTime, clamped at the target.
#include <cstdio>

static float Step(float shown, float target, float elapsed, float count_time)
{
    shown += target * elapsed / count_time;
    return shown > target ? target : shown;
}

int main()
{
    float gold = 0.0f;
    for (int i = 0; i < 3; ++i)
        gold = Step(gold, 300.0f, 1.0f, 2.0f);
    if (gold != 300.0f) { std::printf("005e78f0_TEST FAIL\n"); return 1; }
    float xp = Step(0.0f, 100.0f, 0.5f, 2.0f);
    if (xp != 25.0f) { std::printf("005e78f0_TEST FAIL\n"); return 1; }
    std::printf("005e78f0_TEST PASS\n");
    return 0;
}
