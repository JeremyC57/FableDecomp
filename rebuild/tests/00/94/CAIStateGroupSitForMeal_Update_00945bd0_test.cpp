// Behaviour test for CAIStateGroup_SitForMeal::Update() (0x00945BD0): a model of the row -- no eating component: terminate;
// a live pot is remembered and, when food is taken, the creature sits on a free chair (looping the do-nothing action while it
// keeps running and the group stays valid) or stands around eating; every path terminates exactly once.
#include <stdio.h>
struct Group {
    bool canEat, potLive, havePot, food, chair, sitDown; int validLoops, doNothingRunning;
    int terminates, standEats, seats, releases, doNothings; bool potRemembered;
    bool IsStateGroupStillValid() { return validLoops-- > 0; }
    int AIAction_DoNothing() { ++doNothings; return doNothingRunning-- > 0 ? 4 : 2; }
    void Update()
    {
        if (!canEat) { ++terminates; return; }
        if (havePot && potLive) {
            potRemembered = true;
            if (!food) { ++terminates; return; }
            if (chair) {
                ++releases; ++seats;
                if (sitDown) {
                    do {
                        if (AIAction_DoNothing() != 4)
                            break;
                    } while (IsStateGroupStillValid());
                }
            } else {
                ++standEats;
            }
        }
        ++terminates;
    }
};
static Group Make() { Group g = { true, true, true, true, true, true, 0, 0, 0, 0, 0, 0, 0, false }; return g; }
int main()
{
    Group g = Make(); g.canEat = false; g.Update();
    int ok = g.terminates == 1 && !g.potRemembered;
    g = Make(); g.food = false; g.Update(); ok = ok && g.terminates == 1 && g.potRemembered && g.seats == 0;
    g = Make(); g.chair = false; g.Update(); ok = ok && g.terminates == 1 && g.standEats == 1;
    g = Make(); g.doNothingRunning = 5; g.validLoops = 2; g.Update(); ok = ok && g.terminates == 1 && g.doNothings == 3 && g.seats == 1;
    g = Make(); g.doNothingRunning = 1; g.validLoops = 9; g.Update(); ok = ok && g.doNothings == 2;
    g = Make(); g.potLive = false; g.Update(); ok = ok && g.terminates == 1 && !g.potRemembered;
    printf(ok ? "00945bd0_TEST PASS\n" : "00945bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}