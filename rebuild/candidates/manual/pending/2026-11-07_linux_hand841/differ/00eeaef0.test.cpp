// Behaviour test for CExpression_PickpocketScript::Main (0x00EEAEF0): the frame count is time / ((skill+1)/(max+1)) * fps; each
// frame progress = 1 - count/time (capped at 1); releasing the button ends the attempt (an unexpired spot is forgiven), a hit fails
// it, a spot whose timer runs out ends it; a full bar ends it; afterwards a spotted hero triggers the event, else the theft succeeds
// when rand()%100 < progress*100.
#include <cstdio>

struct Pickpocket {
    float Time; long Count, LastCount, SpottedTimer; bool Done, Failed, Spotted; float Progress;
    void Setup(float seconds, long level, long max, long fps)
    {
        Time = seconds; Time /= (level + 1.0f) / (max + 1.0f); Time *= fps; Count = (long)Time; LastCount = Count;
        SpottedTimer = 0; Done = Failed = Spotted = false;
    }
    void Frame(bool held, bool hit, long roll, long chance, long spotFrames)
    {
        if (!held) { if (Spotted && SpottedTimer > 0) Spotted = false; Done = true; }
        if (hit) { Done = true; Failed = true; }
        if (Spotted && SpottedTimer == 0) Done = true;
        Progress = 1.0f - Count / Time;
        if (Progress > 1.0f) Progress = 1.0f;
        if (!(Progress >= 1.0f)) {
            if (Count < LastCount) {
                if (roll < chance) { Spotted = true; SpottedTimer = spotFrames; }
                LastCount = Count;
            }
        }
        else Done = true;
        if (Count > 0) --Count;
        if (SpottedTimer > 0) --SpottedTimer;
    }
    bool Steals(long roll) const { return !Failed && !Spotted && roll < Progress * 100.0f; }
};

int main()
{
    Pickpocket p;
    p.Setup(2.0f, 1, 3, 10);   bool ok = p.Count == 40;
    p.Frame(true, false, 99, 10, 5); ok = ok && !p.Done && p.Count == 39 && p.Progress == 0.0f;
    p.Frame(true, false, 0, 10, 5);  ok = ok && p.Spotted && p.SpottedTimer == 4;
    p.Frame(false, false, 99, 10, 5); ok = ok && p.Done && !p.Spotted && !p.Failed;
    ok = ok && p.Steals(0) && !p.Steals(99);
    p.Setup(1.0f, 0, 0, 4);
    p.Frame(true, true, 99, 0, 0);   ok = ok && p.Done && p.Failed && !p.Steals(0);
    if (!ok) { std::printf("00eeaef0_TEST FAIL\n"); return 1; }
    std::printf("00eeaef0_TEST PASS\n");
    return 0;
}
