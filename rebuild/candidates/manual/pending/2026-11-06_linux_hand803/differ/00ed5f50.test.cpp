// Behaviour test for CV_SwordInTheStoneScript::CSwordCrowd::Main (0x00ED5F50): a crowd member only starts a pull when the crowd
// timer is zero, no pull is active, nobody is attempting and his coin toss is odd; the pull marks the crowd busy, makes the
// sword unusable, and when it ends everything is reset and the sword is usable again.
#include <cstdio>

struct Crowd {
    bool CrowdSwordPullActive, CrowdAttemptingToPullSword, SwordUsable; long Timer, Pulls;
    void Turn(int toss)
    {
        if (Timer != 0 || CrowdSwordPullActive) return;
        if (!CrowdAttemptingToPullSword && toss % 2 == 1) {
            CrowdSwordPullActive = true; CrowdAttemptingToPullSword = true; SwordUsable = false; ++Pulls;
            Timer = 10;
        }
        CrowdAttemptingToPullSword = false; SwordUsable = true; CrowdSwordPullActive = false;
    }
};

int main()
{
    Crowd c = { false, false, true, 0, 0 };
    c.Turn(2);
    bool ok = c.Pulls == 0 && c.SwordUsable;
    c.Turn(3);
    ok = ok && c.Pulls == 1 && c.SwordUsable && !c.CrowdSwordPullActive && c.Timer == 10;
    c.Turn(5);
    ok = ok && c.Pulls == 1;
    if (!ok) { std::printf("00ed5f50_TEST FAIL\n"); return 1; }
    std::printf("00ed5f50_TEST PASS\n");
    return 0;
}
