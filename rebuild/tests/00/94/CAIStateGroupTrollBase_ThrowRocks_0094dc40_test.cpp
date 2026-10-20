// behaviour test for CAIStateGroup_TrollBase::ThrowRocks (0x0094DC40): logic copy over stubs
#include <cstdio>
struct Output { unsigned long flags; int action; };
struct Troll {
    bool performing; long currentAction; long nextAction; long shotCounter; int posted; Output last;
    void SetBrainOutput(const Output& o) { last = o; posted++; }
    void Yield() { currentAction = nextAction; performing = nextAction != 0; }
    long ThrowRocks()
    {
        long result = 4;
        long previous = 0;
        if (performing) previous = currentAction;
        Output output = { 0, 0 };
        output.flags |= 8;
        output.action = 150;
        SetBrainOutput(output);
        Yield();
        if (performing && currentAction != previous) { ++shotCounter; result = 2; }
        return result;
    }
};
int main()
{
    Troll t = { true, 7, 9, 0, 0, { 0, 0 } };
    bool ok = t.ThrowRocks() == 2 && t.shotCounter == 1 && t.last.flags == 8 && t.last.action == 150;
    t.nextAction = 9;
    if (t.ThrowRocks() != 4 || t.shotCounter != 1) ok = false;          // same action still running
    t.nextAction = 0;
    if (t.ThrowRocks() != 4 || t.posted != 3) ok = false;               // nothing performing after the yield
    if (!ok) { std::printf("0094dc40_TEST FAIL\n"); return 1; }
    std::printf("0094dc40_TEST PASS\n");
    return 0;
}