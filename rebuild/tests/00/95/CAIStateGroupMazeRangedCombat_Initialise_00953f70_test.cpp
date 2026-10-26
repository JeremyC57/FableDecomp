// behaviour test for 0x00953F70 CAIStateGroup_MazeRangedCombat::Initialise(...): the five arguments reach the base initialisation unchanged, and afterwards LastCompletedPhase is -1 whatever it held before
#include <cstdio>
struct Base {
    int gotBrain, gotCreature, gotPriority, gotScript, gotName, calls;
    Base() : gotBrain(0), gotCreature(0), gotPriority(0), gotScript(0), gotName(0), calls(0) { }
    void Initialise(int brain, int creature, int priority, int script, int name)
    { gotBrain = brain; gotCreature = creature; gotPriority = priority; gotScript = script; gotName = name; calls++; }
};
struct Group : Base {
    int LastCompletedPhase;
    void Initialise(int brain, int creature, int priority, int script, int name)   // the function under test
    {
        Base::Initialise(brain, creature, priority, script, name);
        LastCompletedPhase = -1;
    }
};
int main()
{
    int fail = 0;
    Group g; g.LastCompletedPhase = 12345;
    g.Initialise(1, 2, 3, 4, 5);
    if (g.calls != 1 || g.gotBrain != 1 || g.gotCreature != 2 || g.gotPriority != 3 || g.gotScript != 4 || g.gotName != 5) fail |= 1;
    if (g.LastCompletedPhase != -1) fail |= 2;
    if (fail) { std::printf("00953f70_TEST FAIL %d\n", fail); return 1; }
    std::printf("00953f70_TEST PASS\n");
    return 0;
}