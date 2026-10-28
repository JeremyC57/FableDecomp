// behaviour test for 0x00843940 CCreatureAction_PlayAnimationWithLookTurning::FinishAction():
// after the base FinishAction, a creature that is not aborting and a looping action with loops left gets a fresh copy
// of the action as its current action, with the loop count decremented (-1 = forever stays -1)
#include <cstdio>
struct Creature {
    bool aborting; int setCount; long lastLoops; bool lastLooping;
    Creature() : aborting(false), setCount(0), lastLoops(99), lastLooping(false) {}
};
static int gBaseFinish = 0;
struct Action {
    Creature* PCreature; bool Looping; long NumberOfTimesToLoop;
    Action(Creature* c, bool looping, long n) : PCreature(c), Looping(looping), NumberOfTimesToLoop(n) {}
    void BaseFinishAction() { ++gBaseFinish; }
    void FinishAction()
    {
        BaseFinishAction();
        if (PCreature->aborting)
            return;
        if (Looping && NumberOfTimesToLoop != 0) {
            long loops;
            if (NumberOfTimesToLoop == -1) loops = -1; else loops = NumberOfTimesToLoop - 1;
            Action next(PCreature, Looping, loops);
            PCreature->setCount++; PCreature->lastLoops = next.NumberOfTimesToLoop; PCreature->lastLooping = next.Looping;
        }
    }
};
int main()
{
    int fails = 0;
    Creature c;
    Action a1(&c, true, 3); a1.FinishAction();
    if (c.setCount != 1 || c.lastLoops != 2 || !c.lastLooping) fails |= 1;
    Action a2(&c, true, -1); a2.FinishAction();
    if (c.setCount != 2 || c.lastLoops != -1) fails |= 2;
    Action a3(&c, true, 0); a3.FinishAction();
    Action a4(&c, false, 5); a4.FinishAction();
    if (c.setCount != 2) fails |= 4;
    c.aborting = true; Action a5(&c, true, 4); a5.FinishAction();
    if (c.setCount != 2 || gBaseFinish != 5) fails |= 8;
    if (fails) { std::printf("00843940_TEST FAIL %d\n", fails); return 1; }
    std::printf("00843940_TEST PASS\n");
    return 0;
}