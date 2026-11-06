// Behaviour test for CQ_NewOakValeIntroScript::CNOVI_BarrelThug::Main (0x00DB6C60): the watch-timer tempt lines fire once
// each as the timer counts down past 45, 38, 34, 30, 25, 20, 10 (LastTimeSpoken remembers the last), and a hit adds bad deed 2.
#include <cstdio>

struct Thug {
    long LastTimeSpoken, Spoken, BadDeed;
    void Tick(long time)
    {
        static const long marks[7] = { 45, 38, 34, 30, 25, 20, 10 };
        for (int i = 0; i < 7; ++i)
            if (time < marks[i] && LastTimeSpoken > marks[i]) { ++Spoken; LastTimeSpoken = time; return; }
    }
    void Hit() { BadDeed = 2; }
};

int main()
{
    Thug t = { 60, 0, 0 };
    for (long time = 59; time > 0; --time) t.Tick(time);
    bool ok = t.Spoken == 7 && t.LastTimeSpoken == 9;
    t.Hit(); ok = ok && t.BadDeed == 2;
    if (!ok) { std::printf("00db6c60_TEST FAIL\n"); return 1; }
    std::printf("00db6c60_TEST PASS\n");
    return 0;
}